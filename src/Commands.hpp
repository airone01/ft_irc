/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/28 17:19:28 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "IRCMessage.hpp"

class Commands {
public:
  static void join(IRCMessage const &param, ChannelManager channels,
                   Client user);
  void part(IRCMessage const &param, ChannelManager channels, Client &user);
  void mode(IRCMessage const &tmp, ChannelManager channels, Client &user);
  void topic(IRCMessage const &tmp, ChannelManager channels, Client &user);
  void invite(IRCMessage const &tmp, ClientManager clients,
              ChannelManager channels, Client &user);
  void kick(IRCMessage const &param, ChannelManager channels, Client &admin);
};

#endif //! COMMANDS_HPP
