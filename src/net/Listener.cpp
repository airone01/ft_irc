/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:18:31 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 15:37:20 by elagouch         ###   ########.fr       */
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
    : _socket(), _addr(addr), _port(port), _reactor(reactor), _connManager(cm),
      _defaultMsgCb(NULL), _factory(NULL) {}

Listener::~Listener() { _socket.close(); }

bool Listener::start() {
  if (!_socket.createAndBind(_addr, _port))
    return false;
  if (!_socket.setNonBlocking(true))
    return false;
  if (!_socket.listen())
    return false;
  if (_reactor) {
    if (!_reactor->addFd(_socket.fd(), EPOLLIN, this))
      return false;
  }
  return true;
}

void Listener::handleEvent(uint32_t events) {
  if (!(events & EPOLLIN))
    return;
  // accept loop
  for (;;) {
    int clientFd = _socket.accept();
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
    if (_factory) {
      c = _factory(clientFd, _reactor, _connManager);
    } else {
      c = new Connection(clientFd, _reactor, _connManager);
    }

    if (c == NULL) {
      ::close(clientFd);
      continue;
    }

    // install default message callback if provided
    if (_defaultMsgCb)
      c->setMessageCallback(_defaultMsgCb);

    if (_connManager)
      _connManager->add(c);
    if (_reactor) {
      if (!_reactor->addFd(clientFd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR,
                           c)) {
        // failed to add
        c->close();
        delete c;
      }
    }
  }
}

void Listener::setDefaultMessageCallback(Connection::MessageCallback cb) {
  _defaultMsgCb = cb;
}

void Listener::setConnectionFactory(ConnectionFactory f) { _factory = f; }
