#include "Server.hpp"

// Server::Server( void ) :  _port(NULL), _pswrd(NULL) {}

Server::Server( int port, std::string pswrd ) :  _pswrd(pswrd), _port(port) {

}

Server::~Server(){

}

CMDS applyCommands(const std::string& cmd) {
	if (cmd == "CAP")
		return CAP;
	if (cmd == "PASS")
		return PASS;
	if (cmd == "QUIT")
		return QUIT;
	if (cmd == "NICK")
		return NICK;
	if (cmd == "USER")
		return USER;
	if (cmd == "JOIN")
		return JOIN;
	if (cmd == "PART")
		return PART;
	if (cmd == "TOPIC")
		return TOPIC;
	if (cmd == "MODE")
		return MODE;
	if (cmd == "KICK")
		return KICK;
	if (cmd == "INVITE")
		return INVITE;
	if (cmd == "PRIVMSG")
		return PRIVMSG;
	if (cmd == "NOTICE")
		return NOTICE;
	if (cmd == "PING")
		return PING;
	if (cmd == "PONG")
		return PONG;
	if (cmd == "WHO")
		return WHO;
	if (cmd == "WHOIS")
		return WHOIS;
	if (cmd == "LIST")
		return LIST;
	if (cmd == "NAMES")
		return NAMES;
	return DEFAULT;
}

void Server::handleEvent(int clientSocket){
	const std::string& data = handleRead(clientSocket);
	if (data.empty())
		return;
	try {
		Client &client = _clients.getClientFromSocket(clientSocket);
		client.appendToBuffer(data);
		while (true) {
			std::string message = client.extractMessage();
			if (message.empty())
				break;
			try {
				IRCMessage msg(message);
				execute(clientSocket, msg, this->_pswrd);
				client.clearBuffer();
			} catch (const IRCMessage::MsgEmptyException&) {
			}
		}
	} catch (const ClientManager::ClientNotFound&) {
	}
}

void Server::execute(int clientSocket, const IRCMessage &msg, const std::string &serverPassword) {
	Client* clientPtr = NULL;
	try {
		clientPtr = &this->_clients.getClientFromSocket(clientSocket);
	} catch (const ClientManager::ClientNotFound&) {
		Client newClient(clientSocket);
		this->_clients.addClient(newClient);
		clientPtr = &this->_clients.getClientFromSocket(clientSocket);
	}
	Client& client = *clientPtr;
	const std::string& cmd = msg.getCommand();
	CMDS command = applyCommands(cmd);
	if (command == CAP) {
		const std::vector<std::string>& params = msg.getParams();
		if (!params.empty()) {
			if (params[0] == "LS") {
				client.sendMessage(":localhost CAP * LS :\r\n");
			} else if (params[0] == "REQ") {
				client.sendMessage(":localhost CAP * NAK :\r\n");
			}
		}
		return;
	}
	if (command == PASS) {
		Commands::pass(clientSocket, msg, this->_clients, serverPassword);
		return;
	}
	if (command == QUIT) {
		Commands::quit(clientSocket, this->_clients);
		return;
	}
	if (!client.getAuth()) {
		client.sendMessage(ReplyMessage::errPasswdMismatch());
		return;
	}
	// if (command == NICK) {
	//	 Commands::nick(clientSocket, msg, this->_clients);
	//	 return;
	// }
	// if (command == USER) {
	//	 Commands::user(clientSocket, msg, this->_clients);
	//	 return;
	// }
	// if (!client.getRegistered()) {
	// 	client.sendMessage(ReplyMessage::errNotRegistered());
	// 	return;
	// }
	switch (command) {
		case JOIN:
			Commands::join(clientSocket, msg, this->_channels, this->_clients);
			return;
		case PART:
			Commands::part(clientSocket, msg, this->_channels, this->_clients);
			return;
		case TOPIC:
			Commands::topic(clientSocket, msg, this->_channels, this->_clients);
			return;
		case MODE:
			Commands::mode(clientSocket, msg, this->_channels, this->_clients);
			return;
		case KICK:
			Commands::kick(clientSocket, msg, this->_channels, this->_clients);
			return;
		case INVITE:
			Commands::invite(clientSocket, msg, this->_channels, this->_clients);
			return;
		// case PRIVMSG:
		//	 Commands::privmsg(clientSocket, msg, this->_channels, this->_clients);
		//	 break;
		// case PING:
		//	 Commands::ping(clientSocket, msg, this->_clients);
		//	 break;
		default:
			client.sendMessage(ReplyMessage::errUnknownCommand(cmd));
			return;
	}
	client.clearBuffer();
}


std::string Server::handleRead(int fd) {
	char buf[4096];
	try
	{
		Client &client = _clients.getClientFromSocket(fd);
		std::string	&readBuff = client.getRbuff();
		for (;;)
		{
			ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
			if (n > 0)
				readBuff.append(buf, n);
			else if (n == 0){
				close(fd);
				_clients.removeClient(fd);
				_channels.removeUserFromAllChannels(fd);
				return std::string();
			}
			else
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					break;
				else {
					_clients.removeClient(fd);
					_channels.removeUserFromAllChannels(fd);
					close(fd);
					return std::string();
				}
			}
	
		}
		return readBuff;
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';		
	}
	
	return std::string();
}

// ssize_t Server::handleWrite() {
//   if (_fd < 0)
// 	return -1;

//   while (!_writeBuf.empty()) {
// 	ssize_t n = ::send(_fd, &_writeBuf[0], _writeBuf.size(), MSG_NOSIGNAL);
// 	if (n > 0) {
// 	  _lastActivity = std::time(NULL);
// 	  if (static_cast<size_t>(n) >= _writeBuf.size()) {
// 		_writeBuf.clear();
// 		return n;
// 	  } else {
// 		_writeBuf.erase(_writeBuf.begin(), _writeBuf.begin() + n);
// 	  }
// 	} else {
// 	  if (errno == EAGAIN || errno == EWOULDBLOCK) {
// 		return 0;
// 	  }
// 	  perror("send");
// 	  close();
// 	  return -1;
// 	}
//   }
//   if (_writeBuf.empty() && _disconnecting) {
// 	close();
//   }
//   return 0;
// }

// // overload for convenience
// void Connection::send(const std::string &data) {
//   std::vector<char> char_data(data.begin(), data.end());
//   this->send(char_data);
// }

// void Connection::send(const std::vector<char> &data) {
//   if (_closed || data.empty())
// 	return;

//   ssize_t sent = 0;

//   if (_writeBuf.empty()) {
// 	sent = ::send(_fd, &data[0], data.size(), MSG_NOSIGNAL);

// 	if (sent < 0) {
// 	  if (errno == EAGAIN || errno == EWOULDBLOCK) {
// 		sent = 0;
// 	  } else {
// 		perror("send");
// 		close();
// 		return;
// 	  }
// 	} else {
// 	  _lastActivity = std::time(NULL);
// 	}
//   }

//   if (static_cast<size_t>(sent) < data.size())
// 	_writeBuf.insert(_writeBuf.end(), data.begin() + sent, data.end());
// }

void Server::setSock(){

	int opt = 1;
	this->_socketFd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFd < 0)
		throw std::runtime_error("listen failed.");

	setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	int oldflags = fcntl(this->_socketFd, F_GETFL, 0);
	fcntl(this->_socketFd, F_SETFL, oldflags | O_NONBLOCK);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(static_cast<short>(this->_port));
	// if (::bind(this->_socketFd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	if (::bind(this->_socketFd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) < 0)
		throw std::runtime_error("bind failed.");
	if (::listen(this->_socketFd, 1024) < 0)
		throw std::runtime_error("listen failed.");
}

void	Server::rmClient(epoll_event event){
		close(event.data.fd);
		epoll_ctl(this->_epfd, EPOLL_CTL_DEL, event.data.fd, NULL);
		_channels.removeUserFromAllChannels(event.data.fd);
		_clients.removeClient(event.data.fd);
}

void Server::serverRoutine() {

	int maxEvents = 1024;
	struct epoll_event ev, events[maxEvents];
	this->setSock();

	ev.events = EPOLLIN;
	ev.data.fd = this->_socketFd;
	this->_epfd = epoll_create1(0);
	if (this->_epfd < 0)
		throw std::runtime_error("epoll_create1 failed.");

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_socketFd, &ev) == -1)
					throw std::runtime_error("wpoll_wait failed");

	for(;;){
		int nfds = epoll_wait(this->_epfd, events, maxEvents, -1);
		if (nfds < 0)
			throw std::runtime_error("epoll_wait failed");

		for (int i = 0; i < nfds; ++i) {

			if (events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
				std::cerr << "quit event\n";
				close(events[i].data.fd);
				epoll_ctl(this->_epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
				_channels.removeUserFromAllChannels(events[i].data.fd);
				_clients.removeClient(events[i].data.fd);
			}
			// si le fd est le meme que celui de listen, alors c'est un nv client
			else if (events[i].data.fd == this->_socketFd){
				sockaddr_in client;
				socklen_t len = sizeof(client);
				// int connSock = accept(events[i].data.fd, (struct sockaddr*)&client, &len);
				int connSock = accept(events[i].data.fd, reinterpret_cast<sockaddr*>(&client), &len);
				if (connSock < 0){
					std::cerr << "error: accept failed on fd " << events[i].data.fd << '\n' << std::endl;
					continue;
				}

				int oldflags = fcntl(events[i].data.fd, F_GETFL, 0);
				fcntl(connSock, F_SETFL, oldflags | O_NONBLOCK);
				ev.events = EPOLLIN;
				ev.data.fd = connSock;
				if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, connSock, &ev) == -1)
					throw std::runtime_error("epoll_ctl failed");
				_clients.addClient(Client(connSock));
			}
			else if (events[i].events & EPOLLIN) {
				handleEvent(events[i].data.fd);
			}
		}
	}
}
