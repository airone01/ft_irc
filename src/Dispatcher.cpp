/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:30:39 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/08 15:59:12 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dispatcher.hpp"
#include "Channel.hpp"
#include "Commands.hpp"
#include "IRCMessage.hpp"
#include "Logger.hpp"
#include "ReplyMessage.hpp"
#include <ostream>

Dispatcher::Dispatcher(ClientManager *clients, ChannelManager *channels,
                       const std::string &password)
    : _clients(clients), _channels(channels), _password(password) {}

Dispatcher::~Dispatcher() {}

bool Dispatcher::handleData(Connection *conn, const std::vector<char> &data) {
  if (!conn)
    return true;

  Client *client = dynamic_cast<Client *>(conn);
  if (!client)
    return true;
  std::string chunk(data.begin(), data.end());
  client->appendToBuffer(chunk);
  
  while (true) {
    std::string line = client->extractMessage();
    if (line.empty())
    break;
  
  logger::debug() << "Processing: " << line << std::endl;
  
  if (executeCommand(*client, line)) { // if cleanup is needed
    return false;
  }
}
  return true;
}

/**
 * Return true if client was closed/deleted
 */
bool Dispatcher::executeCommand(Client &client, const std::string &line) {
  try {
    IRCMessage msg(line);
    std::string cmd = msg.getCommand();

    if (cmd.empty())
      return false;

    bool isHandshakeCmd = (cmd == "CAP" || cmd == "PASS" || cmd == "NICK" ||
                           cmd == "USER" || cmd == "QUIT");
    if (!client.getRegistered() && !isHandshakeCmd) {
      client.send(ReplyMessage::errNotRegistered());
      return false;
    }
    if (cmd == "PASS") {
      Commands::pass(msg, client, _password);
    } else if (cmd == "NICK") {
      Commands::nick(msg, *_clients, client);
    } else if (cmd == "USER") {
      Commands::user(msg, client);
    } else if (cmd == "QUIT") {
      client.close();
      return true; // client is dead
    } else if (cmd == "CAP") {
      Commands::cap(msg, client);
    } else if (cmd == "VERSION") {
      Commands::version(client);
    } else if (cmd == "JOIN") {
      if (client.getRegistered())
        Commands::join(msg, *_channels, client);
    } else if (cmd == "PRIVMSG") {
      if (client.getRegistered()) {
        Commands::privmsg(msg, client, *_clients, *_channels);
      }
    } else if (cmd == "KICK") {
      Commands::kick(msg, *_channels, client);
    } else if (cmd == "INVITE") {
      Commands::invite(msg, *_clients, *_channels, client);
    } else if (cmd == "MODE") {
      Commands::mode(msg, *_channels, client);
    } else if (cmd == "PING") {
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

  return false;
}
