/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:30:39 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/04 15:39:48 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dispatcher.hpp"
#include "Channel.hpp"
#include "Commands.hpp"
#include "IRCMessage.hpp"
#include "Logger.hpp"

Dispatcher::Dispatcher(ClientManager *clients, ChannelManager *channels)
    : _clients(clients), _channels(channels) {}

Dispatcher::~Dispatcher() {}

void Dispatcher::handleData(Connection *conn, const std::vector<char> &data) {
  if (!conn)
    return;

  // 1. Safe cast to Client
  Client *client = dynamic_cast<Client *>(conn);
  if (!client)
    return;

  // 2. Buffer the data
  std::string chunk(data.begin(), data.end());
  client->appendToBuffer(chunk);

  // 3. Process complete lines
  while (true) {
    std::string line = client->extractMessage();
    if (line.empty())
      break;

    logger::debug() << "Processing: " << line << std::endl;
    executeCommand(*client, line);
  }
}

void Dispatcher::executeCommand(Client &client, const std::string &line) {
  try {
    IRCMessage msg(
        const_cast<std::string &>(line)); // Parser modifies string temporarily?
    std::string cmd = msg.getCommand();

    // if (cmd == "CAP") {
    //   Commands::cap(msg, client);
    // } else if (cmd == "NICK") {
    //   Commands::nick(msg, *_clients, client);
    // } else if (cmd == "USER") {
    //   Commands::user(msg, *_clients, client);
    // } else if (cmd == "JOIN") {
    if (cmd == "VERSION") {
      Commands::version(client);
    }
    if (cmd == "JOIN") {
      if (client.getRegistered())
        Commands::join(msg, *_channels, client);
    } else if (cmd == "PRIVMSG") {
      if (client.getRegistered()) {
        Commands::privmsg(msg, client, *_clients, *_channels);
      }
    } else if (cmd == "PING") {
      // Simple PONG response
      std::string token = msg.getParams().empty() ? "" : msg.getParams()[0];
      std::string pong = "PONG " + token + "\r\n";
      std::vector<char> r(pong.begin(), pong.end());
      client.send(r);
    } else {
      logger::warning() << "Unknown command: " << cmd << std::endl;
    }

  } catch (std::exception &e) {
    logger::warning() << "Parser/Exec error: " << e.what() << std::endl;
  }
}
