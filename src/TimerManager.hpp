/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:22:28 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 15:40:39 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <map>
#include <vector>

class Connection;

/**
 * @brief Simple timer manager for scheduling connection timeouts.
 *
 * This is a simple, single-threaded timer manager suitable for use in a reactor
 * loop. It stores absolute expiration times and allows polling for expired
 * items.
 */
class TimerManager {
public:
  TimerManager();
  ~TimerManager();

  /**
   * @brief Schedule a connection to expire at absolute time \"when\" (time_t).
   */
  void schedule(Connection *c, std::time_t when);

  /**
   * @brief Convenience: schedule to expire in `secondsFromNow` seconds.
   */
  void scheduleIn(Connection *c, unsigned int secondsFromNow);

  /**
   * @brief Cancel any scheduled timer for the connection.
   */
  void cancel(Connection *c);

  /**
   * @brief Run timers up to now and return list of expired connections.
   *
   * This call removes expired entries from the internal structure.
   */
  std::vector<Connection *> pollExpired();

  /**
   * @brief Remove all timers.
   */
  void clear();

private:
  typedef std::multimap<std::time_t, Connection *> Timers;
  Timers _timers;
};
