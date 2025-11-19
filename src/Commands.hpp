/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/19 11:18:52 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "IRCMessage.hpp"

class Commands {
  virtual void join(IRCMessage const &param, ChannelManager channels,
                    Client user);
  virtual void part(IRCMessage const &param);
  virtual void mode(IRCMessage const &param);
  virtual void topic(IRCMessage const &param);
  virtual void invite(IRCMessage const &param);
  virtual void kick(IRCMessage const &param, Client admin);
};

#endif
