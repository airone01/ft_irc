/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:13:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/28 17:30:46 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include "Channel.hpp"
#include <vector>

// This was meant to be static, but it's too much trouble.
class ChannelManager {
public:
  // ChannelManager();
  // ChannelManager(const ChannelManager &);
  // ~ChannelManager();
  //
  // ChannelManager &operator=(const ChannelManager &);

  std::vector<Channel> getChannels();
  void addChannels(const Channel &);
  void rmChannels(const Channel &);
  Channel &getChannelFromName(std::string &name);

  class noSuchChannel : public std::exception {
    const char *what() const throw();
  };

private:
  std::vector<Channel> _channels;
};

#endif
