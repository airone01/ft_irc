/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:17:10 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 15:37:47 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionManager.hpp"
#include "Connection.hpp"
#include <unistd.h>

ConnectionManager::ConnectionManager(Reactor *reactor) : _reactor(reactor) {}

ConnectionManager::~ConnectionManager() { closeAll(); }

void ConnectionManager::add(Connection *conn) {
  if (!conn)
    return;
  _map[conn->fd()] = conn;
}

void ConnectionManager::remove(Connection *conn) {
  if (!conn)
    return;
  MapType::iterator it = _map.find(conn->fd());
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
    if ((now - c->lastActivity()) >= seconds)
      toClose.push_back(c);
  }
  for (size_t i = 0; i < toClose.size(); ++i) {
    Connection *c = toClose[i];
    c->close();
    _map.erase(c->fd());
  }
}

size_t ConnectionManager::size() const { return _map.size(); }
