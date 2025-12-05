/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:23:02 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/05 03:35:50 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "Reactor.hpp"

#include <errno.h>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

static long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL);
  return te.tv_sec * 1000 + te.tv_usec / 1000;
}

#define LOG_DEBUG(msg)                                                         \
  std::cerr << "[" << current_timestamp() << "] [FD " << _fd << "] " << msg    \
            << std::endl

Connection::Connection()
    : _fd(-1), _reactor(NULL), _manager(NULL), _readBuf(), _writeBuf(),
      _msgCb(NULL), _closed(false), _lastActivity(std::time(NULL)) {}

Connection::Connection(const Connection &other)
    : _fd(other._fd), _reactor(other._reactor), _manager(other._manager),
      _readBuf(other._readBuf), _writeBuf(other._writeBuf),
      _msgCb(other._msgCb), _closed(other._closed),
      _lastActivity(other._lastActivity) {}

Connection::Connection(int fd, Reactor *reactor, ConnectionManager *mgr)
    : _fd(fd), _reactor(reactor), _manager(mgr), _readBuf(), _writeBuf(),
      _msgCb(NULL), _closed(false), _lastActivity(std::time(NULL)) {}

Connection::~Connection() {
  if (_fd >= 0)
    ::close(_fd);
}

Connection &Connection::operator=(const Connection &other) {
  if (this != &other) {
    this->_fd = other._fd;
    this->_reactor = other._reactor;
    this->_manager = other._manager;
    this->_readBuf = other._readBuf;
    this->_writeBuf = other._writeBuf;
    this->_msgCb = other._msgCb;
    this->_closed = other._closed;
    this->_lastActivity = other._lastActivity;
  }
  return (*this);
}

int Connection::getFd() const { return _fd; }

void Connection::setMessageCallback(MessageCallback cb) { _msgCb = cb; }

void Connection::touch() { _lastActivity = std::time(NULL); }

std::time_t Connection::getLastActivity() const { return _lastActivity; }

void Connection::handleEvent(uint32_t events) {
  if (_closed)
    return;

  if (events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
    LOG_DEBUG("Event: ERROR/HUP. Closing.");
    close();
    return;
  }

  if (events & EPOLLIN) {
    LOG_DEBUG("Event: EPOLLIN triggered.");
    ssize_t r = handleRead();
    (void)r;
  }

  // Note: If connection closed in handleRead, _closed might be true now.
  if (!_closed && (events & EPOLLOUT)) {
    LOG_DEBUG("Event: EPOLLOUT triggered.");
    ssize_t r = handleWrite();
    (void)r;
  }
}

ssize_t Connection::handleRead() {
  char buf[4096];
  bool keepReading = true;
  ssize_t totalRead = 0;
  int loopCount = 0;

  if (_fd < 0)
    return -1;

  LOG_DEBUG("Entering handleRead loop...");

  while (keepReading) {
    loopCount++;
    ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);

    if (n > 0) {
      _lastActivity = std::time(NULL);
      _readBuf.insert(_readBuf.end(), buf, buf + n);
      totalRead += n;

      LOG_DEBUG("handleRead loop #" << loopCount << ": read " << n
                                    << " bytes. Invoking Callback.");

      // notify dispatcher
      if (_msgCb) {
        bool alive = _msgCb(this, _readBuf);
        if (!alive) {
          LOG_DEBUG("Callback returned false (Connection died via QUIT). "
                    "Returning -1.");
          return -1;
        }
        _readBuf.clear();
      }

      if (static_cast<size_t>(n) < sizeof(buf)) {
        LOG_DEBUG("Read less than buffer size (" << n
                                                 << " < 4096). Socket empty.");
        keepReading = false;
      }

      // Safety break to prevent starvation if client spams faster than we
      // process
      if (loopCount > 50) {
        LOG_DEBUG("STARVATION WARNING: Broke read loop after 50 iterations to "
                  "yield to other clients.");
        keepReading = false;
      }

    } else if (n == 0) {
      LOG_DEBUG("Recv returned 0 (Peer closed).");
      close();
      return 0;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_DEBUG("Recv EAGAIN. Done reading.");
        keepReading = false;
      } else {
        perror("recv");
        close();
        return -1;
      }
    }
  }

  LOG_DEBUG("Exiting handleRead. Total read: " << totalRead);
  return totalRead;
}

ssize_t Connection::handleWrite() {
  if (_fd < 0)
    return -1;

  LOG_DEBUG("Entering handleWrite. Buffer size: " << _writeBuf.size());

  while (!_writeBuf.empty()) {
    ssize_t n = ::send(_fd, &_writeBuf[0], _writeBuf.size(), MSG_NOSIGNAL);
    if (n > 0) {
      _lastActivity = std::time(NULL);
      LOG_DEBUG("handleWrite: Sent " << n << " bytes.");

      if (static_cast<size_t>(n) >= _writeBuf.size()) {
        _writeBuf.clear();
        LOG_DEBUG("Write buffer drained. Removing EPOLLOUT.");
        if (_reactor)
          _reactor->modFd(_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR,
                          this);
        return n;
      } else {
        _writeBuf.erase(_writeBuf.begin(), _writeBuf.begin() + n);
        LOG_DEBUG("Partial write. Remaining: " << _writeBuf.size());
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_DEBUG("send EAGAIN. Buffer full.");
        return 0;
      }
      perror("send");
      close();
      return -1;
    }
  }
  return 0;
}

// overload for convenience
void Connection::send(const std::string &data) {
  std::vector<char> char_data(data.begin(), data.end());
  this->send(char_data);
}
void Connection::send(const std::vector<char> &data) {
  if (_closed || data.empty())
    return;

  ssize_t sent = 0;

  if (_writeBuf.empty()) {
    sent = ::send(_fd, &data[0], data.size(), MSG_NOSIGNAL);

    if (sent < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG_DEBUG("Optimistic send: EAGAIN. Buffering all " << data.size()
                                                            << " bytes.");
        sent = 0;
      } else {
        perror("send");
        close();
        return;
      }
    } else {
      _lastActivity = std::time(NULL);
      LOG_DEBUG("Optimistic send: Directly sent " << sent << "/" << data.size()
                                                  << " bytes.");
    }
  }

  if (static_cast<size_t>(sent) < data.size()) {
    _writeBuf.insert(_writeBuf.end(), data.begin() + sent, data.end());
    LOG_DEBUG("Buffering " << (data.size() - sent)
                           << " bytes. Total WriteBuf: " << _writeBuf.size());

    if (_reactor) {
      _reactor->modFd(
          _fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR, this);
    }
  }
}

void Connection::close() {
  if (_closed)
    return;
  LOG_DEBUG("Closing connection.");
  _closed = true;
  if (_reactor)
    _reactor->delFd(_fd);
  if (_manager)
    _manager->remove(this);
  if (_fd >= 0)
    ::close(_fd);
  _fd = -1;
}
