/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:23:02 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 14:33:07 by elagouch         ###   ########.fr       */
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
    : m_fd(fd), m_reactor(reactor), m_manager(mgr), m_readBuf(), m_writeBuf(),
      m_msgCb(NULL), m_closed(false), m_lastActivity(std::time(NULL)) {}

Connection::~Connection() {
  if (m_fd >= 0)
    ::close(m_fd);
}

int Connection::fd() const { return m_fd; }

void Connection::setMessageCallback(MessageCallback cb) { m_msgCb = cb; }

void Connection::touch() { m_lastActivity = std::time(NULL); }

std::time_t Connection::lastActivity() const { return m_lastActivity; }

void Connection::handleEvent(uint32_t events) {
  if (m_closed)
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
  if (m_fd < 0)
    return -1;
  char buf[4096];
  while (1) {
    ssize_t n = ::recv(m_fd, buf, sizeof(buf), 0);
    if (n > 0) {
      m_lastActivity = std::time(NULL);
      m_readBuf.insert(m_readBuf.end(), buf, buf + n);
      // notify dispatcher if present
      if (m_msgCb) {
        m_msgCb(this, m_readBuf);
        m_readBuf.clear();
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
  return static_cast<ssize_t>(m_readBuf.size());
}

ssize_t Connection::handleWrite() {
  if (m_fd < 0)
    return -1;
  while (!m_writeBuf.empty()) {
    ssize_t n = ::send(m_fd, &m_writeBuf[0], m_writeBuf.size(), MSG_NOSIGNAL);
    if (n > 0) {
      m_lastActivity = std::time(NULL);
      // if ((size_t)n >= m_writeBuf.size()) {
      if (static_cast<size_t>(n) >= m_writeBuf.size()) {
        m_writeBuf.clear();
        // remove EPOLLOUT interest
        if (m_reactor)
          m_reactor->modFd(m_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR,
                           this);
        return n;
      } else {
        m_writeBuf.erase(m_writeBuf.begin(), m_writeBuf.begin() + n);
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
  if (m_closed)
    return;
  if (data.empty())
    return;
  bool wasEmpty = m_writeBuf.empty();
  m_writeBuf.insert(m_writeBuf.end(), data.begin(), data.end());
  if (wasEmpty && m_reactor) {
    m_reactor->modFd(
        m_fd, EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR, this);
  }
}

void Connection::close() {
  if (m_closed)
    return;
  m_closed = true;
  if (m_reactor)
    m_reactor->delFd(m_fd);
  if (m_manager)
    m_manager->remove(this);
  if (m_fd >= 0)
    ::close(m_fd);
  m_fd = -1;
}
