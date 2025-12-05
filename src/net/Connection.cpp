/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:23:02 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/05 04:10:22 by elagouch         ###   ########.fr       */
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
#include <sys/time.h>
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
  // this should be safe even if closed previously
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
    close();
    return;
  }

  if (events & EPOLLIN) {
    // If handleRead returns <= 0, it means the connection is closed or broken.
    // We must return immediately because 'this' might be deleted.
    if (handleRead() <= 0)
      return;
  }

  // Only check EPOLLOUT if we are still alive (implicit check via return above)
  if (events & EPOLLOUT) {
    handleWrite();
  }
}

ssize_t Connection::handleRead() {
  char buf[4096];
  bool keepReading = true;
  ssize_t totalRead = 0;
  int loopCount = 0;

  if (_fd < 0)
    return -1;

  while (keepReading) {
    loopCount++;
    ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);

    if (n > 0) {
      _lastActivity = std::time(NULL);
      _readBuf.insert(_readBuf.end(), buf, buf + n);
      totalRead += n;

      if (_msgCb) {
        bool alive = _msgCb(this, _readBuf);
        if (!alive) {
          return -1; // signals handleEvent to stop
        }
        _readBuf.clear();
      }

      if (static_cast<size_t>(n) < sizeof(buf)) {
        keepReading = false;
      }

      if (loopCount > 50) {
        keepReading = false;
      }

    } else if (n == 0) {
      close();
      return 0; // Signals handleEvent to stop
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        keepReading = false;
      } else {
        perror("recv");
        close();
        return -1; // Signals handleEvent to stop
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
        if (_reactor)
          _reactor->modFd(_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR,
                          this);
        return n;
      } else {
        _writeBuf.erase(_writeBuf.begin(), _writeBuf.begin() + n);
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
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
        sent = 0;
      } else {
        perror("send");
        close();
        return;
      }
    } else {
      _lastActivity = std::time(NULL);
    }
  }

  if (static_cast<size_t>(sent) < data.size()) {
    _writeBuf.insert(_writeBuf.end(), data.begin() + sent, data.end());

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
  // release system resources first
  if (_reactor)
    _reactor->delFd(_fd);
  if (_fd >= 0)
    ::close(_fd);
  _fd = -1;
  // remove from manager last
  // this triggers 'delete this', so we must not touch any member variables
  // after this line.
  if (_manager)
    _manager->remove(this);
}
