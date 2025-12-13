/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:30:39 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/05 03:42:05 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dispatcher.hpp"
#include "Channel.hpp"
#include "Commands.hpp"
#include "IRCMessage.hpp"
#include "Logger.hpp"
#include <ostream>

Dispatcher::Dispatcher(ClientManager *clients, ChannelManager *channels)
    : _clients(clients), _channels(channels) {}

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
