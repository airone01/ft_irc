#include "Commands.hpp"
#include <iostream>
#include <iterator>
#include <string>
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
		size_t nextComma = params.find(',', i);
		if (nextComma == std::string::npos) {
			tmp = params.substr(i);
			newParam.push_back(tmp);
			break;
		}
		tmp = params.substr(i, nextComma - i);
		newParam.push_back(tmp);
		i = nextComma + 1;
	}
	return newParam;
}

void Commands::join(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
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
		std::string joinMsg = ":" + client.getNickname() + " JOIN " + channelName + "\r\n";
		client.sendMessage(joinMsg);
		broadcastToChannel(*channel, joinMsg, clients, clientSocket);
		if (!channel->getTopic().empty())
			client.sendMessage(ReplyMessage::rplTopic(channelName, channel->getTopic()));
		std::string namesMsg;
		const std::set<int>& users = channel->getUsers();
		std::set<int>::const_iterator it = users.begin();
		std::set<int>::const_iterator ite = users.end();
		for (; it != ite; ++it) {
			Client& user = clients.getClientFromSocket(*it);
			if (channel->isAdmin(*it))
				namesMsg += "@";
			namesMsg += user.getNickname() + " ";
		}
		client.sendMessage(ReplyMessage::rplNamReply("= " + channelName, namesMsg));
		client.sendMessage(ReplyMessage::rplEndOfNames(channelName));
	}
}

void Commands::part(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
	if (msg.getParams().empty()) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("PART"));
		return;
	}
	std::vector<std::string> param = paramHandler(msg.getParams()[0]);
	std::vector<std::string>::iterator it = param.begin();
	std::vector<std::string>::iterator ite = param.end();
	for (; it != ite; ++it) {
		try {
			Channel& channel = channels.getChannelFromName(*it);
			std::string partMsg = ":" + client.getNickname() + " PART " + channel.getName();
			if (!msg.getTrailing().empty())
				partMsg += " :" + msg.getTrailing();
			partMsg += "\r\n";
			broadcastToChannel(channel, partMsg, clients, -1);
			channel.removeUser(clientSocket);
			if (channel.getUserCount() == 0) {
				channels.removeChannel(*it);
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
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
	if (msg.getCountParams() != 1) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("TOPIC"));
		return ;
	}
	const std::vector<std::string>& params = msg.getParams();
	try {
		Channel& channel = channels.getChannelFromName(params[0]);
		if (!msg.getTrailing().empty()) {
			channel.changeTopic(msg.getTrailing(), clientSocket);
			std::string topicMsg = ":" + client.getNickname() + " TOPIC " + channel.getName() + " :" + channel.getTopic() + "\r\n";
			broadcastToChannel(channel, topicMsg, clients, -1);
		}
		if (!channel.getTopic().empty())
			client.sendMessage(ReplyMessage::rplTopic(channel.getName(), channel.getTopic()));
		else
			client.sendMessage(ReplyMessage::rplNoTopic(params[0]));
	} catch (const std::runtime_error& e) {
		const std::string error = e.what();
		if (error == "ERR_NOSUCHCHANNEL")
			client.sendMessage(ReplyMessage::errNoSuchChannel(params[0]));
		if (error == "ERR_NOTONCHANNEL")
			client.sendMessage(ReplyMessage::errNotOnChannel(params[0]));
		if (error == "ERR_CHANOPRIVSNEEDED")
			client.sendMessage(ReplyMessage::errChaNoPrivsNeeded(params[0]));
	}
}

void Commands::mode(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
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
			std::string modeMsg = ":" + client.getNickname() + " MODE " + target;
			for (size_t i = 1; i < params.size(); ++i) {
				modeMsg += " " + params[i];
			}
			modeMsg += "\r\n";
			broadcastToChannel(channel, modeMsg, clients, -1);
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
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
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
	std::string kickMsg = ":" + client.getNickname() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
	broadcastToChannel(channel, kickMsg, clients, -1);
	channel.kickUser(targetSocket);
}

void Commands::invite(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
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
	std::string inviteMsg = ":" + client.getNickname() + " INVITE " + targetNick + " " + channelName + "\r\n";
	targetClient->sendMessage(inviteMsg);
}

void Commands::pass(int clientSocket, const IRCMessage& msg, ClientManager& clients, const std::string& serverPassword) {
	Client& client = clients.getClientFromSocket(clientSocket);
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
}

void Commands::who(int clientSocket, const IRCMessage& msg, ClientManager& clients) {
	Client& client = clients.getClientFromSocket(clientSocket);
	if (!client.getRegistered()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		return;
	}
	const std::vector<std::string>& params = msg.getParams();
	std::vector<Client*> allClients = clients.getAllClients();

	std::string searchMask = (params.size() > 0) ? params[0] : "*";
	std::string::size_type pos = searchMask.find('*');

	for (std::vector<Client*>::iterator it = allClients.begin(); it != allClients.end(); ++it) {
		Client* target = *it;
		std::string nick = target->getNickname();
		bool isMatch = false;

		if (pos == std::string::npos) {
			if (nick == searchMask) {
				isMatch = true;
			}
		} else {
			std::string prefix = searchMask.substr(0, pos);
			std::string suffix = "";

			if (pos + 1 < searchMask.size()) {
				suffix = searchMask.substr(pos + 1);
			}
			if (nick.length() >= prefix.length() + suffix.length()) {
				if (nick.substr(0, prefix.length()) == prefix &&
					nick.substr(nick.length() - suffix.length()) == suffix) {
					isMatch = true;
				}
			}
		}

		if (isMatch) {
			std::stringstream ss;
			ss << 0;
			std::string hopcount = ss.str();
			client.sendMessage(ReplyMessage::rplWhoReply(
				"*", target->getUsername(), "ft_irc", "ft_irc", nick, "H", hopcount, "realname"
			));
		}
	}
	client.sendMessage(ReplyMessage::rplEndOfWho(searchMask));
}

void Commands::nick(int clientSocket, const IRCMessage& msg, ClientManager& clients) {
	Client &client = clients.getClientFromSocket(clientSocket);
	if (!client.getAuth()) {
		client.sendMessage("Error : Need to authenticate first\r\n");
		return;
	}
	if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
		return ;
	if (msg.getParams().empty()) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("NICK"));
		return;
	}
	std::string newNick = msg.getParams()[0];
	if (newNick.empty()) {
		client.sendMessage(ReplyMessage::errErroneusNuckname(newNick));
		return;
	}
	if (clients.isNicknameUsed(newNick)){
		client.sendMessage(ReplyMessage::errNicknameInUse(newNick));
		return ;
	}
	client.setNickname(newNick);
	if (!client.getUsername().empty() && client.getAuth()){
		client.setRegistered(true);
	}
}

void Commands::user(int clientSocket, const IRCMessage& msg, ClientManager& clients) {
	Client &client = clients.getClientFromSocket(clientSocket);
	if (!client.getAuth()) {
		client.sendMessage("Error : Need to authenticate first\r\n");
		return;
	}
	if (client.getRegistered()) {
		client.sendMessage(ReplyMessage::errAlreadyRegistered());
		return;
	}
	if ((msg.getParams().size() < 1 || msg.getParams().size() > 3) || msg.getTrailing().empty()) {
		client.sendMessage(ReplyMessage::errNeedMoreParams("USER"));
		return;
	}
	std::string newUser = msg.getParams()[0];
	if (clients.isUsernameUsed(newUser) && client.getUsername() != newUser)
		return ;
	client.setUsername(newUser);
	if (!client.getNickname().empty() && client.getAuth()){
		client.setRegistered(true);
	}
}

void Commands::privmsg(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients){
	try {
		Client &client = clients.getClientFromSocket(clientSocket);
		if (!client.getRegistered()) {
			client.sendMessage(ReplyMessage::errNotRegistered());
			return;
		}
		if (!msg.getPrefix().empty() && msg.getPrefix() != client.getNickname())
			return ;
		if (msg.getParams().empty()) {
			client.sendMessage(ReplyMessage::errNoRecipient(msg.getCommand()));
			return ;
		}
		if (msg.getTrailing().empty()) {
			client.sendMessage(ReplyMessage::errNoTextToSend());
			return ;
		}
		std::vector<std::string> tmp = paramHandler(msg.getParams()[0]);
		if (tmp.size() > 4) {
			client.sendMessage(ReplyMessage::errTooManyTargets(tmp[0]));
			return ;
		}
		std::vector<std::string>::iterator it = tmp.begin();
		std::vector<std::string>::iterator ite = tmp.end();
		std::string privMsgBase = ":" + client.getNickname() + " PRIVMSG ";
		for (; it != ite; ++it ) {
			std::string privMsg = privMsgBase + *it + " :" + msg.getTrailing() + "\r\n";
			if (channels.hasChannel(*it) && channels.getChannelFromName(*it).hasUser(client.getSocket()))
				broadcastToChannel(channels.getChannelFromName(*it), privMsg, clients, clientSocket);
			else if (clients.isNicknameUsed(*it))
				send(clients.getClientFromNickname(*it).getSocket(), privMsg.c_str(), privMsg.size(), 0);
			else {
				client.sendMessage(ReplyMessage::errNoSuchNick(*it));
			}
		}
	}
	catch(const std::exception& ) {
	}
}