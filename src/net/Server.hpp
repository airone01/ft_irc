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

class Server{
	
	private:

	ClientManager _clients;
	ChannelManager _channels;
	int	_socketFd;

	public:

	Server( void );
	~Server();

	void setSock();
	void server(sockaddr_in sin);

	void handleEvent(int);
	std::string handleRead(int fd);
	void executeCommand(Client &client, const std::string &line);
};

#endif