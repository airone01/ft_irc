#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Server.hpp"
#include "ChannelManager.hpp"
#include "ClientManager.hpp"
#include "Client.hpp"
#include "ReplyMessage.hpp"
#include "IRCMessage.hpp"
#include <sstream>

class Commands {
private:
	static void broadcastToChannel(Channel& channel, const std::string& message, ClientManager& clients, int senderSocket);
	// static void	sendWelcome(Client& client);
	// static bool	isValidNickname(const std::string& nickname);
public:
	static void	join(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	static void	part(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	static void	topic(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	static void	mode(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	static void	kick(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	static void	invite(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	static void pass(int clientSocket, const IRCMessage& msg, ClientManager& clients, const std::string& serverPassword);
	static void nick(int clientSocket, const IRCMessage& msg, ClientManager& clients);
	static void user(int clientSocket, const IRCMessage& msg, ClientManager& clients);
	static void quit(int clientSocket);

	static void privmsg(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	// static void notice(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);

	// static void ping(int clientSocket, const IRCMessage& msg, ClientManager& clients);
	// static void pong(int clientSocket, const IRCMessage& msg, ClientManager& clients);

	// static void who(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	// static void whois(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	// static void list(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
	// static void names(int clientSocket, const IRCMessage& msg, ChannelManager& channels, ClientManager& clients);
};

#endif //! COMMANDS_HPP
