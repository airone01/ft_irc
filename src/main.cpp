/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/08 14:43:24 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "Channel.hpp"
#include "ChannelManager.hpp"
#include "Client.hpp"
#include "Dispatcher.hpp"
#include "Logger.hpp"
#include "net/Connection.hpp"
#include "net/ConnectionManager.hpp"
#include "net/Listener.hpp"
#include "net/Reactor.hpp"

static Reactor *g_reactor = NULL;
static Dispatcher *g_dispatcher = NULL;
static ConnectionManager *g_connMgr = NULL;
static ChannelManager *g_chanMgr = NULL;
static ClientManager *g_clientMgr = NULL;

extern "C" void handle_sigint(int) {
  if (g_reactor)
    g_reactor->stop();
}

static Connection *userFactory(int fd, Reactor *reactor,
                               ConnectionManager *mgr) {
  return new Client(fd, reactor, mgr);
}

// Bridge function: C-style callback -> Class method
static bool bridgeCallback(Connection *conn, const std::vector<char> &data) {
  if (g_dispatcher) {
    return g_dispatcher->handleData(conn, data);
  }
  return true;
}

int main(int argc, char **argv) {
  unsigned short port;

  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return 1;
  }

  int p = atoi(argv[1]);
  if (p > 0)
    port = static_cast<unsigned short>(p);
  std::string password = argv[2];

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
  g_connMgr = &connMgr;
  ChannelManager chanMgr;
  g_chanMgr = &chanMgr;
  ClientManager clientMgr;
  g_clientMgr = &clientMgr;
  // TimerManager timerMgr;

  // Create the Dispatcher
  Dispatcher dispatcher(&clientMgr, &chanMgr, password);
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
