#ifndef SERVER_HPP
# define SERVER_HPP

#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Commands.hpp"
#include <stdexcept>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sys/epoll.h>

class Server{

	private:

	ClientManager _clients;
	ChannelManager _channels;

	const std::string _pswrd;
	const int _port;
	int	_socketFd;
	int _epfd;
	epoll_event ev;
	epoll_event events[1024];

	// Server( void );

	public:

	Server( int port, std::string pswrd );
	~Server();

	void setSock();
	void serverRoutine();

	void handleEvent(int);
	std::string handleRead(int fd);
	void	rmClient(int clientSocket);
	// ssize_t Server::handleWrite();
	void execute(int clientSocket, const IRCMessage &msg, const std::string &serverPassword);
};

enum CMDS {
	CAP,
	PASS,
	QUIT,
	NICK,
	USER,
	JOIN,
	PART,
	TOPIC,
	MODE,
	KICK,
	INVITE,
	PRIVMSG,
	NOTICE,
	PING,
	PONG,
	WHO,
	WHOIS,
	LIST,
	NAMES,
	NONE
};


CMDS applyCommands(const std::string& cmd);

#endif
