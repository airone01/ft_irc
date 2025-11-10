/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reactor.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:16:29 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 14:53:15 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "Reactor.hpp"

Reactor::Reactor(int maxEvents)
    : m_epollFd(-1), m_eventFd(-1), m_running(false) {
  m_epollFd = ::epoll_create(1024);
  if (m_epollFd < 0) {
    perror("epoll_create");
    return;
  }
  m_events.resize(maxEvents);

// create eventfd used to wake up epoll_wait
#if defined(__linux__)
  m_eventFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
#else
  m_eventFd = -1;
#endif
  if (m_eventFd >= 0) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = NULL; // Reactor wakeup has no handler
    if (::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_eventFd, &ev) != 0) {
      perror("epoll_ctl add eventfd");
      ::close(m_eventFd);
      m_eventFd = -1;
    }
  }
}

Reactor::~Reactor() {
  if (m_eventFd >= 0)
    ::close(m_eventFd);
  if (m_epollFd >= 0)
    ::close(m_epollFd);
}

void Reactor::run() {
  if (m_epollFd < 0)
    return;
  m_running = true;
  while (m_running) {
    // int n = ::epoll_wait(m_epollFd, &m_events[0], (int)m_events.size(),
    // 1000);
    int n = ::epoll_wait(m_epollFd, &m_events[0],
                         static_cast<int>(m_events.size()), 1000);
    if (n < 0) {
      if (errno == EINTR)
        continue;
      perror("epoll_wait");
      break;
    }
    for (int i = 0; i < n; ++i) {
      struct epoll_event &ev = m_events[i];
      // if this is the eventfd wakeup, we simply read and continue
      if (m_eventFd >= 0 && ev.data.fd == m_eventFd) {
        uint64_t val;
        ssize_t r = ::read(m_eventFd, &val, sizeof(val));
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
  m_running = false;
  if (m_eventFd >= 0) {
    uint64_t v = 1;
    ssize_t r = ::write(m_eventFd, &v, sizeof(v));
    (void)r;
  }
}

bool Reactor::addFd(int fd, uint32_t events, EventHandler *handler) {
  if (m_epollFd < 0)
    return false;
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = events;
  ev.data.ptr = handler;
  if (::epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &ev) != 0) {
    return false;
  }
  return true;
}

bool Reactor::modFd(int fd, uint32_t events, EventHandler *handler) {
  if (m_epollFd < 0)
    return false;
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = events;
  ev.data.ptr = handler;
  if (::epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &ev) != 0) {
    return false;
  }
  return true;
}

bool Reactor::delFd(int fd) {
  if (m_epollFd < 0)
    return false;
  // Some kernels require a non-NULL event pointer for DEL; pass NULL.
  if (::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL) != 0) {
    return false;
  }
  return true;
}
