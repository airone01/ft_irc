/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reactor.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:12:58 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/12 16:01:41 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REACTOR_HPP
#define REACTOR_HPP

#include "EventHandler.hpp"
#include <stdint.h>
#include <sys/epoll.h>
#include <vector>

/**
 * @brief Reactor encapsulates epoll and dispatches events to EventHandler
 * objects.
 */
class Reactor {
public:
  /**
   * @param int maxEvents Maximum number of events to fetch per epoll_wait.
   */
  Reactor(int maxEvents = 64);
  ~Reactor();

  /**
   * @brief Start the event loop. This call blocks until stop() is called.
   */
  void run();

  /**
   * @brief Stop the event loop. Safe to call from another thread.
   */
  void stop();

  /**
   * @brief Add a file descriptor to epoll and associate it with handler.
   */
  bool addFd(int fd, uint32_t events, EventHandler *handler);

  /**
   * @brief Modify events for a registered fd.
   */
  bool modFd(int fd, uint32_t events, EventHandler *handler);

  /**
   * @brief Remove fd from epoll.
   */
  bool delFd(int fd);

private:
  int _epollFd;
  int _eventFd; // used to wake up the loop
  bool _running;
  std::vector<struct epoll_event> _events;
};

#endif // !REACTOR_HPP
