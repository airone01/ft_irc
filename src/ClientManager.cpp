/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:21:18 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/04 23:32:01 by elagouch         ###   ########.fr       */
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

struct ClientSocketMatcher {
  ClientSocketMatcher(const int &fd) : _socket(fd) {}
  bool operator()(const Client &user) const {
    return user.getSocket() == _socket;
  }
  int _socket;
};

Client &ClientManager::getClientFromUsername(std::string &name) {
  const std::vector<Client>::iterator it =
      std::find_if(_clients.begin(), _clients.end(), ClientNameMatcher(name));

  if (it == _clients.end()) { // no result
    throw std::runtime_error("ERR_NOSUCHNICK");
  }
  return (*it);
}

Client &ClientManager::getClientFromSocket(int &socket) {
  const std::vector<Client>::iterator it = std::find_if(
      _clients.begin(), _clients.end(), ClientSocketMatcher(socket));

  if (it == _clients.end()) { // no result
    throw std::runtime_error("ERR_NOSUCHNICK");
  }
  return (*it);
}

std::vector<Client> &ClientManager::getClients() { return _clients; }
