/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/19 13:40:17 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include <iostream>
#include <sstream>

// Helper to split string by delimiter
static std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

// Helper to find a client by nickname in the ConnectionManager
Client *Commands::findClientByNick(ConnectionManager &conns,
                                   const std::string &nick) {
  const std::map<int, Connection *> &map = conns.getMap();
  for (std::map<int, Connection *>::const_iterator it = map.begin();
       it != map.end(); ++it) {
    Client *c = dynamic_cast<Client *>(it->second);
    if (c && c->getNickname() == nick) {
      return c;
    }
  }
  return NULL;
}

void Commands::cap(IRCMessage const &msg, Client &user) {
  if (msg.getParams().empty())
    return;

  std::string subcmd = msg.getParams()[0];
  std::string target = user.getRegistered() ? user.getNickname() : "*";

  if (subcmd == "LS") {
    // We support NO capabilities. Send empty list.
    std::string reply = ":irc.local CAP " + target + " LS :\r\n";
    std::vector<char> r(reply.begin(), reply.end());
    user.send(r);
  } else if (subcmd == "REQ") {
    // We support nothing, so we reject all requests
    std::string reply =
        ":irc.local CAP " + target + " NAK :" + msg.getTrailing() + "\r\n";
    std::vector<char> r(reply.begin(), reply.end());
    user.send(r);
  }
  // CAP END is ignored (just allows state machine to proceed)
}

void Commands::nick(IRCMessage const &msg, ConnectionManager &conns,
                    Client &user) {
  if (msg.getParams().empty()) {
    // ERR_NONICKNAMEGIVEN
    return;
  }
  std::string newNick = msg.getParams()[0];

  // Check collision
  if (findClientByNick(conns, newNick) != NULL) {
    std::string err =
        ":irc.local 433 * " + newNick + " :Nickname is already in use\r\n";
    std::vector<char> resp(err.begin(), err.end());
    user.send(resp);
    return;
  }

  user.setNickname(newNick);

  // Check registration
  if (!user.getRegistered() && !user.getUsername().empty()) {
    sendWelcome(user);
  }
}

void Commands::user(IRCMessage const &msg, ConnectionManager &conns,
                    Client &user) {
  (void)conns;
  if (user.getRegistered()) {
    // ERR_ALREADYREGISTRED
    return;
  }
  if (msg.getParams().size() < 3) {
    // ERR_NEEDMOREPARAMS
    return;
  }

  user.setUsername(msg.getParams()[0]);
  // Realname is usually in trailing
  // user.setRealname(msg.getTrailing());

  // Check registration
  if (!user.getRegistered() && !user.getNickname().empty()) {
    sendWelcome(user);
  }
}

void Commands::sendWelcome(Client &user) {
  user.setRegistered(true);
  std::string nick = user.getNickname();
  std::string welcome = ":irc.local 001 " + nick +
                        " :Welcome to the Internet Relay Network " + nick +
                        "\r\n";
  std::vector<char> resp(welcome.begin(), welcome.end());
  user.send(resp);
  logger::info() << "Registered new user: " << nick << std::endl;
}

void Commands::join(IRCMessage const &msg, ChannelManager &channels,
                    Client &user) {
  if (msg.getParams().empty())
    return;

  std::vector<std::string> roomNames = split(msg.getParams()[0], ',');
  std::vector<std::string> keys;
  if (msg.getParams().size() > 1) {
    keys = split(msg.getParams()[1], ',');
  }

  for (size_t i = 0; i < roomNames.size(); ++i) {
    std::string name = roomNames[i];
    std::string key = (i < keys.size()) ? keys[i] : "";

    if (name.empty() || (name[0] != '#' && name[0] != '&')) {
      // ERR_NOSUCHCHANNEL or similar, or just ignore invalid
      continue;
    }

    Channel *chan = channels.getChannel(name);
    if (!chan) {
      // Create channel
      try {
        chan = channels.createChannel(name, user);
        logger::info() << user.getNickname() << " created channel " << name
                       << std::endl;
      } catch (std::exception &e) {
        continue;
      }
    } else {
      // Join existing
      try {
        chan->tryJoin(user, key); // Checks password, limit, invite
        chan->newUser(user);
      } catch (std::exception &e) {
        std::string err = ":irc.local 471 " + user.getNickname() + " " + name +
                          " :Cannot join channel (" + e.what() + ")\r\n";
        std::vector<char> resp(err.begin(), err.end());
        user.send(resp);
        continue;
      }
    }

    // Send JOIN confirmation
    std::string joinMsg = ":" + user.getNickname() + " JOIN " + name + "\r\n";
    std::vector<char> resp(joinMsg.begin(), joinMsg.end());

    // Broadcast to EVERYONE in channel (including user)
    const std::map<int, Client *> &members = chan->getUsers();
    for (std::map<int, Client *>::const_iterator it = members.begin();
         it != members.end(); ++it) {
      it->second->send(resp);
    }
  }
}

void Commands::privmsg(IRCMessage const &msg, ChannelManager &channels,
                       ConnectionManager &conns, Client &user) {
  if (msg.getParams().empty()) {
    // ERR_NORECIPIENT
    return;
  }
  if (msg.getTrailing().empty()) {
    // ERR_NOTEXTTOSEND
    return;
  }

  std::string targetName = msg.getParams()[0];
  std::string text = msg.getTrailing();
  std::string fullMsg = ":" + user.getNickname() + " PRIVMSG " + targetName +
                        " :" + text + "\r\n";
  std::vector<char> payload(fullMsg.begin(), fullMsg.end());

  if (targetName[0] == '#' || targetName[0] == '&') {
    // Channel message
    Channel *chan = channels.getChannel(targetName);
    if (!chan) {
      // ERR_NOSUCHCHANNEL
      std::string err = ":irc.local 403 " + user.getNickname() + " " +
                        targetName + " :No such channel\r\n";
      std::vector<char> resp(err.begin(), err.end());
      user.send(resp);
      return;
    }

    // Broadcast to all channel members EXCEPT sender
    const std::map<int, Client *> &members = chan->getUsers();
    for (std::map<int, Client *>::const_iterator it = members.begin();
         it != members.end(); ++it) {
      if (it->second->getFd() != user.getFd()) {
        it->second->send(payload);
      }
    }
  } else {
    // Private message
    Client *target = findClientByNick(conns, targetName);
    if (!target) {
      // ERR_NOSUCHNICK
      std::string err = ":irc.local 401 " + user.getNickname() + " " +
                        targetName + " :No such nick/channel\r\n";
      std::vector<char> resp(err.begin(), err.end());
      user.send(resp);
      return;
    }
    target->send(payload);
  }
}

// Stubs
void Commands::part(IRCMessage const &msg) { (void)msg; }
void Commands::mode(IRCMessage const &msg) { (void)msg; }
void Commands::topic(IRCMessage const &msg) { (void)msg; }
void Commands::invite(IRCMessage const &msg) { (void)msg; }
void Commands::kick(IRCMessage const &msg, Client &admin) {
  (void)msg;
  (void)admin;
}
