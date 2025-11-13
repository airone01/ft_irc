/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:21:18 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 15:06:23 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"
#include "Channel.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

// ChannelManager::ChannelManager() : _channels() {}
//
// ChannelManager::ChannelManager(const ChannelManager &other)
//     : _channels(other._channels) {}
//
// ChannelManager::~ChannelManager() { /* vector should delete itself */ }
//
// ChannelManager &ChannelManager::operator=(const ChannelManager &other) {
//   if (this != &other) {
//     this->_channels = other._channels;
//   }
//   return (*this);
// }

struct ChannelNameMatcher {
  ChannelNameMatcher(const std::string &n) : name(n) {}
  bool operator()(const Channel &chan) const { return chan.getName() == name; }
  std::string name;
};

Channel &ChannelManager::getChannelFromName(std::string &name) {
  const std::vector<Channel>::iterator it = std::find_if(
      _channels.begin(), _channels.end(), ChannelNameMatcher(name));

  if (it == _channels.end()) { // no result
    throw std::runtime_error("Couldn't find a channel with that name.");
    // TODO: BETTER ERROR HERE
  }
  return (*it);
}
