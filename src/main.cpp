/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 11:24:08 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <signal.h>

#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "Client.hpp"
#include "Logger.hpp"
#include "TimerManager.hpp"
#include "net/Connection.hpp"
#include "net/ConnectionManager.hpp"
#include "net/Listener.hpp"
#include "net/Reactor.hpp"

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 6697
#endif

// global reactor pointer so signal handler can stop it.
static Reactor *g_reactor = NULL;

// signal handler for Ctrl-C
extern "C" void handle_sigint(int) {
  if (g_reactor)
    g_reactor->stop();
}

/**
 * Connection factory that creates a User instance (subclass of Connection).
 */
static Connection *userFactory(int fd, Reactor *reactor,
                               ConnectionManager *mgr) {
  return new Client(fd, reactor, mgr);
}

/**
 * Very small demo message callback.
 *
 * It receives a Connection* (may be a Client*) and the bytes read since the
 * last callback. For simplicity this callback treats the bytes as text,
 * splits on '\\n', trims '\\r', and handles a tiny subset of IRC-like commands:
 *
 *  - "NICK <nick>" sets the nick
 *  - "USER <username> <unused> <unused> :<realname>" sets username and realname
 * (very simplified)
 *
 * When both nick and username are set we mark the user registered and send a
 * simple welcome numeric. Otherwise lines are echoed back.
 */
static void demoIrcCallback(Connection *conn, const std::vector<char> &data) {
  if (!conn)
    return;

  Client *user = dynamic_cast<Client *>(conn); // may be NULL if plain Connection
  // copy into a string for easy splitting
  std::string s(data.begin(), data.end());
  size_t start = 0;
  while (true) {
    size_t pos = s.find('\n', start);
    if (pos == std::string::npos)
      break;
    std::string line = s.substr(start, pos - start);
    // trim trailing '\r' if present
    if (!line.empty() && line[line.size() - 1] == '\r')
      line.erase(line.size() - 1);
    logger::debug() << "Received line: " << line << std::endl;
    // for some bizare reason, the endl here is not printed???

    ///////////////////////// bery barebones parse /////////////////////////
    if (line.size() >= 5 && line.substr(0, 5) == "NICK ") {
      if (user) {
        std::string nick = line.substr(5);
        user->setNickname(nick);
        logger::info() << "Set nick to " << nick << " for fd=" << user->getFd()
                       << std::endl;
      }
    } else if (line.size() >= 5 && line.substr(0, 5) == "USER ") {
      if (user) {
        // very crude parsing: USER <username> ... :<realname>
        std::string rest = line.substr(5);
        std::string username;
        std::string realname;
        size_t colon = rest.find(" :");
        if (colon != std::string::npos) {
          username = rest.substr(0, colon);
          realname = rest.substr(colon + 2);
        } else {
          // fallback: first token as username
          size_t sp = rest.find(' ');
          if (sp != std::string::npos)
            username = rest.substr(0, sp);
          else
            username = rest;
        }
        // trim username field's trailing spaces
        while (!username.empty() && username[username.size() - 1] == ' ')
          username.erase(username.size() - 1);
        user->setUsername(username);
        // user->setRealname(realname);
        logger::info() << "Set username to " << username << " realname='"
                       << realname << "'" << std::endl;
      }
    } else {
      // otherwise echo the line back
      std::string out = "Echo: " + line + "\r\n"; // clrf bc irc
      std::vector<char> vb(out.begin(), out.end());
      conn->send(vb);
    }

    // if we have both nick and username, mark registered and send a welcome
    // message
    if (user && !user->getRegistered()) {
      if (!user->getNickname().empty() && !user->getUsername().empty()) {
        user->setRegistered(true);
        std::string welcome = ":" + std::string("irc.example.com") + " 001 " +
                              user->getNickname() +
                              " :Welcome to this minimal IRC demo\r\n";
        std::vector<char> wv(welcome.begin(), welcome.end());
        conn->send(wv);
        logger::info() << "User fd=" << user->getFd() << " registered as "
                       << user->getNickname() << std::endl;
      }
    }

    start = pos + 1;
  }
  ///////////////////////// bery barebones parse end /////////////////////////
}

int main(int argc, char **argv) {
  unsigned short port = 6667;
  if (argc > 1) {
    int p = atoi(argv[1]);
    if (p > 0)
      port = static_cast<unsigned short>(p);
  }

  Reactor reactor(128);
  g_reactor = &reactor;

  // install ctrl-c handler
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  ConnectionManager connMgr(&reactor);
  TimerManager timerMgr;

  Listener listener("0.0.0.0", port, &reactor, &connMgr);

  // set factory so Listener creates Client objects
  listener.setConnectionFactory(&userFactory);

  // set default message callback (our demo IRC-ish handler)
  listener.setDefaultMessageCallback(&demoIrcCallback);

  if (!listener.start()) {
    logger::error() << "Failed to start listener on port " << port << std::endl;
    return 1;
  }

  logger::info() << "IRC demo server listening on port " << port << std::endl;
  logger::info() << "Press Ctrl-C to stop" << std::endl;

  reactor.run();

  logger::info() << "Server stopped, closing connections..." << std::endl;
  connMgr.closeAll();

  return 0;
}
