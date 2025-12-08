/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/12/08 15:07:06 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "ChannelManager.hpp"
#include "ClientManager.hpp"
#include "Logger.hpp"
#include "ReplyMessage.hpp"
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

void Commands::join(IRCMessage const &tmp, ChannelManager &channels,
                    Client &user) {
  std::vector<std::string> param = tmp.getParams();
  if (param.empty()) {
    ReplyMessage::errNeedMoreParams("JOIN");
    return;
  }

  std::vector<std::string> rooms;
  std::vector<std::string> keys;

  if (tmp.getParams().size() >= 1)
    rooms = paramHandler(param[0]);
  if (tmp.getParams().size() >= 2)
    keys = paramHandler(param[1]);

  for (size_t i = 0; i < rooms.size(); i++) {
    std::string name = rooms[i];
    std::string key = (i < keys.size()) ? keys[i] : "";

    Channel *chan = NULL;
    try {
      // try to get existing channel
      chan = &channels.getChannelFromName(name);
    } catch (const ChannelManager::noSuchChannel &e) {
      // create new if it doesn't exist
      // TODO: ensure channel doesn't duplicate the user if we call newUser()
      // later.
      try {
        Channel newChan(user, name);
        channels.addChannels(newChan);
        chan = &channels.getChannelFromName(name);
      } catch (const std::exception &e2) {
        logger::error() << e2.what() << std::endl;
        continue;
      }
    }

    // validatation (password, limit, invite)
    try {
      // only check constraints if it's an existing channel
      // (if we just created it, we are the owner/first user, so we get in free)
      bool isNew =
          (chan->getUsers().find(user.getSocket()) != chan->getUsers().end());

      if (!isNew) {
        chan->tryJoin(user, key);
        chan->newUser(user);
      }
    } catch (const std::exception &e) {
      // send error reply (e.g. ERR_BADCHANNELKEY)
      logger::error() << "Join Error: " << e.what() << std::endl;
      user.send(ReplyMessage::errBadChannelKey(name));
      continue;
    }

    // broadcasting join message
    std::string joinMsg = ":" + user.getNickname() + "!" + user.getUsername() +
                          "@" + user.getHostname() +
                          " JOIN :" + chan->getName() + "\r\n";
    std::vector<char> joinResp(joinMsg.begin(), joinMsg.end());
    // send to everyone in the channel (including the joiner)
    std::map<int, Client *> users = chan->getUsers();
    for (std::map<int, Client *>::iterator it = users.begin();
         it != users.end(); ++it) {
      it->second->send(joinResp);
    }

    // send topic
    if (!chan->getTopic().empty()) {
      user.send(ReplyMessage::rplTopic(chan->getName(), chan->getTopic()));
    }

    // send names list
    std::string namesList;
    for (std::map<int, Client *>::iterator it = users.begin();
         it != users.end(); ++it) {
      // logic to check if OP (@) or Voice (+) goes here
      // for now, just list them
      namesList += it->second->getNickname() + " ";
    }

    std::string rplNames = ":localhost 353 " + user.getNickname() + " = " +
                           chan->getName() + " :" + namesList + "\r\n";
    std::vector<char> nResp(rplNames.begin(), rplNames.end());
    user.send(nResp);

    std::string rplEndNames = ":localhost 366 " + user.getNickname() + " " +
                              chan->getName() + " :End of /NAMES list\r\n";
    std::vector<char> enResp(rplEndNames.begin(), rplEndNames.end());
    user.send(enResp);
  }
}

void Commands::part(IRCMessage const &tmp, ChannelManager &channels,
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
      logger::error() << e.what() << '\n';
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
void Commands::mode(IRCMessage const &param, ChannelManager &channels,
                    Client &user) {
  std::vector<std::string> params = param.getParams();
  if (params.size() < 1) {
    ReplyMessage::errNeedMoreParams("MODE");
    return;
  }

  std::string target = params[0];

  if (target[0] == '#' || target[0] == '&') {
    Channel *actual = NULL;

    // find channel.
    try {
      actual = &channels.getChannelFromName(target);
    } catch (const ChannelManager::noSuchChannel &e) {
      ReplyMessage::errNoSuckChannel(target);
      return;
    }

    // MODE QUERY (no flags given)
    if (params.size() == 1) {
      // construct the mode string from the set of active modes.
      std::string modeStr = "+";
      std::set<char> modes = actual->getMode();
      for (std::set<char>::iterator it = modes.begin(); it != modes.end();
           ++it) {
        modeStr += *it;
      }

      // RPL_CHANNELMODEIS (324): :<server> 324 <nick> <channel> <mode> <mode
      // params> Note: for simplicity we dc about mode parameters here.
      std::string response = ":localhost 324 " + user.getNickname() + " " +
                             target + " " + modeStr + "\r\n";
      std::vector<char> respVec(response.begin(), response.end());
      user.send(respVec);
      return;
    }

    // MODE CHANGE (flags given)
    try {
      if (actual)
        actual->updateMode(param, user);

      // TODO: if updateMode succeeds, broadcast the new mode change to the
      // channel.
    } catch (const std::exception &e) {
      std::cerr << "MODE Error: " << e.what() << std::endl;
      // if the error is related to parameters (e.g., ERR_UNKNOWNMODE),
      // the error should be sent to the client.
    }
  } else {
    if (target == user.getNickname()) {
      return;
    } else {
      ReplyMessage::errUsersDontMatch();
    }
  }
}

void Commands::topic(IRCMessage const &tmp, ChannelManager &channels,
                     Client &user) {
  if (tmp.getCountParams() != 1)
    logger::error() << "ERR_NEEDMOREPARAMS\n";
  std::vector<std::string> param = paramHandler(tmp.getParams()[0]);
  std::vector<std::string>::iterator it = param.begin();

  for (; it != param.end(); it++) {
    try {
      Channel &actual = channels.getChannelFromName(*it);
      actual.changeTopic(tmp, user);
    } catch (const std::exception &e) {
      logger::error() << e.what() << '\n';
    }
  }
}

void Commands::invite(IRCMessage const &tmp, ClientManager &clients,
                      ChannelManager &channels, Client &user) {
  std::vector<std::string> param = tmp.getParams();
  try {
    Channel &actual = channels.getChannelFromName(param[1]);
    actual.tryInvite(param, clients, user.getSocket(),
                     clients.getClientFromUsername(param[0]).getSocket());
  } catch (const std::exception &e) {
    logger::error() << e.what() << std::endl;
  }
}

void Commands::kick(IRCMessage const &tmp, ChannelManager &channels,
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
    logger::error() << e.what() << '\n';
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
    logger::error() << "411 ERROR HANDLING :No recipient given "
                    << msg.getCommand() << std::endl;
    return;
  }
  if (msg.getTrailing().empty()) {
    logger::error() << "412 ERR_NOTEXTTOSEND :No text to send" << std::endl;
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
        logger::error() << "404 ERR_CANNOTSENDTOCHAN " << msg.getParams()[0]
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
      logger::error() << "403 ERR_NOSUCHCHANNEL " << msg.getParams()[0]
                      << " :No such channel" << std::endl;
    }
  } else {
    try {
      Client &recipient = clients.getClientFromUsername(target);
      recipient.send(msgVec);
    } catch (const std::exception &e) {
      logger::error() << "401 ERR_NOSUCHNICK :No such nick" << std::endl;
    }
  }
}

void Commands::version(Client &sender) {
  sender.send(ReplyMessage::rplVersion("1.0.0", "ft_irc", "com"));
}

void checkRegistration(Client &client) {
  if (client.getRegistered())
    return; // already registered

  // we need at least a Nickname and a Username to register
  if (client.getAuth() && !client.getNickname().empty() &&
      !client.getUsername().empty()) {
    client.setRegistered(true);

    // send RPL_WELCOME (001) - REQUIRED for client to finish connecting
    std::string nick = client.getNickname();
    std::string msg =
        ":localhost 001 " + nick + " :Welcome to ft_irc, " + nick + "\r\n";
    std::vector<char> resp(msg.begin(), msg.end());
    client.send(resp);

    // usually servers also send 002, 003, 004 here, but 001 is the trigger.
    logger::info() << "Client " << nick << " is now registered!" << std::endl;
  }
}

void Commands::pass(IRCMessage const &msg, Client &client,
                    const std::string &serverPass) {
  if (client.getRegistered()) {
    client.send(ReplyMessage::errAlreadyRegistered());
    return;
  }
  if (msg.getParams().empty()) {
    client.send(ReplyMessage::errNeedMoreParams("PASS"));
    return;
  }

  std::string providedPass = msg.getParams()[0];
  if (providedPass != serverPass) {
    client.send(ReplyMessage::errPasswdMismatch());
    // Note: idk about closing the connection right after a wrong password, but
    // many implementations do that
    client.close();
    return;
  }

  client.setAuth(true);
  logger::debug() << "Client " << client.getSocket() << " password verified."
                  << std::endl;
}

void Commands::nick(IRCMessage const &msg, ClientManager &clients,
                    Client &client) {
  if (msg.getParams().empty()) {
    ReplyMessage::errNoNickNameGiven();
    return;
  }
  if (!client.getAuth()) {
    client.send(ReplyMessage::errNotRegistered());
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
  if (!client.getAuth()) {
    client.send(ReplyMessage::errNotRegistered());
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

/**
 * CAP is needed for advanced clients such as IRSSI or HexChat which do not
 * limit their support to the classic RFC.
 * We have to tell them that out server does not, in fact, implement advanced
 * stuff.
 */
void Commands::cap(IRCMessage const &msg, Client &client) {
  // we expect 1 param at least for CAP (LS, REQ, END, etc.)
  if (msg.getParams().empty())
    return;

  std::string subcommand = msg.getParams()[0];

  // 1. CAP LS: client asks "what do you support?"
  // we reply with an empty list (":") meaning "nothing special".
  if (subcommand == "LS") {
    logger::debug() << "CAP LIST asked. Sending empty list." << std::endl;
    std::string resp = ":localhost CAP * LS :\r\n";
    std::vector<char> r(resp.begin(), resp.end());
    client.send(r);
  }
  // CAP END: the server says "okay, I understand".
  // we do nothing and wait for more requests.
  else if (subcommand == "END") {
    return;
  }
  // 3. CAP REQ: client asks if we support a specific feature. we don't.
  // we deny everything with NAK.
  else if (subcommand == "REQ") {
    logger::debug() << "CAP REQ asked. Denying." << std::endl;
    std::string resp = ":localhost CAP * NAK :\r\n";
    std::vector<char> r(resp.begin(), resp.end());
    client.send(r);
  }
}
