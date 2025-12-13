/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/08 10:26:35 by elagouch         ###   ########.fr       */
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
#include "net/Listener.hpp"

#include <sstream>

static Reactor *g_reactor = NULL;
static Dispatcher *g_dispatcher = NULL;
static ConnectionManager *g_connMgr = NULL;
static ChannelManager *g_chanMgr = NULL;
static ClientManager *g_clientMgr = NULL;

extern "C" void handle_sigint(int) {
  if (g_reactor)
    g_reactor->stop();
}

// Bridge function: C-style callback -> Class method
static bool bridgeCallback(Connection *conn, const std::vector<char> &data) {
  if (g_dispatcher) {
    return g_dispatcher->handleData(conn, data);
  }
  return true;
}

int main(int argc, char **argv) {

  if (argc < 2 || argc > 3) {
    // TODO: implement password as second arg
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return 1;
  }

  std::istringstream ss(argv[1]);
  unsigned short port; ss >> port;
  if (!ss.eof())
    std::cerr << "error: unvalid port." << std::endl;


  // Set log level (optional)
  logger::Logger::getInstance().setMinLevel(logger::DEBUG);

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  ChannelManager chanMgr;
  g_chanMgr = &chanMgr;
  ClientManager clientMgr;
  g_clientMgr = &clientMgr;
  // TimerManager timerMgr;

  // Create the Dispatcher
  Dispatcher dispatcher(&clientMgr, &chanMgr);
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
