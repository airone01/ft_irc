/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:17:10 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 11:26:40 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionManager.hpp"
#include "Connection.hpp"
#include <unistd.h>

ConnectionManager::ConnectionManager() : _map(), _reactor(NULL) {}

ConnectionManager::ConnectionManager(const ConnectionManager &other)
    : _map(other._map), _reactor(other._reactor) {}

ConnectionManager::ConnectionManager(Reactor *reactor) : _reactor(reactor) {}

ConnectionManager::~ConnectionManager() { closeAll(); }

ConnectionManager &
ConnectionManager::operator=(const ConnectionManager &other) {
  if (this != &other) {
    this->_map = other._map;
    this->_reactor = other._reactor;
  }
  return (*this);
}

void ConnectionManager::add(Connection *conn) {
  if (!conn)
    return;
  _map[conn->getFd()] = conn;
}

void ConnectionManager::remove(Connection *conn) {
  if (!conn)
    return;
  MapType::iterator it = _map.find(conn->getFd());
  if (it != _map.end())
    _map.erase(it);
}

void ConnectionManager::closeAll() {
  for (MapType::iterator it = _map.begin(); it != _map.end(); ++it) {
    Connection *c = it->second;
    if (c)
      c->close();
  }
  _map.clear();
}

void ConnectionManager::sweepIdle(std::time_t seconds) {
  std::time_t now = std::time(NULL);
  std::vector<Connection *> toClose;
  for (MapType::iterator it = _map.begin(); it != _map.end(); ++it) {
    Connection *c = it->second;
    if (!c)
      continue;
    if ((now - c->getLastActivity()) >= seconds)
      toClose.push_back(c);
  }
  for (size_t i = 0; i < toClose.size(); ++i) {
    Connection *c = toClose[i];
    c->close();
    _map.erase(c->getFd());
  }
}

size_t ConnectionManager::size() const { return _map.size(); }
