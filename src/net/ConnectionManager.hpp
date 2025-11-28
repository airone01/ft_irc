/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:13:30 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/19 12:10:19 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include "Reactor.hpp"
#include <ctime>
#include <map>

class Connection;

class ConnectionManager {
public:
  ConnectionManager();
  ConnectionManager(const ConnectionManager &);
  ConnectionManager(Reactor *reactor);
  ~ConnectionManager();

  ConnectionManager &operator=(const ConnectionManager &);

  void add(Connection *conn);
  void remove(Connection *conn);
  void closeAll();
  void sweepIdle(std::time_t seconds);
  size_t size() const;

  typedef std::map<int, Connection *> MapType;

  // Added getter to allow iteration
  const MapType &getMap() const { return _map; }

private:
  MapType _map; // fd -> Connection*
  Reactor *_reactor;
};

#endif // !CONNECTIONMANAGER_HPP
