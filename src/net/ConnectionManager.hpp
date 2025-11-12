/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:13:30 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 16:00:00 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include <ctime>
#include <map>

#include "Reactor.hpp"

class Connection;

class ConnectionManager {
public:
  ConnectionManager(Reactor *reactor);
  ~ConnectionManager();

  void add(Connection *conn);
  void remove(Connection *conn);

  /**
   * @brief Close all connections and clear the manager.
   */
  void closeAll();

  /**
   * @brief Sweep and close connections idle for longer than "seconds".
   */
  void sweepIdle(std::time_t seconds);

  /**
   * @brief Return current number of managed connections.
   */
  size_t size() const;

private:
  typedef std::map<int, Connection *> MapType; // fd -> Connection*
  MapType _map;
  Reactor *_reactor;
};

#endif // !CONNECTIONMANAGER_HPP
