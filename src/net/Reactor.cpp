/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reactor.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:16:29 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 11:39:55 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>
#include "Reactor.hpp"

Reactor::Reactor() : _epollFd(-1), _eventFd(-1), _running(false), _events() {}

Reactor::Reactor(const Reactor &other)
    : _epollFd(other._epollFd), _eventFd(other._eventFd),
      _running(other._running), _events(other._running) {}

Reactor::Reactor(int maxEvents) : _epollFd(-1), _eventFd(-1), _running(false) {
  _epollFd = ::epoll_create(1024);
  if (_epollFd < 0) {
    perror("epoll_create");
    return;
  }
  _events.resize(maxEvents);

  // create eventfd used to wake up epoll_wait
  _eventFd = epoll_ctl(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (_eventFd >= 0) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = NULL; // Reactor wakeup has no handler
    if (::epoll_ctl(_epollFd, EPOLL_CTL_ADD, _eventFd, &ev) != 0) {
      perror("epoll_ctl add eventfd");
      ::close(_eventFd);
      _eventFd = -1;
    }
  }
}

Reactor::~Reactor() {
  if (_eventFd >= 0)
    ::close(_eventFd);
  if (_epollFd >= 0)
    ::close(_epollFd);
}

Reactor &Reactor::operator=(const Reactor &other) {
  if (this != &other) {
    this->_epollFd = other._epollFd;
    this->_eventFd = other._eventFd;
    this->_running = other._running;
    this->_events = other._events;
  }
  return (*this);
}

void Reactor::run() {
  if (_epollFd < 0)
    return;
  _running = true;
  while (_running) {
    int n = ::epoll_wait(_epollFd, &_events[0],
                         static_cast<int>(_events.size()), 1000);
    if (n < 0) {
      if (errno == EINTR)
        continue;
      perror("epoll_wait");
      break;
    }
    for (int i = 0; i < n; ++i) {
      struct epoll_event &ev = _events[i];
      // if this is the eventfd wakeup, we simply read and continue
      if (_eventFd >= 0 && ev.data.fd == _eventFd) {
        uint64_t val;
        ssize_t r = ::read(_eventFd, &val, sizeof(val));
        (void)r;
        continue;
      }
      EventHandler *h = static_cast<EventHandler *>(ev.data.ptr);
      if (h) {
        uint32_t events = ev.events;
        h->handleEvent(events);
      }
    }
  }
}

void Reactor::stop() {
  _running = false;
  if (_eventFd >= 0) {
    uint64_t v = 1;
    ssize_t r = ::write(_eventFd, &v, sizeof(v));
    (void)r;
  }
}

bool Reactor::addFd(int fd, uint32_t events, EventHandler *handler) {
  if (_epollFd < 0)
    return false;
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = events;
  ev.data.ptr = handler;
  if (::epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) != 0) {
    return false;
  }
  return true;
}

bool Reactor::modFd(int fd, uint32_t events, EventHandler *handler) {
  if (_epollFd < 0)
    return false;
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = events;
  ev.data.ptr = handler;
  if (::epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) != 0) {
    return false;
  }
  return true;
}

bool Reactor::delFd(int fd) {
  if (_epollFd < 0)
    return false;
  // Some kernels require a non-NULL event pointer for DEL; pass NULL.
  if (::epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) != 0) {
    return false;
  }
  return true;
}
