/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dispatcher.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:30:19 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/05 01:08:54 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "net/Connection.hpp"
#include "net/ConnectionManager.hpp"
#include <vector>

class Dispatcher {
public:
  Dispatcher(ClientManager *clients, ChannelManager *channels);
  ~Dispatcher();

  /**
   * @brief Entry point for new data from a connection.
   * Buffers data, extracts lines, parses, and executes commands.
   */
  bool handleData(Connection *conn, const std::vector<char> &data);

private:
  ClientManager *_clients;
  ChannelManager *_channels;

  /**
   * @brief Execute a single parsed command.
   */
  bool executeCommand(Client &client, const std::string &line);
};

#endif //! DISPATCHER_HPP
