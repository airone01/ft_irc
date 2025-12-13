/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/12/08 12:57:43 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "IRCMessage.hpp"

class Commands {
public:
  static void join(IRCMessage const &param, ChannelManager &channels,
                   Client &user);
  static void part(IRCMessage const &param, ChannelManager &channels,
                   Client &user);
  static void mode(IRCMessage const &tmp, ChannelManager &channels,
                   Client &user, ClientManager &clients);
  static void topic(IRCMessage const &tmp, ChannelManager &channels,
                    Client &user);
  static void invite(IRCMessage const &tmp, ClientManager &clients,
                     ChannelManager &channels, Client &user);
  static void kick(IRCMessage const &param, ChannelManager &channels,
                   Client &admin);
  static void privmsg(IRCMessage const &msg, Client &sender,
                      ClientManager &clients, ChannelManager &channels);
  static void version(Client &sender);
  static void pass(IRCMessage const &msg, Client &client);
  static void nick(IRCMessage const &msg, ClientManager &clients,
                   Client &client);
  static void user(IRCMessage const &msg, Client &client);
  static void cap(IRCMessage const &msg, Client &client);
  static void oper(IRCMessage const &msg, Client &client);
  static void names(IRCMessage const &msg, ChannelManager &channels,
                    Client &client);
  static void who(IRCMessage const &msg, ChannelManager &channels,
                  ClientManager &clients, Client &client);
  static void kill(IRCMessage const &msg, ClientManager &clients,
                   Client &admin);
};

#endif //! COMMANDS_HPP
