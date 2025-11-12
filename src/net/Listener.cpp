/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:18:31 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 16:20:51 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener.hpp"
#include "ConnectionManager.hpp"
#include "Reactor.hpp"
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

Listener::Listener(const std::string &addr, unsigned short port,
                   Reactor *reactor, ConnectionManager *cm)
    : m_socket(), m_addr(addr), m_port(port), m_reactor(reactor),
      m_connManager(cm), m_defaultMsgCb(NULL), m_factory(NULL) {}

Listener::~Listener() { m_socket.close(); }

bool Listener::start() {
  if (!m_socket.createAndBind(m_addr, m_port))
    return false;
  if (!m_socket.setNonBlocking(true))
    return false;
  if (!m_socket.listen())
    return false;
  if (m_reactor) {
    if (!m_reactor->addFd(m_socket.fd(), EPOLLIN, this))
      return false;
  }
  return true;
}

void Listener::handleEvent(uint32_t events) {
  if (!(events & EPOLLIN))
    return;
  // accept loop
  for (;;) {
    int clientFd = m_socket.accept();
    if (clientFd < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      // real error
      perror("accept");
      break;
    }
    // configure client socket: non-blocking, disable Nagle
    int flags = ::fcntl(clientFd, F_GETFL, 0);
    if (flags >= 0)
      ::fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);
    int one = 1;
    ::setsockopt(clientFd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    // create Connection or user-defined subclass via factory
    Connection *c = NULL;
    if (m_factory) {
      c = m_factory(clientFd, m_reactor, m_connManager);
    } else {
      c = new Connection(clientFd, m_reactor, m_connManager);
    }

    if (c == NULL) {
      ::close(clientFd);
      continue;
    }

    // install default message callback if provided
    if (m_defaultMsgCb)
      c->setMessageCallback(m_defaultMsgCb);

    if (m_connManager)
      m_connManager->add(c);
    if (m_reactor) {
      if (!m_reactor->addFd(clientFd,
                            EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR, c)) {
        // failed to add
        c->close();
        delete c;
      }
    }
  }
}

void Listener::setDefaultMessageCallback(Connection::MessageCallback cb) {
  m_defaultMsgCb = cb;
}

void Listener::setConnectionFactory(ConnectionFactory f) { m_factory = f; }
