/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:13:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 15:06:17 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "Channel.hpp"

#ifndef CHANNELMANAGER_HPP

// This was meant to be static, but it's too much trouble.
class ChannelManager {
public:
  // ChannelManager();
  // ChannelManager(const ChannelManager &);
  // ~ChannelManager();
  //
  // ChannelManager &operator=(const ChannelManager &);

  std::vector<Channel> getChannels();
  Channel &getChannelFromName(std::string &name);

private:
  std::vector<Channel> _channels;
};

#endif // !CHANNELMANAGER_HPP
