/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:18:31 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 11:45:13 by elagouch         ###   ########.fr       */
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
#include <stdexcept>

Listener::Listener()
    : _socket(), _addr(), _port(),
      _defaultMsgCb(NULL) {}

Listener::Listener(const Listener &other)
    : _socket(other._socket), _addr(other._addr), _port(other._port),
      _defaultMsgCb(other._defaultMsgCb) {}

Listener::Listener(const std::string &addr, unsigned short port,
                   Reactor *reactor, ConnectionManager *cm)
    : _socket(), _addr(addr), _port(port),
      _defaultMsgCb(NULL) {}

Listener::~Listener() { _socket.close(); }

Listener &Listener::operator=(const Listener &other) {
  if (this != &other) {
    this->_socket = other._socket;
    this->_addr = other._addr;
    this->_port = other._port;
    this->_defaultMsgCb = other._defaultMsgCb;
  }
  return (*this);
}

void Listener::start() {
  if (!_socket.createAndBind(_addr, _port))
    throw std::runtime_error("createAndBind failed.");
  if (!_socket.setNonBlocking(true))
    throw std::runtime_error("setNonBlocking failed.");
  if (!_socket.listen())
    throw std::runtime_error("listen failed.");
}

void Listener::setDefaultMessageCallback(Connection::MessageCallback cb) {
  _defaultMsgCb = cb;
}
