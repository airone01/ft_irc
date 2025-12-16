#include "Commands.hpp"
#include <iostream>
#include <vector>

void Commands::broadcastToChannel(Channel& channel, const std::string& message, ClientManager& clients, int senderSocket) {
	const std::set<int>& users = channel.getUsers();
	std::set<int>::const_iterator it = users.begin();
	std::set<int>::const_iterator ite = users.end();
	for (; it != ite; ++it) {
		if (*it == senderSocket)
			continue;
		try {
			Client& client = clients.getClientFromSocket(*it);
			if (client.getSocket() == -1)
				continue;
			client.sendMessage(message);
		} catch (const ClientManager::ClientNotFound&) {
		}
	}
}

static std::vector<std::string> paramHandler(const std::string& params) {
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

void Commands::join(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	const std::vector<std::string>& params = msg.getParams();
	if (params.empty()) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("JOIN"));
		return ;
	}
	std::vector<std::string> rooms;
	std::vector<std::string> keys;
	if (!msg.getParams().empty())
		rooms = paramHandler(params[0]);
	if (msg.getParams().size() >= 2)
		keys = paramHandler(params[1]);
	for (size_t i = 0; i< rooms.size(); i++) {
		const std::string& channelName = rooms[i];
		const std::string& password = (i < keys.size() && !keys[i].empty()) ? keys[i] : "";
		Channel* channel = NULL;
		if (channels.hasChannel(channelName)) {
			try {
				channel = &channels.getChannelFromName(channelName);
				channel->tryJoin(clientSocket, password);
				channel->addUser(clientSocket);
			} catch (const std::runtime_error& e) {
				std::string error = e.what();
				if (error == "ERR_NOSUCHCHANNEL")
					client.sendMessage(ReplyMessage::errNoSuchChannel(channelName));
				else if (error == "ERR_BANNEDFROMCHAN")
					client.sendMessage(ReplyMessage::errBannedFromChan(channelName));
				else if (error == "ERR_INVITEONLYCHAN")
					client.sendMessage(ReplyMessage::errInviteOnlyChan(channelName));
				else if (error == "ERR_BADCHANNELKEY")
					client.sendMessage(ReplyMessage::errBadChannelKey(channelName));
				else if (error == "ERR_CHANNELISFULL")
					client.sendMessage(ReplyMessage::errChannelIsFull(channelName));
				continue ;
			}
		} else {
			try {
				channel = &channels.createChannel(channelName, clientSocket);
				channel->addAdmin(clientSocket);
			} catch (const Channel::InvalidChanName& ) {
				client.sendMessage(ReplyMessage::errNoSuchChannel(channelName));
				continue ;
			} catch (const ChannelManager::ChannelAlreadyExists& ) {
				client.sendMessage(ReplyMessage::errNoSuchChannel(channelName));
				continue ;
			}
		}
		std::ostringstream joinMsg;
		joinMsg << client.getNickname() << " JOIN " << channelName << "\r\n";
		broadcastToChannel(*channel, joinMsg.str(), clients, clientSocket);
		if (!channel->getTopic().empty())
			client.sendMessage(ReplyMessage::rplTopic(channelName, channel->getTopic()));
		else
			client.sendMessage(ReplyMessage::rplNoTopic(channelName));
		std::ostringstream namesMsg;
		const std::set<int>& users = channel->getUsers();
		std::set<int>::const_iterator it = users.begin();
		std::set<int>::const_iterator ite = users.end();
		for (; it != ite; ++it) {
			Client& user = clients.getClientFromSocket(*it);
			if (channel->isAdmin(*it))
				namesMsg << "@";
			namesMsg << user.getNickname() << " ";
		}
		client.sendMessage(ReplyMessage::rplNamReply("= " + channelName, namesMsg.str()));
		client.sendMessage(ReplyMessage::rplEndOfNames(channelName));
	}
}

void Commands::part(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	std::vector<std::string> param = msg.getParams();
	std::vector<std::string>:: iterator it = param.begin();
	std::vector<std::string>:: iterator ite = param.end();
	for (; it != ite; ++it) {
		try {
			Channel& channel = channels.getChannelFromName(*it);
			channel.removeUser(clientSocket);
			if (channel.getUserCount() == 0) {
				channels.removeChannel(*it);
				channel.~Channel();
			}
		} catch (const std::runtime_error& e) {
			const std::string error = e.what();
			if (error == "ERR_NOSUCHCHANNEL")
				client.sendMessage(ReplyMessage::errNoSuchChannel(*it));
			else if (error == "ERR_USERNOTINCHANNEL")
				client.sendMessage(ReplyMessage::errUserNotInChannel(client.getNickname(), *it));
		}
	}
}

void Commands::topic(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	if (msg.getCountParams() != 1)
		client.sendMessage(ReplyMessage::errNeedMoreParams("TOPIC"));
	const std::vector<std::string>& params = msg.getParams();
	try {
		Channel& channel = channels.getChannelFromName(params[0]);
		channel.changeTopic(msg.getTrailing(), clientSocket);
		client.sendMessage(ReplyMessage::rplTopic(channel.getName(), channel.getTopic()));
	} catch (const std::runtime_error& e) {
		const std::string error = e.what();
		if (error == "ERR_NOSUCHCHANNEL")
			client.sendMessage(ReplyMessage::errNoSuchChannel(params[0]));
		if (error == "ERR_NOTONCHANNEL")
			client.sendMessage(ReplyMessage::errNotOnChannel(params[0]));
		if (error == "ERR_CHANOPRIVSNEEDED")
			client.sendMessage(ReplyMessage::errChaNoPrivsNeeded(params[0]));
		if (error == "RPL_NOTOPIC")
			client.sendMessage(ReplyMessage::rplNoTopic(params[0]));
	}
}

void Commands::mode(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	const std::vector<std::string>& params = msg.getParams();
	if (params.empty()) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("MODE"));
		return;
	}
	const std::string& target = params[0];
	if (target[0] == '#' || target[0] == '&') {
		if (!channels.hasChannel(target)) {
			client.sendMessage(ReplyMessage::errNoSuchChannel(target));
			return;
		}
		Channel& channel = channels.getChannelFromName(target);
		if (params.size() == 1) {
			std::string modeStr = "+";
			std::string modeParams = "";
			const std::set<char>& modes = channel.getMode();
			std::set<char>::const_iterator it = modes.begin();
			std::set<char>::const_iterator ite = modes.end();
			for (; it != ite; ++it) {
				modeStr += *it;
			}
			if (channel.hasMode('l')) {
				std::ostringstream oss;
				oss << channel.getMaxCapacity();
				modeParams += " " + oss.str();
			}
			if (channel.hasMode('k')) {
				modeParams += " " + channel.getPassword();
			}
			client.sendMessage(ReplyMessage::rplChannelModeIs(target, modeStr, modeParams));
			return;
		}
		if (!channel.hasUser(clientSocket)) {
			client.sendMessage(ReplyMessage::errNotOnChannel(target));
			return;
		}
		try {
			channel.updateMode(msg, clientSocket, clients);
			std::ostringstream modeMsg;
			modeMsg << ":" << client.getNickname() << " MODE " << target;
			for (size_t i = 1; i < params.size(); ++i) {
				modeMsg << " " << params[i];
			}
			modeMsg << "\r\n";
			broadcastToChannel(channel, modeMsg.str(), clients, -1);
		} catch (const std::runtime_error& e) {
			std::string error = e.what();
			if (error == "ERR_NEEDMOREPARAMS")
				client.sendMessage(ReplyMessage::errNeedMoreParams("MODE"));
			else if (error == "ERR_CHANOPRIVSNEEDED")
				client.sendMessage(ReplyMessage::errChaNoPrivsNeeded(target));
			else if (error == "ERR_NOSUCHNICK")
				client.sendMessage(ReplyMessage::errNoSuchNick(params.size() > 2 ? params[2] : ""));
			else if (error == "ERR_USERNOTINCHANNEL")
				client.sendMessage(ReplyMessage::errUserNotInChannel(params.size() > 2 ? params[2] : "", target));
			else if (error == "ERR_UNKNOWNMODE")
				client.sendMessage(ReplyMessage::errUnknownMode(params.size() > 1 ? params[1] : ""));
		}
	}
	else {
		client.sendMessage(ReplyMessage::errUModeUnknownFlag());
	}
}

void Commands::kick(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	const std::vector<std::string>& params = msg.getParams();
	if (params.size() < 2) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("KICK"));
		return;
	}
	const std::string& channelName = params[0];
	const std::string& targetNick = params[1];
	std::string reason = (params.size() > 2) ? params[2] : client.getNickname();
	if (!channels.hasChannel(channelName)) {
		client.sendMessage(ReplyMessage::errNoSuchChannel(channelName));
		return;
	}
	Channel& channel = channels.getChannelFromName(channelName);
	if (!channel.hasUser(clientSocket)) {
		client.sendMessage(ReplyMessage::errNotOnChannel(channelName));
		return;
	}
	if (!channel.isAdmin(clientSocket)) {
		client.sendMessage(ReplyMessage::errChaNoPrivsNeeded(channelName));
		return;
	}
	int targetSocket;
	try {
		Client& target = clients.getClientFromNickname(targetNick);
		targetSocket = target.getSocket();
	} catch (const ClientManager::ClientNotFound&) {
		client.sendMessage(ReplyMessage::errNoSuchNick(targetNick));
		return;
	}
	if (!channel.hasUser(targetSocket)) {
		client.sendMessage(ReplyMessage::errUserNotInChannel(targetNick, channelName));
		return;
	}
	std::ostringstream kickMsg;
	kickMsg << ":" << client.getNickname() << " KICK " << channelName << " " << targetNick << " :" << reason << "\r\n";
	broadcastToChannel(channel, kickMsg.str(), clients, -1);
	channel.kickUser(targetSocket);
}

void Commands::invite(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	const std::vector<std::string>& params = msg.getParams();
	if (params.size() < 2) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("INVITE"));
		return;
	}
	const std::string& targetNick = params[0];
	const std::string& channelName = params[1];
	int targetSocket;
	Client* targetClient;
	try {
		targetClient = &clients.getClientFromNickname(targetNick);
		targetSocket = targetClient->getSocket();
	} catch (const ClientManager::ClientNotFound&) {
		client.sendMessage(ReplyMessage::errNoSuchNick(targetNick));
		return;
	}
	if (!channels.hasChannel(channelName)) {
		client.sendMessage(ReplyMessage::errNoSuchChannel(channelName));
		return;
	}
	Channel& channel = channels.getChannelFromName(channelName);
	if (!channel.hasUser(clientSocket)) {
		client.sendMessage(ReplyMessage::errNotOnChannel(channelName));
		return;
	}
	if (channel.hasMode('i') && !channel.isAdmin(clientSocket)) {
		client.sendMessage(ReplyMessage::errChaNoPrivsNeeded(channelName));
		return;
	}
	if (channel.hasUser(targetSocket)) {
		client.sendMessage(ReplyMessage::errUserOnChannel(targetNick, channelName));
		return;
	}
	channel.inviteUser(targetSocket);
	client.sendMessage(ReplyMessage::rplInviting(channelName, targetNick));
	std::ostringstream inviteMsg;
	inviteMsg << ":" << client.getNickname() << " INVITE " << targetNick << " " << channelName << "\r\n";
	targetClient->sendMessage(inviteMsg.str());
}


void pass(int clientSocket, const IRCMessage& msg, ClientManager& clients, const std::string& serverPassword);
void nick(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
void user(int clientSocket, const IRCMessage& msg, ClientManager& clients);
void quit(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);

void privmsg(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
void notice(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);

void ping(int clientSocket, const IRCMessage& msg, ClientManager& clients);
void pong(int clientSocket, const IRCMessage& msg, ClientManager& clients);

void who(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
void whois(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
void list(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
void names(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);




// void Commands::join(IRCMessage const &tmp, ChannelManager &channels,
//					 Client &user) {
//   std::vector<std::string> param = tmp.getParams();
//   if (param.empty()) {
//	 ReplyMessage::errNeedMoreParams("JOIN");
//	 return;
//   }

//   std::vector<std::string> rooms;
//   std::vector<std::string> keys;

//   if (tmp.getParams().size() >= 1)
//	 rooms = paramHandler(param[0]);
//   if (tmp.getParams().size() >= 2)
//	 keys = paramHandler(param[1]);

//   for (size_t i = 0; i < rooms.size(); i++) {
//	 std::string name = rooms[i];
//	 std::string key = (i < keys.size()) ? keys[i] : "";

//     Channel *chan = NULL;
//     try {
//       // try to get existing channel
//       chan = &channels.getChannelFromName(name);
//     } catch (const ChannelManager::noSuchChannel &e) {
//       // create new if it doesn't exist
//       // TODO: ensure channel doesn't duplicate the user if we call newUser()
//       // later.
//       try {
//         Channel newChan(user, name);
//         channels.addChannels(newChan);
//         chan = &channels.getChannelFromName(name);
//       } catch (const std::exception &e2) {
//         logger::error() << e2.what() << std::endl;
//         continue;
//       }
//     }

//     // validatation (password, limit, invite)
//     try {
//       // only check constraints if it's an existing channel
//       // (if we just created it, we are the owner/first user, so we get in free)
//       bool isNew =
//           (chan->getUsers().find(user.getSocket()) != chan->getUsers().end());

//       if (!isNew) {
//         chan->tryJoin(user, key);
//         chan->newUser(user);
//       }
//     } catch (const std::exception &e) {
//       // send error reply (e.g. ERR_BADCHANNELKEY)
//       logger::error() << "Join Error: " << e.what() << std::endl;
//       user.send(ReplyMessage::errBadChannelKey(name));
//       continue;
//     }

//     // broadcasting join message
//     std::string joinMsg = ":" + user.getNickname() + "!" + user.getUsername() +
//                           "@" + user.getHostname() +
//                           " JOIN :" + chan->getName() + "\r\n";
//     std::vector<char> joinResp(joinMsg.begin(), joinMsg.end());
//     // send to everyone in the channel (including the joiner)
//     std::map<int, Client *> users = chan->getUsers();
//     for (std::map<int, Client *>::iterator it = users.begin();
//          it != users.end(); ++it) {
//       it->second->send(joinResp);
//     }

//     // send topic
//     if (!chan->getTopic().empty()) {
//       user.send(ReplyMessage::rplTopic(chan->getName(), chan->getTopic()));
//     }

//     // send names list
//     std::string namesList;
//     for (std::map<int, Client *>::iterator it = users.begin();
//          it != users.end(); ++it) {
//       // logic to check if OP (@) or Voice (+) goes here
//       // for now, just list them
//       namesList += it->second->getNickname() + " ";
//     }

//     std::string rplNames = ":localhost 353 " + user.getNickname() + " = " +
//                            chan->getName() + " :" + namesList + "\r\n";
//     std::vector<char> nResp(rplNames.begin(), rplNames.end());
//     user.send(nResp);

//     std::string rplEndNames = ":localhost 366 " + user.getNickname() + " " +
//                               chan->getName() + " :End of /NAMES list\r\n";
//     std::vector<char> enResp(rplEndNames.begin(), rplEndNames.end());
//     user.send(enResp);
//   }
// }

// void Commands::part(IRCMessage const &tmp, ChannelManager &channels,
//                     Client &user) {
//   std::vector<std::string> param = tmp.getParams();
//   std::vector<std::string>::iterator it;
//   for (it = param.begin(); it != param.end(); it++) {
//     try {
//       Channel &actual = channels.getChannelFromName(*it);
//       actual.leaveChannel(user);
//       if (actual.getUsers().empty()) {
//         channels.rmChannels(actual);
//         actual.~Channel();
//         // todo: reply
//       }
//     } catch (const std::exception &e) {
//       logger::error() << e.what() << '\n';
//     }
//   }
// }

// /*
// flags:
//         i(set/unset invite only)
//         t(set/unset topic priv to admin)
//         k(set/unset password)
//         o(give/take admin priv)
//         l(set/unset limit size)

// replies :
//            ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
//            ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_KEYSET
//            RPL_BANLIST                     RPL_ENDOFBANLIST
//            ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL
//            ERR_USERSDONTMATCH              ERR_UMODEUNKNOWNFLAG
//            RPL_UMODEIS
// */
// void Commands::mode(IRCMessage const &param, ChannelManager &channels,
//                     Client &user) {
//   std::vector<std::string> params = param.getParams();
//   if (params.size() < 1) {
//     ReplyMessage::errNeedMoreParams("MODE");
//     return;
//   }

//   std::string target = params[0];

//   if (target[0] == '#' || target[0] == '&') {
//     try {
//       Channel &chan = channels.getChannelFromName(target);

//       // query mode
//       if (params.size() == 1) {
//         std::string modeStr = "+";
//         std::set<char> modes = chan.getMode();
//         for (std::set<char>::iterator it = modes.begin(); it != modes.end();
//              ++it) {
//           modeStr += *it;
//         }
//         // also append args for k and l if they exist
//         // (usually sending args in trailing works)
//         std::string rpl = ":localhost 324 " + user.getNickname() + " " +
//                           target + " " + modeStr + "\r\n";
//         user.send(rpl);
//         return;
//       }

//       // change mode
//       std::string changes = chan.updateMode(param, user);
//       if (!changes.empty()) {
//         // boradcast
//         std::string msg = ":" + user.getNickname() + "!" + user.getUsername() +
//                           "@" + user.getHostname() + " MODE " + target + " :" +
//                           changes + "\r\n";
//         std::vector<char> raw(msg.begin(), msg.end());
//         std::map<int, Client *> users = chan.getUsers();
//         for (std::map<int, Client *>::iterator it = users.begin();
//              it != users.end(); ++it) {
//           it->second->send(raw);
//         }
//       }
//     } catch (const Channel::errorMode &e) {
//       // Handle specific errors like ERR_CHANOPRIVSNEEDED (482)
//       if (std::string(e.what()) == "ERR_CHANOPRIVSNEEDED") {
//         user.send(ReplyMessage::errChaNoPrivsNeeded(target));
//       } else {
//         user.send(ReplyMessage::errUnknownMode(e.what()));
//       }
//     } catch (const std::exception &e) {
//       // Fallback
//       logger::error() << "Mode error: " << e.what() << std::endl;
//     }
//   }
// }

// void Commands::topic(IRCMessage const &tmp, ChannelManager &channels,
//                      Client &user) {
//   if (tmp.getCountParams() != 1)
//     logger::error() << "ERR_NEEDMOREPARAMS\n";
//   std::vector<std::string> param = paramHandler(tmp.getParams()[0]);
//   std::vector<std::string>::iterator it = param.begin();
//   Channel &chan = channels.getChannelFromName(param[0]);

//   if (!tmp.getTrailing().empty()) {
//     // CHECK +t and +o
//     if (chan.getMode().find('t') != chan.getMode().end()) {
//       if (!chan.isOperator(user)) {
//         user.send(ReplyMessage::errChaNoPrivsNeeded(param[0]));
//         return;
//       }
//     }
//   }

//   for (; it != param.end(); it++) {
//     try {
//       Channel &actual = channels.getChannelFromName(*it);
//       actual.changeTopic(tmp, user);
//     } catch (const std::exception &e) {
//       logger::error() << e.what() << '\n';
//     }
//   }
// }

// void Commands::invite(IRCMessage const &tmp, ClientManager &clients,
//                       ChannelManager &channels, Client &user) {
//   std::vector<std::string> param = tmp.getParams();
//   Channel &chan = channels.getChannelFromName(param[1]);

//   try {
//     if (chan.getMode().find('i') != chan.getMode().end()) {
//       if (!chan.isOperator(user)) {
//         user.send(ReplyMessage::errChaNoPrivsNeeded(param[1]));
//         return;
//       }
//     }
//     Channel &actual = channels.getChannelFromName(param[1]);
//     actual.tryInvite(param, clients, user.getSocket(),
//                      clients.getClientFromUsername(param[0]).getSocket());
//   } catch (const std::exception &e) {
//     logger::error() << e.what() << std::endl;
//   }
// }

// void Commands::kick(IRCMessage const &tmp, ChannelManager &channels,
//                     Client &admin) {
//   std::vector<std::string> param = tmp.getParams();
//   if (param.size() < 2) {
//     // TODO: handle error
//     return;
//   }

//   try {
//     Channel &actual = channels.getChannelFromName(param[0]);
//     std::string targetNick = param[1];
//     actual.tryKick(param, tmp, admin);

//     Client *victim = NULL;
//     std::map<int, Client *> &users = actual.getUsers();

//     for (std::map<int, Client *>::iterator it = users.begin();
//          it != users.end(); ++it) {
//       if (it->second->getNickname() == targetNick) {
//         victim = it->second;
//         break;
//       }
//     }

//     if (victim) {
//       // kicking the user
//       // sending the message BEFORE removing the user so they receive
//       // it.

//       actual.setKickedUsers(victim->getSocket());
//       actual.leaveChannel(*victim);

//       logger::info() << admin.getNickname() << " kicked " << targetNick
//                      << std::endl;
//     } else {
//       admin.send(
//           ReplyMessage::errUserNotInChannel(targetNick, actual.getName()));
//     }

//   } catch (const std::exception &e) {
//     logger::error() << e.what() << '\n';
//   }
// }

// /**
//  *	@Brief This is the parsing of the messages sent.
//  *	Format			:	[ ':' <prefix> <SPACE> ] <PRIVMSG>
//  * <SPACE> <params1>[ ',' <params2> ] [ <SPACE> <params2> [ ',' <params2>]]
//  * <SPACE> ':' <trailing> Numeric Replies	: ERR_NORECIPIENT
//  *			ERR_NOTEXTTOSEND ERR_CANNOTSENDTOCHAN
//  *	ERR_NOTOPLEVEL ERR_WILDTOPLEVEL 	ERR_TOOMANYTARGETS
//  *						ERR_NOSUCHNICK
//  *		RPL_AWAY Exemple			:	:Angel PRIVMSG
//  * Wiz :Hello are you receiving this message ?; PRIVMSG Angel :yes I'm
//  * receiving it !receiving it !'u>(768u+1n) .br; PRIVMSG jtotolsun.oulu.fi
//  * :Hello !; PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.; Message to
//  * everyone on a server which has a name matching *.fi. PRIVMSG #*.edu :NSFNet
//  * is undergoing work, expect interruptions; Message to all users who come
//  * from a host which has a name matching *.edu.
//  */
// void Commands::privmsg(IRCMessage const &msg, Client &sender,
//                        ClientManager &clients, ChannelManager &channels) {
//   if (msg.getCountParams() < 1) {
//     logger::error() << "411 ERROR HANDLING :No recipient given "
//                     << msg.getCommand() << std::endl;
//     return;
//   }
//   if (msg.getTrailing().empty()) {
//     logger::error() << "412 ERR_NOTEXTTOSEND :No text to send" << std::endl;
//     return;
//   }
//   std::string target = msg.getParams()[0];
//   std::string message = msg.getTrailing();
//   std::string formatted = ":" + sender.getNickname() + "!" +
//                           sender.getUsername() + "@" + sender.getHostname() +
//                           " PRIVMSG " + target + " :" + message + "\r\n";
//   std::vector<char> msgVec(formatted.begin(), formatted.end());
//   if (!target.empty() && (target[0] == '#' || target[0] == '&')) {
//     try {
//       Channel &chan = channels.getChannelFromName(target);
//       std::map<int, Client *> users = chan.getUsers();
//       if (users.find(sender.getSocket()) == users.end()) {
//         logger::error() << "404 ERR_CANNOTSENDTOCHAN " << msg.getParams()[0]
//                         << " :Cannot send to channel" << std::endl;
//         return;
//       }
//       for (std::map<int, Client *>::iterator it = users.begin();
//            it != users.end(); ++it) {
//         if (it->second->getSocket() != sender.getSocket()) {
//           it->second->send(msgVec);
//         }
//       }
//     } catch (const std::exception &e) {
//       logger::error() << "403 ERR_NOSUCHCHANNEL " << msg.getParams()[0]
//                       << " :No such channel" << std::endl;
//     }
//   } else {
//     try {
//       Client &recipient = clients.getClientFromUsername(target);
//       recipient.send(msgVec);
//     } catch (const std::exception &e) {
//       logger::error() << "401 ERR_NOSUCHNICK :No such nick" << std::endl;
//     }
//   }
// }

// void Commands::version(Client &sender) {
//   sender.send(ReplyMessage::rplVersion("1.0.0", "ft_irc", "com"));
// }

// void checkRegistration(Client &client) {
//   if (client.getRegistered())
//     return; // already registered

//   // we need at least a Nickname and a Username to register
//   if (client.getAuth() && !client.getNickname().empty() &&
//       !client.getUsername().empty()) {
//     client.setRegistered(true);

//     // send RPL_WELCOME (001) - REQUIRED for client to finish connecting
//     std::string nick = client.getNickname();
//     std::string msg =
//         ":localhost 001 " + nick + " :Welcome to ft_irc, " + nick + "\r\n";
//     std::vector<char> resp(msg.begin(), msg.end());
//     client.send(resp);

//     // usually servers also send 002, 003, 004 here, but 001 is the trigger.
//     logger::info() << "Client " << nick << " is now registered!" << std::endl;
//   }
// }

void Commands::pass(int clientSocket, const IRCMessage& msg, ClientManager& clients, const std::string& serverPassword) {
  Client &client = clients.getClientFromSocket(clientSocket);

  if (client.getRegistered()) {
    client.sendMessage(ReplyMessage::errAlreadyRegistered());
    return;
  }

  if (msg.getParams().empty()) {
    client.sendMessage(ReplyMessage::errNeedMoreParams("PASS"));
    return;
  }

  std::string providedPass = msg.getParams()[0];
  if (providedPass != serverPassword) {
    client.sendMessage(ReplyMessage::errPasswdMismatch());
    return;
  }

  client.setAuth(true);
  std::cerr << client.getAuth() << '\n';
}

// void Commands::nick(IRCMessage const &msg, ClientManager &clients,
//                     Client &client) {
//   if (msg.getParams().empty()) {
//     ReplyMessage::errNoNickNameGiven();
//     return;
//   }

//   std::string newNick = msg.getParams()[0];

//   // nickname validation
//   if (newNick.empty()) {
//     ReplyMessage::errErroneusNuckname(newNick);
//     return;
//   }

//   // we check collistion
//   // eventually this will use ClientManager
//   std::vector<Client> &allClients = clients.getClients();
//   for (size_t i = 0; i < allClients.size(); i++) {
//     if (allClients[i].getSocket() !=
//         client.getSocket()) { // Don't check against self
//       if (allClients[i].getNickname() == newNick) {
//         ReplyMessage::errNicknameInUse(newNick);
//         return;
//       }
//     }
//   }

//   // if user was already registered, we might need to broadcast nick change
//   // i haven't done it yet bc i just want it to work
//   client.setNickname(newNick);

//   checkRegistration(client); // try registration
// }

// void Commands::user(IRCMessage const &msg, Client &client) {
//   if (client.getRegistered()) {
//     ReplyMessage::errAlreadyRegistered();
//     return;
//   }

//   // USER <username> <hostname> <servername> <realname>
//   if (msg.getParams().size() < 3 || msg.getTrailing().empty()) {
//     ReplyMessage::errNeedMoreParams("USER");
//     return;
//   }

//   client.setUsername(msg.getParams()[0]);
//   // client.setHostname(msg.getParams()[1]); // Optional: store hostname
//   // Realname is usually in the trailing part

//   checkRegistration(client); // try registration
// }

// /**
//  * CAP is needed for advanced clients such as IRSSI or HexChat which do not
//  * limit their support to the classic RFC.
//  * We have to tell them that out server does not, in fact, implement advanced
//  * stuff.
//  */
// void Commands::cap(IRCMessage const &msg, Client &client) {
//   // we expect 1 param at least for CAP (LS, REQ, END, etc.)
//   if (msg.getParams().empty())
//     return;

//   std::string subcommand = msg.getParams()[0];

//   // 1. CAP LS: client asks "what do you support?"
//   // we reply with an empty list (":") meaning "nothing special".
//   if (subcommand == "LS") {
//     logger::debug() << "CAP LIST asked. Sending empty list." << std::endl;
//     std::string resp = ":localhost CAP * LS :\r\n";
//     std::vector<char> r(resp.begin(), resp.end());
//     client.send(r);
//   }
//   // CAP END: the server says "okay, I understand".
//   // we do nothing and wait for more requests.
//   else if (subcommand == "END") {
//     return;
//   }
//   // 3. CAP REQ: client asks if we support a specific feature. we don't.
//   // we deny everything with NAK.
//   else if (subcommand == "REQ") {
//     logger::debug() << "CAP REQ asked. Denying." << std::endl;
//     std::string resp = ":localhost CAP * NAK :\r\n";
//     std::vector<char> r(resp.begin(), resp.end());
//     client.send(r);
//   }
// }

void Commands::quit(int clientSocket, ClientManager &manager){
  Client &client = manager.getClientFromSocket(clientSocket);
  // close(clientSocket);
  client.setSocket(0);
  
}
