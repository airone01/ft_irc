/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:21:18 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/19 12:09:01 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelManager.hpp"
#include "Channel.hpp"
#include <stdexcept>

ChannelManager::ChannelManager() : _channels() {}

ChannelManager::~ChannelManager() {
  for (std::map<std::string, Channel *>::iterator it = _channels.begin();
       it != _channels.end(); ++it) {
    delete it->second;
  }
  _channels.clear();
}

Channel *ChannelManager::getChannel(const std::string &name) {
  std::map<std::string, Channel *>::iterator it = _channels.find(name);
  if (it != _channels.end()) {
    return it->second;
  }
  return NULL;
}

Channel *ChannelManager::createChannel(const std::string &name, Client &admin) {
  if (getChannel(name) != NULL) {
    throw std::runtime_error("Channel already exists");
  }

  // Note: Channel constructor might throw invalidChannelName
  Channel *newChan = new Channel(admin);
  try {
    newChan->setName(name);
  } catch (std::exception &e) {
    delete newChan;
    throw;
  }

  _channels[name] = newChan;
  return newChan;
}
