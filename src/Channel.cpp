/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/28 17:28:47 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include <iostream>

Channel::Channel(void) {}
Channel::Channel(const Channel &tmp)
    : _users(tmp._users), _admins(tmp._admins), _mode(tmp._mode),
      _kickedUsers(tmp._kickedUsers), _topic(tmp._topic), _pswrd(tmp._pswrd),
      _name(tmp._name), _maxCapacity(tmp._maxCapacity), _modeSet(tmp._modeSet) {
}

Channel &Channel::operator=(const Channel &tmp) {
  if (this != &tmp) {
    _users = tmp._users;
    _admins = tmp._admins;
    _mode = tmp._mode;
    _kickedUsers = tmp._kickedUsers;
    _topic = tmp._topic;
    _pswrd = tmp._pswrd;
    _name = tmp._name;
    _maxCapacity = tmp._maxCapacity;
    _modeSet = tmp._modeSet;
  }
  return *this;
}
Channel::~Channel(void) {}

// Channel::Channel( Client &tmp)
// {
// 	this->_users.insert(std::pair<int, Client*>(tmp.getSocket(), &tmp));
// 	this->_maxCapacity = -1;
// }

// Channel::Channel( Client &tmp, int capacity ) :
// 	_maxCapacity(capacity)
// {
// 	this->_users.insert(std::pair<int, Client*>(tmp.getSocket(), &tmp));
// }

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

Channel::Channel(Client &tmp, std::string name) : _modeSet(false) {
  try {
    validChannelName(name);
    _name = name;
    _users.insert(std::pair<int, Client *>(tmp.getSocket(), &tmp));
    _admins.insert(std::pair<int, Client *>(tmp.getSocket(), &tmp));
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

std::string Channel::getTopic() const { return this->_topic; }

std::string Channel::getPswrd() const { return this->_pswrd; }

std::string Channel::getName() const { return this->_name; }

std::size_t Channel::getCapacity() const { return this->_maxCapacity; }

bool Channel::getModeSet() const { return this->_modeSet; }

std::set<char> Channel::getMode() const { return this->_mode; }

std::set<int> Channel::getKickedUsers() const { return this->_kickedUsers; }

std::map<int, Client *> Channel::getInvitedUsers() {
  return this->_invitedUsers;
}

std::map<int, Client *> Channel::getUsers() { return this->_users; }

void Channel::setTopic(std::string newTopic) { this->_topic = newTopic; }

void Channel::setName(std::string newName) {
  try {
    validChannelName(newName);
    this->_name = newName;
  } catch (const Channel::invalidChannelName::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Channel::setPswrd(std::string newPswrd) { this->_pswrd = newPswrd; }

void Channel::setCapacity(int newCapacity) { this->_maxCapacity = newCapacity; }

void Channel::setModeSet(bool changeMode) { this->_modeSet = changeMode; }

void Channel::setMode(const char c) { this->_mode.insert(c); }

void Channel::setKickedUsers(const int socket) {
  this->_kickedUsers.insert(socket);
}

void Channel::setInvitedUsers(Client &user) {
  _invitedUsers.insert(std::pair<int, Client *>(user.getSocket(), &user));
}

void Channel::newUser(Client &tmp) {
  _users.insert(std::pair<int, Client *>(tmp.getSocket(), &tmp));
}

void Channel::updatePriv(const Client &admin, Client &user) {
  if (_admins.find(admin.getSocket()) == _admins.end())
    throw insufficientPrivilege();
  _admins.insert(std::pair<int, Client *>(user.getSocket(), &user));
}

void Channel::tryJoin(const Client &tmp, std::string pswrd) {
  if (_modeSet) {
    if (_mode.find('i') != _mode.end()) {
      if (_invitedUsers.find(tmp.getSocket()) == _invitedUsers.end())
        throw errorMode("ERR_INVITEONLYCHAN");
    }
    if (_mode.find('l') != _mode.end())
      if ((_users.size() == _maxCapacity))
        throw errorMode("ERR_CHANNELISFULL");
    if (_mode.find('k') != _mode.end()) {
      if (pswrd.empty())
        throw errorMode("ERR_NEEDMOREPARAMS");
      else if ((pswrd != _pswrd))
        throw errorMode("ERR_BADCHANNELKEY");
    }
  }
}

void Channel::tryKick(std::vector<std::string> param, IRCMessage const &tmp,
                      Client &admin) {
  (void)tmp;
  std::vector<std::string>::iterator it = param.begin();
  std::map<int, Client *>::iterator victimIt;

  for (victimIt = _users.begin(); victimIt != _users.end(); victimIt++) {
    if (victimIt == _users.end())
      throw errorKick("ERR_NOTONCHANNEL");
  }
  if ((param[1].empty()) || (++it == param.end())) {
    throw errorKick("ERR_NEEDMOREPARAMS");
  }
  if (_admins.find(admin.getSocket()) == _admins.end()) {
    throw errorKick("ERR_CHANOPRIVSNEEDED");
  }
}

void Channel::leaveChannel(Client const &user) {
  std::map<int, Client *>::iterator it;

  if (_users.find(user.getSocket()) != _users.end())
    _users.erase(it);
  else
    throw errorPart("ERR_NOTONCHANNEL");
  if (_admins.find(user.getSocket()) != _admins.end())
    _admins.erase(it);
}

/*
        -ERR_NEEDMOREPARAMS              -ERR_NOSUCHNICK
        -ERR_NOTONCHANNEL                -ERR_USERONCHANNEL
        -ERR_CHANOPRIVSNEEDED
        RPL_INVITING                    RPL_AWAY
*/

bool val(std::string str) {
  if (str.find(',') != std::string::npos)
    return false;
  return true;
}

void Channel::tryInvite(std::vector<std::string> param, ClientManager clients,
                        int adminSocket, int userSocket) {

  if (param.size() != 2 ||
      (std::find_if(param.begin(), param.end(), val) != param.end())) {
    throw errorInvite("ERR_NEEDMOREPARAMS");
  }
  if (_users.find(adminSocket) == _users.end())
    throw errorInvite("ERR_NOTONCHANNEL");
  if (std::find_if(param.begin(), param.end(), val) != param.end()) {
    // todo: this error does not have any replies equivalent
    throw errorInvite("ERR_TOOMANYPARAMS");
  }
  if (_users.find(userSocket) != _users.end())
    throw errorInvite("ERR_USERONCHANNEL");
  clients.getClientFromSocket(userSocket);
  if (_modeSet && _mode.find('o') != _mode.end() &&
      _admins.find(adminSocket) == _admins.end())
    throw errorInvite("ERR_CHANOPRIVSNEEDED");
  setInvitedUsers(clients.getClientFromSocket(userSocket));
}

void Channel::changeTopic(IRCMessage const &tmp, Client const &user) {
  if (!tmp.getPrefix().empty())
    if (user.getUsername() != tmp.getPrefix())
      throw errorTopic("ERR_USERSDONTMATCH");
  if (_modeSet && _mode.find('o') != _mode.end() &&
      _admins.find(user.getSocket()) == _admins.end())
    throw errorTopic("ERR_CHANOPRIVSNEEDED");
  if (_topic.empty() && tmp.getTrailing().empty())
    throw errorTopic("RPL_NOTOPIC");
  else {
    _topic = tmp.getTrailing();
    throw errorTopic("RPL_TOPIC");
  }
}
/*
flags:
        i(set/unset invite only)
        t(set/unset topic priv to admin)
        k(set/unset password)
        o(give/take admin priv)
        l(set/unset limit size)

replies :
           -ERR_NEEDMOREPARAMS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL
           ERR_USERSDONTMATCH              ERR_UMODEUNKNOWNFLAG

                   RPL_BANLIST                     RPL_ENDOFBANLIST
           RPL_UMODEIS RPL_CHANNELMODEIS
*/

void isValidMode(std::string str, Channel &actual) {
  std::string allMode = "itkol";
  std::string::iterator it = std::find(allMode.begin(), allMode.end(), str[1]);
  if ((str[0] != '+') || (str[0] != '-'))
    throw Channel::errorMode("ERR_UNKNOWNMODE");
  if ((it == allMode.end()) ||
      (actual.getMode().find(str[1]) != actual.getMode().end()))
    throw it != allMode.end() ? Channel::errorMode("ERR_UNKNOWNMODE")
                              : Channel::errorMode("ERR_KEYSET");
}

enum CHNGMODE {
  SINVITE,
  UINVITE,
  STOPIC,
  UTOPIC,
  SPASSWORD,
  UPASSWORD,
  SPRIV,
  UPRIV,
  SLIMIT,
  ULIMIT,
  DEFAULT
};

CHNGMODE applyMode(std::string &tmp) {
  if (tmp == "+i")
    return SINVITE;
  if (tmp == "-i")
    return UINVITE;
  if (tmp == "+t")
    return STOPIC;
  if (tmp == "-t")
    return UTOPIC;
  if (tmp == "+k")
    return SPASSWORD;
  if (tmp == "-k")
    return UPASSWORD;
  if (tmp == "+o")
    return SPRIV;
  if (tmp == "-o")
    return UPRIV;
  if (tmp == "+l")
    return SLIMIT;
  if (tmp == "-l")
    return ULIMIT;
  return DEFAULT;
}

void Channel::updateMode(IRCMessage const &tmp, Client const &user) {
  (void)user;
  if (tmp.getCountParams() < 2)
    throw errorMode("ERR_NEEDMOREPARAMS");
  isValidMode(tmp.getParams()[1], *this);
  std::string param = tmp.getParams()[1];
  if (tmp.getParams()[1][0] == '-')
    _mode.erase(tmp.getParams()[1][1]);
  else {
    switch (applyMode(param)) {
    case SINVITE:
      _mode.insert(param[1]);
      break;
    case UINVITE:
      _mode.erase(param[1]);
      break;
    case STOPIC:
      _mode.insert(param[1]);
      break;
    case UTOPIC:
      _mode.erase(param[1]);
      break;
    case SPASSWORD:
      _mode.insert(param[1]);
      break;
    case UPASSWORD:
      _mode.erase(param[1]);
      break;
    case SPRIV:
      _mode.insert(param[1]);
      break;
    case UPRIV:
      _mode.erase(param[1]);
      break;
    case SLIMIT:
      _mode.insert(param[1]);
      break;
    case ULIMIT:
      _mode.erase(param[1]);
      break;
    default:
      break;
    }
  }
}

const char *Channel::insufficientPrivilege::what() const throw() {
  return "ERR_CHANOPRIVSNEEDED";
}

const char *Channel::invalidChannelName::what() const throw() {
  return "error: Channel name must begin with '&' or '#'.";
}

const char *Channel::errorMode::what() const throw() { return _errMsg.c_str(); }

const char *Channel::errorKick::what() const throw() { return _errMsg.c_str(); }

const char *Channel::errorPart::what() const throw() { return _errMsg.c_str(); }

const char *Channel::errorInvite::what() const throw() {
  return _errMsg.c_str();
}

const char *Channel::errorTopic::what() const throw() {
  return _errMsg.c_str();
}
