/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:13:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/19 12:11:57 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include "Channel.hpp"
#include <map>
#include <string>

class ChannelManager {
public:
  ChannelManager();
  ~ChannelManager();

  /**
   * @brief Get a channel by name. Returns NULL if not found.
   */
  Channel *getChannel(const std::string &name);

  /**
   * @brief Create a new channel with the given name and admin.
   * Throws if channel already exists or name is invalid.
   */
  Channel *createChannel(const std::string &name, Client &admin);

private:
  // Map Name -> Channel*
  std::map<std::string, Channel *> _channels;
};

#endif // !CHANNELMANAGER_HPP
