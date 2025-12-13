#include "Server.hpp"

Server::Server( void ){

}

Server::~Server(){

}

void Server::handleEvent(int fd){
	const std::string &msg = handleRead(fd);
	executeCommand(_clients.getClientFromSocket(fd) ,msg);
}

void Server::executeCommand(Client &client, const std::string &line) {
  try {
    IRCMessage msg(line); // Parser modifies string temporarily?
    std::string cmd = msg.getCommand();

    if (cmd.empty()) {
      // logger::warning() << "Caught an empty command." << std::endl;
      return ;
    }

    if (cmd == "NICK") {
      Commands::nick(msg, _clients, client);
    } else if (cmd == "USER") {
      Commands::user(msg, client);
    } else if (cmd == "QUIT") {
      client.close();
	  //todo create quit cmd
    } else if (cmd == "CAP") {
      Commands::cap(msg, client);
    } else if (cmd == "VERSION") {
      Commands::version(client);
    } else if (cmd == "JOIN") {
      if (client.getRegistered())
        Commands::join(msg, _channels, client);
    } else if (cmd == "PRIVMSG") {
      if (client.getRegistered()) {
        Commands::privmsg(msg, client, _clients, _channels);
      }
    } else if (cmd == "MODE") {
      Commands::mode(msg, _channels, client, _clients);
    } else if (cmd == "PING") {
      std::string token = msg.getParams().empty() ? "" : msg.getParams()[0];
      std::string pong = "PONG " + token + "\r\n";
      std::vector<char> r(pong.begin(), pong.end());
      client.send(r);
    } else {
		std::string err = "Unknown command: " + cmd;
		::send(client.getSocket(), err.c_str(), err.size(), -1);
    }

  } catch (std::exception &e) {
		send(client.getSocket(), e.what(), sizeof(e.what()), -1);
  }
}


std::string Server::handleRead(int fd) {
	
	bool keepReading = true;
	int loopCount = 0;

	std::string	readBuff = _clients.getClientFromSocket(fd).getRbuff();

	if (fd < 0)
		return ;

	while (keepReading)
	{
		char buf[4096];
		loopCount++;
		ssize_t n = ::recv(fd, buf, sizeof(buf), 0);

		if (n > 0)
		{
			readBuff.append(buf);
			// readBuff.insert(readBuff.end(), buf, buf + n);

			if (static_cast<size_t>(n) < sizeof(buf))
				keepReading = false;

			if (loopCount > 50)
				keepReading = false;
		}
		else
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				keepReading = false;
			else
				// handle error
				return ; // Signals handleEvent to stop
		}

	}
	return readBuff;
}

ssize_t Connection::handleWrite() {
  if (_fd < 0)
	return -1;

  while (!_writeBuf.empty()) {
	ssize_t n = ::send(_fd, &_writeBuf[0], _writeBuf.size(), MSG_NOSIGNAL);
	if (n > 0) {
	  _lastActivity = std::time(NULL);
	  if (static_cast<size_t>(n) >= _writeBuf.size()) {
		_writeBuf.clear();
		return n;
	  } else {
		_writeBuf.erase(_writeBuf.begin(), _writeBuf.begin() + n);
	  }
	} else {
	  if (errno == EAGAIN || errno == EWOULDBLOCK) {
		return 0;
	  }
	  perror("send");
	  close();
	  return -1;
	}
  }
  if (_writeBuf.empty() && _disconnecting) {
	close();
  }
  return 0;
}

// overload for convenience
void Connection::send(const std::string &data) {
  std::vector<char> char_data(data.begin(), data.end());
  this->send(char_data);
}

void Connection::send(const std::vector<char> &data) {
  if (_closed || data.empty())
	return;

  ssize_t sent = 0;

  if (_writeBuf.empty()) {
	sent = ::send(_fd, &data[0], data.size(), MSG_NOSIGNAL);

	if (sent < 0) {
	  if (errno == EAGAIN || errno == EWOULDBLOCK) {
		sent = 0;
	  } else {
		perror("send");
		close();
		return;
	  }
	} else {
	  _lastActivity = std::time(NULL);
	}
  }

  if (static_cast<size_t>(sent) < data.size())
	_writeBuf.insert(_writeBuf.end(), data.begin() + sent, data.end());
}

void Server::setSock(){
	/*
	Note d'erwann
	Apparement bind peut fail, et on doit le faire tourner en boucle sur toutes les configurations
	generees par getaddrinfo jusqu'a ce qu'on reussisse a bind.
	Ca a l'air d'etre le flow general de bind.

	Note 2: On ne peut pas utiliser `abort()`, qui est cense etre call quand getaddrinfo fail: forbidden function
	Note 3: exemple sur https://stackoverflow.com/a/52728208
	Note 4: je suis en aquarium
	*/

	this->_socketFd = ::socket(AF_INET, SOCK_NONBLOCK, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htonl(0);
	// ::bind(socketFd, reinterpret_cast<struct sockaddr *>(&sin), sizeof(socketFd));
	::bind(this->_socketFd, (struct sockaddr*)&sin, sizeof(this->_socketFd));
	::listen(this->_socketFd, 1024);
}

void Server::server(sockaddr_in sin) {

	int maxEvents = 1024;
	epoll_event ev, events[maxEvents];
	ev.events = EPOLLIN;
	int epfd = epoll_create1(0);
	if (epfd < 0)
		throw std::runtime_error("epoll_create1 failed.");

		
	
	this->setSock();
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->_socketFd, &ev) == -1)
					throw std::runtime_error("wpoll_wait failed");

	for(;;){
		int nfds = epoll_wait(epfd, events, maxEvents, -1);
		if (nfds < 0) {
			// TODO: handle error for real
			throw std::runtime_error("epoll_wait failed");
		}
		for (int i = 0; i < nfds; ++i) {
			unsigned int len = sizeof(this->_socketFd);
			// si le fd est le meme que celui de listen, alors c'est un nv client
			if (events[i].data.fd == this->_socketFd){
				int connSock = accept(events[i].data.fd, (struct sockaddr*)&sin, &len);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connSock; 
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, connSock, &ev) == -1){
					throw std::runtime_error("wpoll_wait failed");
					//todo : check this error handler
				}
				// new connection en gros
			}
			else {
				this->handleEvent();
				// if (events[i].events == EPOLLIN)
				// 	recv();
				// en gros, recv et call le dispatcher depuis ici
				// read();

			}
		}
	} 
}