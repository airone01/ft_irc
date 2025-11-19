/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/19 12:25:10 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <unistd.h>

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "Commands.hpp"
#include "IRCMessage.hpp"
#include "Logger.hpp"
#include "TimerManager.hpp"
#include "net/Connection.hpp"
#include "net/ConnectionManager.hpp"
#include "net/Listener.hpp"
#include "net/Reactor.hpp"

// Globals for managers to be accessible in the callback
static ChannelManager *g_channelMgr = NULL;
static ConnectionManager *g_connMgr = NULL;
static Reactor *g_reactor = NULL;

extern "C" void handle_sigint(int) {
  if (g_reactor)
    g_reactor->stop();
}

static Connection *userFactory(int fd, Reactor *reactor,
                               ConnectionManager *mgr) {
  return new Client(fd, reactor, mgr);
}

// The Real Dispatcher
static void ircMessageCallback(Connection *conn,
                               const std::vector<char> &data) {
  if (!conn)
    return;
  Client *user = dynamic_cast<Client *>(conn);
  if (!user)
    return;

  // 1. Append raw bytes to client's internal buffer
  std::string chunk(data.begin(), data.end());
  user->appendToBuffer(chunk);

  // 2. Process all complete lines in buffer
  while (true) {
    std::string line = user->extractMessage();
    if (line.empty())
      break; // No more complete lines

    logger::debug() << "Processing: " << line << std::endl;

    try {
      // 3. Parse
      IRCMessage msg(line);
      std::string cmd = msg.getCommand();

      // 4. Dispatch
      if (cmd == "NICK") {
        Commands::nick(msg, *g_connMgr, *user);
      } else if (cmd == "USER") {
        Commands::user(msg, *g_connMgr, *user);
      } else if (cmd == "JOIN") {
        if (user->getRegistered())
          Commands::join(msg, *g_channelMgr, *user);
      } else if (cmd == "PRIVMSG") {
        if (user->getRegistered())
          Commands::privmsg(msg, *g_channelMgr, *g_connMgr, *user);
      } else if (cmd == "PING") {
        std::string pong = "PONG " +
                           (msg.getParams().empty() ? "" : msg.getParams()[0]) +
                           "\r\n";
        std::vector<char> r(pong.begin(), pong.end());
        user->send(r);
      } else {
        logger::debug() << "Unknown command: " << cmd << std::endl;
      }

    } catch (std::exception &e) {
      logger::warning() << "Failed to parse/exec: " << e.what() << std::endl;
    }
  }
}

int main(int argc, char **argv) {
  logger::Logger::getInstance().setMinLevel(logger::DEBUG);
  unsigned short port = 6667;
  if (argc > 1) {
    int p = atoi(argv[1]);
    if (p > 0)
      port = static_cast<unsigned short>(p);
  }

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
  g_channelMgr = &chanMgr;

  TimerManager timerMgr;

  Listener listener("0.0.0.0", port, &reactor, &connMgr);
  listener.setConnectionFactory(&userFactory);
  listener.setDefaultMessageCallback(&ircMessageCallback);

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
