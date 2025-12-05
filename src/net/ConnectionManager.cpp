/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:17:10 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/05 03:44:07 by elagouch         ###   ########.fr       */
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
  if (it != _map.end()) {
    _map.erase(it);
    delete conn; // this is essential to avoid leaks.
  }
}

void ConnectionManager::closeAll() {
  MapType::iterator it = _map.begin();

  // use a while loop with map::erase to safely iterate while deleting elements.
  while (it != _map.end()) {
    Connection *c = it->second;
    // advance iterator and erase map entry first.
    // we cannot rely on c->close() calling remove(c) as it breaks iteration.
    MapType::iterator next = it;
    ++next;
    _map.erase(it);
    if (c) {
      // temporarily nullify the manager pointer so c->close() doesn't try
      // to call remove(c) again, which would crash on an empty map entry.
      // since Connection members are protected, we must perform the cleanup
      // manually: delete the allocated object. connection destructor handles
      // closing the FD.
      delete c;
    }
    it = next;
  }
}

void ConnectionManager::sweepIdle(std::time_t seconds) {
  std::time_t now = std::time(NULL);
  // we cannot delete while iterating here easily in C++98 without careful
  // steps, so we collect first.
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
    c->close(); // close() calls remove(), which deletes.
  }
}

size_t ConnectionManager::size() const { return _map.size(); }
