/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:21:18 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 15:10:49 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"
#include "Client.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

struct ClientNameMatcher {
  ClientNameMatcher(const std::string &n) : name(n) {}
  bool operator()(const Client &chan) const {
    return chan.getNickname() == name;
  }
  std::string name;
};

Client &ClientManager::getClientFromNickname(std::string &name) {
  const std::vector<Client>::iterator it =
      std::find_if(_clients.begin(), _clients.end(), ClientNameMatcher(name));

  if (it == _clients.end()) { // no result
    throw std::runtime_error("Couldn't find a channel with that name.");
    // TODO: BETTER ERROR HERE
  }
  return (*it);
}
