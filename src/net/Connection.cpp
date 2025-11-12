/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:23:02 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 15:38:41 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "Reactor.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

Connection::Connection(int fd, Reactor *reactor, ConnectionManager *mgr)
    : _fd(fd), _reactor(reactor), _manager(mgr), _readBuf(), _writeBuf(),
      _msgCb(NULL), _closed(false), _lastActivity(std::time(NULL)) {}

Connection::~Connection() {
  if (_fd >= 0)
    ::close(_fd);
}

int Connection::fd() const { return _fd; }

void Connection::setMessageCallback(MessageCallback cb) { _msgCb = cb; }

void Connection::touch() { _lastActivity = std::time(NULL); }

std::time_t Connection::lastActivity() const { return _lastActivity; }

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
  if (_fd < 0)
    return -1;
  char buf[4096];
  while (1) {
    ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);
    if (n > 0) {
      _lastActivity = std::time(NULL);
      _readBuf.insert(_readBuf.end(), buf, buf + n);
      // notify dispatcher if present
      if (_msgCb) {
        _msgCb(this, _readBuf);
        _readBuf.clear();
      }
    } else if (n == 0) {
      // orderly shutdown by peer
      close();
      return 0;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      // error
      perror("recv");
      close();
      return -1;
    }
  }
  return static_cast<ssize_t>(_readBuf.size());
}

ssize_t Connection::handleWrite() {
  if (_fd < 0)
    return -1;
  while (!_writeBuf.empty()) {
    ssize_t n = ::send(_fd, &_writeBuf[0], _writeBuf.size(), MSG_NOSIGNAL);
    if (n > 0) {
      _lastActivity = std::time(NULL);
      // if ((size_t)n >= m_writeBuf.size()) {
      if (static_cast<size_t>(n) >= _writeBuf.size()) {
        _writeBuf.clear();
        // remove EPOLLOUT interest
        if (_reactor)
          _reactor->modFd(m_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR,
                           this);
        return n;
      } else {
        _writeBuf.erase(_writeBuf.begin(), _writeBuf.begin() + n);
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // cannot write now
        return 0;
      }
      perror("send");
      close();
      return -1;
    }
  }
  return 0;
}

void Connection::send(const std::vector<char> &data) {
  if (_closed)
    return;
  if (data.empty())
    return;
  bool wasEmpty = _writeBuf.empty();
  _writeBuf.insert(_writeBuf.end(), data.begin(), data.end());
  if (wasEmpty && _reactor) {
    _reactor->modFd(
        m_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR, this);
  }
}

void Connection::close() {
  if (_closed)
    return;
  _closed = true;
  if (_reactor)
    _reactor->delFd(m_fd);
  if (_manager)
    _manager->remove(this);
  if (_fd >= 0)
    ::close(_fd);
  _fd = -1;
}
