/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/16 10:47:12 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <cstddef>
#include <iostream>
#include <utility>

Channel::Channel(void) {}
Channel::Channel(const Channel &other)
    : _users(other._users), _admins(other._admins),
      _invitedUsers(other._invitedUsers), _mode(other._mode),
      _topic(other._topic), _pswrd(other._pswrd), _name(other._name),
      _maxCapacity(other._maxCapacity), _modeSet(other._modeSet) {}
Channel::~Channel(void) {}

Channel &Channel::operator=(const Channel &other) {
  if (this != &other) {
    this->_users = other._users;
    this->_admins = other._admins;
    this->_invitedUsers = other._invitedUsers;
    this->_mode = other._mode;
    this->_topic = other._topic;
    this->_pswrd = other._pswrd;
    this->_name = other._name;
    this->_maxCapacity = other._maxCapacity;
    this->_modeSet = other._modeSet;
  }
  return *this;
}

Channel::Channel(const Client &tmp) {
  _users.insert(std::make_pair(tmp.getSocket(), tmp));
  _maxCapacity = -1;
}

Channel::Channel(const Client &tmp, int capacity) : _maxCapacity(capacity) {
  _users.insert(std::make_pair(tmp.getSocket(), tmp));
  _maxCapacity = capacity;
}

std::string Channel::getTopic() const { return this->_topic; }

std::string Channel::getPswrd() const { return this->_pswrd; }

int Channel::getCapacity() const { return this->_maxCapacity; }

std::string Channel::getName() const { return this->_name; }

bool Channel::getInvite() const { return this->_modeSet; }

void Channel::setTopic(std::string newTopic) { this->_topic = newTopic; }

void validChannelName(std::string tmp) {
  size_t space = tmp.find(' ');
  size_t comma = tmp.find(',');
  size_t ctrlG = tmp.find(7);
  size_t size = tmp.size();
  if (size > 200 || space != size || comma != size || ctrlG != size)
    throw Channel::invalidChannelName();
  if (tmp[0] == '#' || tmp[0] == '&')
    ;
  else
    throw Channel::invalidChannelName();
}

void Channel::setName(std::string newName) {
  try {
    validChannelName(newName);
    this->_name = newName;
  } catch (const Channel::invalidChannelName &e) {
    std::cerr << &e << std::endl;
  }
}

void Channel::setPswrd(std::string newPswrd) { this->_pswrd = newPswrd; }

void Channel::setCapacity(int newCapacity) { this->_maxCapacity = newCapacity; }

void Channel::setInvite(bool changeMode) { this->_modeSet = changeMode; }

void Channel::newUser(const Client &tmp) {
  _users.insert(std::make_pair(tmp.getSocket(), tmp));
}

void Channel::updatePriv(const Client &admin, Client &user) {
  if (_admins.find(admin.getSocket()) == _admins.end())
    throw insufficientPrivilege();
  _admins.insert(std::make_pair(user.getSocket(), user));
}

bool Channel::tryJoin(const Client &tmp) {
  if (_modeSet) {
    if (_mode.find('i') != _mode.end()) {
      if (_invitedUsers.find(tmp.getSocket()) == _invitedUsers.end())
        throw invitationNeeded();
    }
    if (_mode.find('l') != _mode.end())
      if ((_users.size() == static_cast<size_t>(_maxCapacity)))
        throw maxCapacityReached();
  }
  return true;
}

const char *Channel::maxCapacityReached::what() const throw() {
  return "Max capacity for this channel already reached.";
}

const char *Channel::invitationNeeded::what() const throw() {
  return "You need a invitation to join this channel.";
}

const char *Channel::insufficientPrivilege::what() const throw() {
  return "Unsufficient privilege to do this action.";
}

const char *Channel::invalidChannelName::what() const throw() {
  return "Channel name must begin with '&' or '#'.";
}
