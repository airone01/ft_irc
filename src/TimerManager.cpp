/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:23:14 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 11:49:00 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>

#include "TimerManager.hpp"
#include "net/Connection.hpp"

TimerManager::TimerManager() : _timers() {}

TimerManager::TimerManager(const TimerManager &other)
    : _timers(other._timers) {}

TimerManager::~TimerManager() { _timers.clear(); }

TimerManager &TimerManager::operator=(const TimerManager &other) {
  if (this != &other) {
    this->_timers = other._timers;
  }
  return (*this);
}

void TimerManager::schedule(Connection *c, std::time_t when) {
  if (!c)
    return;
  _timers.insert(std::make_pair(when, c));
}

void TimerManager::scheduleIn(Connection *c, unsigned int secondsFromNow) {
  if (!c)
    return;
  std::time_t when = std::time(NULL) + static_cast<std::time_t>(secondsFromNow);
  schedule(c, when);
}

void TimerManager::cancel(Connection *c) {
  if (!c)
    return;
  for (Timers::iterator it = _timers.begin(); it != _timers.end();) {
    if (it->second == c) {
      _timers.erase(it);
      ++it;
    } else
      ++it;
  }
}

std::vector<Connection *> TimerManager::pollExpired() {
  std::vector<Connection *> out;
  std::time_t now = std::time(NULL);
  for (Timers::iterator it = _timers.begin();
       it != _timers.end() && it->first <= now;) {
    out.push_back(it->second);
    _timers.erase(it);
    ++it;
  }
  return out;
}

void TimerManager::clear() { _timers.clear(); }
