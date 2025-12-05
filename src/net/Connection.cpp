/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:23:02 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/05 02:33:37 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "Reactor.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

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
    // remote closed or error
    close();
    return;
  }
  if (events & EPOLLIN) {
    ssize_t r = handleRead();
    (void)r;
  }
  if (events & EPOLLOUT) {
    ssize_t r = handleWrite();
    (void)r;
  }
}

ssize_t Connection::handleRead() {
  char buf[4096];
  bool keepReading = true;
  ssize_t totalRead = 0;

  if (_fd < 0)
    return -1;

  // read everything available in the kernel buffer
  while (keepReading) {
    ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);

    if (n > 0) {
      _lastActivity = std::time(NULL);
      _readBuf.insert(_readBuf.end(), buf, buf + n);
      totalRead += n;

      // notify dispatcher if present
      if (_msgCb) {
        // FIX: Check return value. If false, object was deleted.
        bool alive = _msgCb(this, _readBuf);
        if (!alive) {
          return -1; // Stop processing immediately
        }
        _readBuf.clear();
      }

      // If we read less than full buffer, socket is likely empty
      if (static_cast<size_t>(n) < sizeof(buf)) {
        keepReading = false;
      }
    } else if (n == 0) {
      // orderly shutdown by peer
      close();
      return 0;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        keepReading = false; // No more data
      } else {
        // real error
        perror("recv");
        close();
        return -1;
      }
    }
  }
  return totalRead;
}

ssize_t Connection::handleWrite() {
  if (_fd < 0)
    return -1;

  while (!_writeBuf.empty()) {
    ssize_t n = ::send(_fd, &_writeBuf[0], _writeBuf.size(), MSG_NOSIGNAL);
    if (n > 0) {
      _lastActivity = std::time(NULL);

      if (static_cast<size_t>(n) >= _writeBuf.size()) {
        _writeBuf.clear();
        // Buffer empty: remove EPOLLOUT interest
        if (_reactor)
          _reactor->modFd(_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR,
                          this);
        return n;
      } else {
        // partial write: remove sent bytes
        _writeBuf.erase(_writeBuf.begin(), _writeBuf.begin() + n);
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // cannot write more right now
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

  // optimistic write:
  // if buffer empty, try to send directly to socket
  if (_writeBuf.empty()) {
    sent = ::send(_fd, &data[0], data.size(), MSG_NOSIGNAL);

    if (sent < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        sent = 0; // kernel buffer full, must buffer all
      } else {
        perror("send"); // Real error
        close();
        return;
      }
    } else {
      _lastActivity = std::time(NULL);
    }
  }

  // if we couldn't send everything (or anything), buffer the rest
  // it will be sent at the next event loop cycle
  if (static_cast<size_t>(sent) < data.size()) {
    _writeBuf.insert(_writeBuf.end(), data.begin() + sent, data.end());

    // we have data pending, ensure EPOLLOUT is set
    if (_reactor) {
      _reactor->modFd(
          _fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR, this);
    }
  }
}

void Connection::close() {
  if (_closed)
    return;
  _closed = true;
  if (_reactor)
    _reactor->delFd(_fd);
  if (_manager)
    _manager->remove(this);
  if (_fd >= 0)
    ::close(_fd);
  _fd = -1;
}
