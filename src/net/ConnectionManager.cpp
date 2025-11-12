/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:17:10 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 14:17:46 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionManager.hpp"
#include "Connection.hpp"
#include <unistd.h>

ConnectionManager::ConnectionManager(Reactor *reactor) : m_reactor(reactor) {}

ConnectionManager::~ConnectionManager() { closeAll(); }

void ConnectionManager::add(Connection *conn) {
  if (!conn)
    return;
  m_map[conn->fd()] = conn;
}

void ConnectionManager::remove(Connection *conn) {
  if (!conn)
    return;
  MapType::iterator it = m_map.find(conn->fd());
  if (it != m_map.end())
    m_map.erase(it);
}

void ConnectionManager::closeAll() {
  for (MapType::iterator it = m_map.begin(); it != m_map.end(); ++it) {
    Connection *c = it->second;
    if (c)
      c->close();
  }
  m_map.clear();
}

void ConnectionManager::sweepIdle(std::time_t seconds) {
  std::time_t now = std::time(NULL);
  std::vector<Connection *> toClose;
  for (MapType::iterator it = m_map.begin(); it != m_map.end(); ++it) {
    Connection *c = it->second;
    if (!c)
      continue;
    if ((now - c->lastActivity()) >= seconds)
      toClose.push_back(c);
  }
  for (size_t i = 0; i < toClose.size(); ++i) {
    Connection *c = toClose[i];
    c->close();
    m_map.erase(c->fd());
  }
}

size_t ConnectionManager::size() const { return m_map.size(); }
