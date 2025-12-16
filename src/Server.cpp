#include "Server.hpp"

// Server::Server( void ) :  _port(NULL), _pswrd(NULL) {}

Server::Server( int port, std::string pswrd ) :  _pswrd(pswrd), _port(port) {

}

Server::~Server(){

}

CMDS applyCommands(const std::string& cmd) {
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
	return NONE;
}

void Server::handleEvent(int clientSocket){
	const std::string tmp = handleRead(clientSocket);
	if (!tmp.empty()){
		IRCMessage msg(tmp);
		execute(clientSocket, msg, this->_pswrd);
	}

}

void Server::execute(int clientSocket, const IRCMessage &msg, const std::string &serverPassword) {
	Client &client = this->_clients.getClientFromSocket(clientSocket);
	std::string cmd = msg.getCommand();
	CMDS command = applyCommands(cmd);
	switch (command) {
		case PASS:
			Commands::pass(clientSocket, msg, this->_clients, serverPassword);
			break;
		case QUIT:{
			Commands::quit(clientSocket);
			this->rmClient(clientSocket);
			break;
		}
		default:
			break;
	}
	if (!client.getAuth()) {
		client.sendMessage(ReplyMessage::errPasswdMismatch());
		return;
	}
	switch (command) {
		case NICK:
			Commands::nick(clientSocket, msg, _clients);
			break;
		case USER:
			Commands::user(clientSocket, msg, _clients);
			break;
		default:
			break;
	}
	if (!client.getRegistered() && !client.getAuth()) {
		client.sendMessage(ReplyMessage::errNotRegistered());
		client.clearBuffer();
		return;
	}
// 	switch (command) {
// 		case JOIN:
// 			join(clientSocket, msg, channels, clients);
// 			break;
// 		case PART:
// 			part(clientSocket, msg, channels, clients);
// 			break;
// 		case TOPIC:
// 			topic(clientSocket, msg, channels, clients);
// 			break;
// 		case MODE:
// 			mode(clientSocket, msg, channels, clients);
// 			break;
// 		case KICK:
// 			kick(clientSocket, msg, channels, clients);
// 			break;
// 		case INVITE:
// 			invite(clientSocket, msg, channels, clients);
// 			break;
// 		case PRIVMSG:
// 			privmsg(clientSocket, msg, channels, clients);
// 			break;
// 		case NOTICE:
// 			notice(clientSocket, msg, channels, clients);
// 			break;
// 		case PING:
// 			ping(clientSocket, msg, clients);
// 			break;
// 		case PONG:
// 			pong(clientSocket, msg, clients);
// 			break;
// 		case WHO:
// 			who(clientSocket, msg, channels, clients);
// 			break;
// 		case WHOIS:
// 			whois(clientSocket, msg, channels, clients);
// 			break;
// 		case LIST:
// 			list(clientSocket, msg, channels, clients);
// 			break;
// 		case NAMES:
// 			names(clientSocket, msg, channels, clients);
// 			break;
// 		case DEFAULT:
// 			client.sendMessage(ReplyMessage::errUnknownCommand(msg.getCommand()));
// 			break;
// 		default:
// 			break;
// 	}
	client.clearBuffer();
	std::cerr << "giga prout\n" << std::endl;
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
					close(fd);
					_clients.removeClient(fd);
					_channels.removeUserFromAllChannels(fd);
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

void	Server::rmClient(int clientSocket){
	int i = 0;
	while (clientSocket != this->events[i].data.fd)
		i++;
	close(this->events[i].data.fd);
	epoll_ctl(this->_epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
	_channels.removeUserFromAllChannels(this->events[i].data.fd);
	_clients.removeClient(this->events[i].data.fd);

}

void Server::serverRoutine() {

	int maxEvents = 1024;
	this->setSock();

	this->ev.events = EPOLLIN;
	this->ev.data.fd = this->_socketFd;
	this->_epfd = epoll_create1(0);
	if (this->_epfd < 0)
		throw std::runtime_error("epoll_create1 failed.");

	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_socketFd, &this->ev) == -1)
					throw std::runtime_error("wpoll_wait failed");

	for(;;){
		int nfds = epoll_wait(this->_epfd, this->events, maxEvents, -1);
		if (nfds < 0)
			throw std::runtime_error("epoll_wait failed");

		for (int i = 0; i < nfds; ++i) {

			if (this->events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
				std::cerr << "quit event\n";
				close(this->events[i].data.fd);
				epoll_ctl(this->_epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
				_channels.removeUserFromAllChannels(this->events[i].data.fd);
				_clients.removeClient(this->events[i].data.fd);
			}
			// si le fd est le meme que celui de listen, alors c'est un nv client
			else if (this->events[i].data.fd == this->_socketFd){
				sockaddr_in client;
				socklen_t len = sizeof(client);
				// int connSock = accept(events[i].data.fd, (struct sockaddr*)&client, &len);
				int connSock = accept(this->events[i].data.fd, reinterpret_cast<sockaddr*>(&client), &len);
				if (connSock < 0){
					std::cerr << "error: accept failed on fd " << this->events[i].data.fd << '\n' << std::endl;
					continue;
				}

				int oldflags = fcntl(this->events[i].data.fd, F_GETFL, 0);
				fcntl(connSock, F_SETFL, oldflags | O_NONBLOCK);
				this->ev.events = EPOLLIN;
				this->ev.data.fd = connSock;
				if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, connSock, &this->ev) == -1)
					throw std::runtime_error("epoll_ctl failed");
				_clients.addClient(Client(connSock));
			}
			else if (this->events[i].events & EPOLLIN) {
				handleEvent(this->events[i].data.fd);
			}
		}
	}
}
