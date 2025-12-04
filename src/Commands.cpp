/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/12/04 23:34:43 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "ChannelManager.hpp"
#include "ClientManager.hpp"
#include "ReplyMessage.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

std::vector<std::string> paramHandler(std::string params) {
  std::vector<std::string> newParam;
  std::string tmp;
  for (size_t i = 0;;) {
    if (i == 0)
      tmp = params.substr(i, params.find(','));
    else
      tmp = params.substr(i);
    newParam.push_back(tmp);
    i = params.find(',', i);
    if (i == std::string::npos)
      break;
    i++;
  }
  return newParam;
}

void Commands::join(IRCMessage const &tmp, ChannelManager channels,
                    Client user) {

  std::vector<std::string> param = tmp.getParams();
  std::vector<std::string>::iterator it = param.begin();
  std::vector<std::string>::iterator roomIt;
  std::vector<std::string>::iterator pswrdIt;

  std::vector<std::string> rooms = tmp.getParams();
  std::vector<std::string> pswrd = tmp.getParams();

  if (std::find(it->begin(), it->end(), ',') != it->end())
    rooms = paramHandler(*it);
  it++;
  if (it != param.end() && std::find(it->begin(), it->end(), ',') != it->end())
    pswrd = paramHandler(*it);

  for (roomIt = rooms.begin(), pswrdIt = pswrd.begin(); roomIt != rooms.end();
       roomIt++) {
    try {
      Channel &actual = channels.getChannelFromName(*roomIt);
      actual.tryJoin(user, *pswrdIt);
      actual.newUser(user);
      // todo: add numeric replies on succes RPL_TOPIC and RPL_NAMREPLY
      pswrdIt++;
    } catch (const ChannelManager::noSuchChannel &e) {
      channels.addChannels(Channel(user, *roomIt));
    } catch (const std::exception &e) {
      // todo: add replies sender for error or valid commands
      std::cerr << e.what() << '\n';
    }
  }
}

void Commands::part(IRCMessage const &tmp, ChannelManager channels,
                    Client &user) {
  std::vector<std::string> param = tmp.getParams();
  std::vector<std::string>::iterator it;
  for (it = param.begin(); it != param.end(); it++) {
    try {
      Channel &actual = channels.getChannelFromName(*it);
      actual.leaveChannel(user);
      if (actual.getUsers().empty()) {
        channels.rmChannels(actual);
        actual.~Channel();
        // todo: reply
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
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
           ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           RPL_BANLIST                     RPL_ENDOFBANLIST
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL
           ERR_USERSDONTMATCH              ERR_UMODEUNKNOWNFLAG
           RPL_UMODEIS
*/
void Commands::mode(IRCMessage const &param, ChannelManager channels,
                    Client &user) {
  std::vector<std::string> tmp = param.getParams();
  try {
    Channel &actual = channels.getChannelFromName(tmp[0]);
    actual.updateMode(param, user);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

void Commands::topic(IRCMessage const &tmp, ChannelManager channels,
                     Client &user) {
  if (tmp.getCountParams() != 1)
    std::cerr << "ERR_NEEDMOREPARAMS\n";
  std::vector<std::string> param = paramHandler(tmp.getParams()[0]);
  std::vector<std::string>::iterator it = param.begin();

  for (; it != param.end(); it++) {
    try {
      Channel &actual = channels.getChannelFromName(*it);
      actual.changeTopic(tmp, user);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
}

void Commands::invite(IRCMessage const &tmp, ClientManager clients,
                      ChannelManager channels, Client &user) {
  std::vector<std::string> param = tmp.getParams();
  try {
    Channel &actual = channels.getChannelFromName(param[1]);
    actual.tryInvite(param, clients, user.getSocket(),
                     clients.getClientFromUsername(param[0]).getSocket());
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Commands::kick(IRCMessage const &tmp, ChannelManager channels,
                    Client &admin) {
  std::vector<std::string> param = tmp.getParams();
  try {
    Channel &actual = channels.getChannelFromName(param[0]);
    actual.tryKick(param, tmp, admin);
    std::map<int, Client *>::iterator victimIt;
    for (victimIt = actual.getUsers().begin();
         victimIt != actual.getUsers().end(); victimIt++)
      if (victimIt->second->getUsername() == param[1])
        actual.setKickedUsers(victimIt->second->getSocket());
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

/**
 *	@Brief This is the parsing of the messages sent.
 *	Format			:	[ ':' <prefix> <SPACE> ] <PRIVMSG>
 * <SPACE> <params1>[ ',' <params2> ] [ <SPACE> <params2> [ ',' <params2>]]
 * <SPACE> ':' <trailing> Numeric Replies	: ERR_NORECIPIENT
 *			ERR_NOTEXTTOSEND ERR_CANNOTSENDTOCHAN
 *	ERR_NOTOPLEVEL ERR_WILDTOPLEVEL 	ERR_TOOMANYTARGETS
 *						ERR_NOSUCHNICK
 *		RPL_AWAY Exemple			:	:Angel PRIVMSG
 * Wiz :Hello are you receiving this message ?; PRIVMSG Angel :yes I'm receiving
 * it !receiving it !'u>(768u+1n) .br; PRIVMSG jtotolsun.oulu.fi :Hello !;
 *						PRIVMSG $*.fi :Server
 * tolsun.oulu.fi rebooting.; Message to everyone on a server which has a name
 * matching *.fi. PRIVMSG #*.edu :NSFNet is undergoing work, expect
 * interruptions; Message to all users who come from a host which has a name
 * matching *.edu.
 */
void Commands::privmsg(IRCMessage const &msg, Client &sender,
                       ClientManager &clients, ChannelManager &channels) {
  if (msg.getCountParams() < 1) {
    std::cerr << "411 ERROR HANDLING :No recipient given " << msg.getCommand()
              << std::endl;
    return;
  }
  if (msg.getTrailing().empty()) {
    std::cerr << "412 ERR_NOTEXTTOSEND :No text to send" << std::endl;
    return;
  }
  std::string target = msg.getParams()[0];
  std::string message = msg.getTrailing();
  std::string formatted = ":" + sender.getNickname() + "!" +
                          sender.getUsername() + "@" + sender.getHostname() +
                          " PRIVMSG " + target + " :" + message + "\r\n";
  std::vector<char> msgVec(formatted.begin(), formatted.end());
  if (!target.empty() && (target[0] == '#' || target[0] == '&')) {
    try {
      Channel &chan = channels.getChannelFromName(target);
      std::map<int, Client *> users = chan.getUsers();
      if (users.find(sender.getSocket()) == users.end()) {
        std::cerr << "404 ERR_CANNOTSENDTOCHAN " << msg.getParams()[0]
                  << " :Cannot send to channel" << std::endl;
        return;
      }
      for (std::map<int, Client *>::iterator it = users.begin();
           it != users.end(); ++it) {
        if (it->second->getSocket() != sender.getSocket()) {
          it->second->send(msgVec);
        }
      }
    } catch (const std::exception &e) {
      std::cerr << "403 ERR_NOSUCHCHANNEL " << msg.getParams()[0]
                << " :No such channel" << std::endl;
    }
  } else {
    try {
      Client &recipient = clients.getClientFromUsername(target);
      recipient.send(msgVec);
    } catch (const std::exception &e) {
      std::cerr << "401 ERR_NOSUCHNICK :No such nick" << std::endl;
    }
  }
}

void Commands::version(Client &sender) {
  std::string ver = "1.0.0";
  std::vector<char> msgVec(ver.begin(), ver.end());

  sender.send(msgVec);
}

void checkRegistration(Client &client) {
  if (client.getRegistered())
    return; // already registered

  // we need at least a Nickname and a Username to register
  if (!client.getNickname().empty() && !client.getUsername().empty()) {
    client.setRegistered(true);

    // send RPL_WELCOME (001) - REQUIRED for client to finish connecting
    std::string nick = client.getNickname();
    std::string msg = ":localhost 001 " + nick +
                      " :Welcome to ft_irc, " + nick + "\r\n";
    std::vector<char> resp(msg.begin(), msg.end());
    client.send(resp);

    // usually servers also send 002, 003, 004 here, but 001 is the trigger.
    std::cout << "Client " << nick << " is now registered!" << std::endl;
  }
}

void Commands::pass(IRCMessage const &msg, Client &client) {
  (void)msg;
  (void)client;
  // TODO: check password here
  // if (msg.getParams()[0] != server_password) {
  //   ReplyMessage::errPasswdMismatch();
  //   client.close();
  // }
}

void Commands::nick(IRCMessage const &msg, ClientManager &clients,
                    Client &client) {
  if (msg.getParams().empty()) {
    ReplyMessage::errNoNickNameGiven();
    return;
  }

  std::string newNick = msg.getParams()[0];

  // nickname validation
  if (newNick.empty()) {
    ReplyMessage::errErroneusNuckname(newNick);
    return;
  }

  // we check collistion
  // eventually this will use ClientManager
  std::vector<Client> &allClients = clients.getClients();
  for (size_t i = 0; i < allClients.size(); i++) {
    if (allClients[i].getSocket() !=
        client.getSocket()) { // Don't check against self
      if (allClients[i].getNickname() == newNick) {
        ReplyMessage::errNicknameInUse(newNick);
        return;
      }
    }
  }

  // if user was already registered, we might need to broadcast nick change
  // i haven't done it yet bc i just want it to work
  client.setNickname(newNick);

  checkRegistration(client); // try registration
}

void Commands::user(IRCMessage const &msg, Client &client) {
  if (client.getRegistered()) {
    ReplyMessage::errAlreadyRegistered();
    return;
  }

  // USER <username> <hostname> <servername> <realname>
  if (msg.getParams().size() < 3 || msg.getTrailing().empty()) {
    ReplyMessage::errNeedMoreParams("USER");
    return;
  }

  client.setUsername(msg.getParams()[0]);
  // client.setHostname(msg.getParams()[1]); // Optional: store hostname
  // Realname is usually in the trailing part

  checkRegistration(client); // try registration
}
