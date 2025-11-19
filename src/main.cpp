/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/19 12:32:16 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <unistd.h>

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "Dispatcher.hpp"
#include "Logger.hpp"
#include "TimerManager.hpp"
#include "net/Connection.hpp"
#include "net/ConnectionManager.hpp"
#include "net/Listener.hpp"
#include "net/Reactor.hpp"

static Reactor *g_reactor = NULL;
static Dispatcher *g_dispatcher = NULL;

extern "C" void handle_sigint(int) {
  if (g_reactor)
    g_reactor->stop();
}

static Connection *userFactory(int fd, Reactor *reactor,
                               ConnectionManager *mgr) {
  return new Client(fd, reactor, mgr);
}

// Bridge function: C-style callback -> Class method
static void bridgeCallback(Connection *conn, const std::vector<char> &data) {
  if (g_dispatcher) {
    g_dispatcher->handleData(conn, data);
  }
}

int main(int argc, char **argv) {
  unsigned short port = 6667;
  if (argc > 1) {
    int p = atoi(argv[1]);
    if (p > 0)
      port = static_cast<unsigned short>(p);
  }

  // Set log level (optional)
  logger::Logger::getInstance().setMinLevel(logger::DEBUG);

  Reactor reactor(128);
  g_reactor = &reactor;

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  ConnectionManager connMgr(&reactor);
  ChannelManager chanMgr;
  TimerManager timerMgr;

  // Create the Dispatcher
  Dispatcher dispatcher(&connMgr, &chanMgr);
  g_dispatcher = &dispatcher;

  Listener listener("0.0.0.0", port, &reactor, &connMgr);
  listener.setConnectionFactory(&userFactory);

  // Hook up the bridge
  listener.setDefaultMessageCallback(&bridgeCallback);

  if (!listener.start()) {
    logger::error() << "Failed to start listener on port " << port << std::endl;
    return 1;
  }

  logger::info() << "ft_irc running on port " << port << std::endl;
  reactor.run();

  logger::info() << "Stopping..." << std::endl;
  connMgr.closeAll();

  return 0;
}
