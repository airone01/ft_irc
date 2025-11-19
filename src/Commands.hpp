/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/19 13:39:54 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "IRCMessage.hpp"
#include "net/ConnectionManager.hpp"

class Commands {
public:
  // Auth
  static void cap(IRCMessage const &msg, Client &user);
  static void nick(IRCMessage const &msg, ConnectionManager &conns,
                   Client &user);
  static void user(IRCMessage const &msg, ConnectionManager &conns,
                   Client &user);

  // Core
  static void join(IRCMessage const &msg, ChannelManager &channels,
                   Client &user);
  static void privmsg(IRCMessage const &msg, ChannelManager &channels,
                      ConnectionManager &conns, Client &user);

  // Stubs for later
  static void part(IRCMessage const &msg);
  static void mode(IRCMessage const &msg);
  static void topic(IRCMessage const &msg);
  static void invite(IRCMessage const &msg);
  static void kick(IRCMessage const &msg, Client &admin);

private:
  // Helpers
  static void sendWelcome(Client &user);
  static Client *findClientByNick(ConnectionManager &conns,
                                  const std::string &nick);
};

#endif //!COMMANDS_HPP
