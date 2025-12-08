/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/12/08 11:48:05 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Logger.hpp"
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
  // RFC 1459/2812 specifies max length of 200 characters for channel names.
  if (tmp.size() > 200) {
    throw Channel::invalidChannelName();
  }

  // Must start with '#' or '&'.
  if (tmp.empty() || (tmp[0] != '#' && tmp[0] != '&')) {
    throw Channel::invalidChannelName();
  }

  // Disallowed characters: space, comma, BELL (ASCII 7).
  if (tmp.find(' ') != std::string::npos ||
      tmp.find(',') != std::string::npos ||
      tmp.find(7) != std::string::npos) { // ASCII 7 is the BELL character
    throw Channel::invalidChannelName();
  }
}

Channel::Channel(Client &tmp, std::string name) : _modeSet(false) {
  validChannelName(name);

  _name = name;
  _users.insert(std::pair<int, Client *>(tmp.getSocket(), &tmp));
  _admins.insert(std::pair<int, Client *>(tmp.getSocket(), &tmp));
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
  std::map<int, Client *>::iterator it = _users.find(user.getSocket());

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
  std::vector<std::string> params = tmp.getParams();

  // If only channel name is given (MODE #channel), it's a query.
  // We simply return, allowing Dispatcher to handle it without error.
  if (params.size() < 2)
    return;

  std::string modeString = params[1];
  bool adding = true;
  size_t argIndex = 2; // Arguments (key, limit, user) start after mode string

  for (size_t i = 0; i < modeString.size(); ++i) {
    char c = modeString[i];

    if (c == '+') {
      adding = true;
      continue;
    }
    if (c == '-') {
      adding = false;
      continue;
    }

    // Supported modes: i, t, k, l, o
    if (c == 'i' || c == 't') {
      if (adding)
        _mode.insert(c);
      else
        _mode.erase(c);
      _modeSet = true;
    } else if (c == 'k') {
      if (adding) {
        if (argIndex < params.size()) {
          _pswrd = params[argIndex++];
          _mode.insert(c);
        }
        // else: technically error, but ignoring prevents crash
      } else {
        _mode.erase(c);
        _pswrd = "";
      }
      _modeSet = true;
    } else if (c == 'l') {
      if (adding) {
        if (argIndex < params.size()) {
          _maxCapacity = std::atoi(params[argIndex++].c_str());
          _mode.insert(c);
        }
      } else {
        _mode.erase(c);
      }
      _modeSet = true;
    } else if (c == 'o') {
      // Operator mode requires a target nick.
      // We skip it here to avoid complexity in this scope,
      // but we consume the arg to keep parsing valid for subsequent modes.
      if (argIndex < params.size()) {
        argIndex++;
      }
    } else {
      // Unknown mode char
      throw errorMode("ERR_UNKNOWNMODE");
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
