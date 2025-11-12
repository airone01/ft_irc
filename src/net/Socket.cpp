/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:22:32 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 15:36:34 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "Socket.hpp"

Socket::Socket() : _fd(-1) {}

Socket::Socket(int fd) : _fd(fd) {}

Socket::~Socket() {
  if (_fd >= 0) {
    ::close(_fd);
    _fd = -1;
  }
}

bool Socket::createAndBind(const std::string &addr, unsigned short port) {
  if (_fd >= 0) {
    ::close(_fd);
    _fd = -1;
  }

  _fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0)
    return false;

  setReuseAddr(true);

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  if (addr.empty())
    sin.sin_addr.s_addr = INADDR_ANY;
  else
    inet_aton(addr.c_str(), &sin.sin_addr);

  // if (::bind(m_fd, (struct sockaddr *)&sin, sizeof(sin)) != 0) {
  if (::bind(_fd, reinterpret_cast<struct sockaddr *>(&sin), sizeof(sin)) !=
      0) {
    ::close(_fd);
    _fd = -1;
    return false;
  }
  return true;
}

bool Socket::listen(int backlog) {
  if (_fd < 0)
    return false;
  if (::listen(_fd, backlog) != 0)
    return false;
  return true;
}

int Socket::accept() {
  if (_fd < 0)
    return -1;
  int s = ::accept(_fd, NULL, NULL);
  if (s < 0)
    return -1;
  // caller should set non-blocking
  return s;
}

void Socket::close() {
  if (_fd >= 0) {
    ::close(_fd);
    _fd = -1;
  }
}

int Socket::fd() const { return _fd; }

bool Socket::setNonBlocking(bool nonBlocking) {
  if (_fd < 0)
    return false;
  int flags = ::fcntl(_fd, F_GETFL, 0);
  if (flags < 0)
    return false;
  if (nonBlocking)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;
  if (::fcntl(_fd, F_SETFL, flags) != 0)
    return false;
  return true;
}

bool Socket::setReuseAddr(bool on) {
  if (_fd < 0)
    return false;
  int val = on ? 1 : 0;
  if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) != 0)
    return false;
  return true;
}

bool Socket::setNoDelay(bool on) {
  if (_fd < 0)
    return false;
  int val = on ? 1 : 0;
  if (::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) != 0)
    return false;
  return true;
}

void Socket::attach(int fd) {
  if (_fd >= 0)
    ::close(_fd);
  _fd = fd;
}

int Socket::detach() {
  int tmp = _fd;
  _fd = -1;
  return tmp;
}

bool Socket::valid() const { return _fd >= 0; }
