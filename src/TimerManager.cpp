/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:23:14 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 16:41:43 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>

#include "TimerManager.hpp"
#include "net/Connection.hpp"

TimerManager::TimerManager() {}

TimerManager::~TimerManager() { m_timers.clear(); }

void TimerManager::schedule(Connection *c, std::time_t when) {
  if (!c)
    return;
  m_timers.insert(std::make_pair(when, c));
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
  for (Timers::iterator it = m_timers.begin(); it != m_timers.end();) {
    if (it->second == c) {
      m_timers.erase(it);
      ++it;
    } else
      ++it;
  }
}

std::vector<Connection *> TimerManager::pollExpired() {
  std::vector<Connection *> out;
  std::time_t now = std::time(NULL);
  for (Timers::iterator it = m_timers.begin();
       it != m_timers.end() && it->first <= now;) {
    out.push_back(it->second);
    m_timers.erase(it);
    ++it;
  }
  return out;
}

void TimerManager::clear() { m_timers.clear(); }
