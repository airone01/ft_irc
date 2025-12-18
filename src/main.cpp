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
#include <sstream>

#include "Channel.hpp"
#include "ChannelManager.hpp"
#include "Client.hpp"
#include "Server.hpp"

// extern "C" void handle_sigint(int) {
//   if (g_reactor)
//     g_reactor->stop();
// }

// Bridge function: C-style callback -> Class method
// static bool bridgeCallback(Connection *conn, const std::vector<char> &data) {
//   if (g_dispatcher) {
//     return g_dispatcher->handleData(conn, data);
//   }
//   return true;
// }

bool doQuit = false;

void handle_sigint(int sig){
  if (sig == SIGINT)
    doQuit = true;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    // TODO: implement password as second arg
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return 1;
  }

  std::istringstream ss(argv[1]);
  unsigned short port; ss >> port;
  if (!ss.eof())
    std::cerr << "error: unvalid port." << std::endl;
  
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  Server serv(port, argv[2]);
	serv.serverRoutine();
  // Set log level (optional)


  // g_connMgr = &connMgr;
  // ChannelManager chanMgr;
  // g_chanMgr = &chanMgr;
  // ClientManager clientMgr;
  // g_clientMgr = &clientMgr;
  // TimerManager timerMgr;

  // Listener listener("0.0.0.0", port, &reactor, &connMgr);
  // listener.setConnectionFactory(&userFactory);

  // Hook up the bridge
  // listener.setDefaultMessageCallback(&bridgeCallback);

  // if (!listener.start()) {
  //   logger::error() << "Failed to start listener on port " << port << std::endl;
  //   return 1;
  // }

  // connMgr.closeAll();

  return 0;
}
