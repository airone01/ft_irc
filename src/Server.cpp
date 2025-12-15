#include "Server.hpp"
#include <stdexcept>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

Server::Server( void ){

}

Server::~Server(){

}

void Server::handleEvent(int fd){
	const std::string msg = handleRead(fd);
	if (!msg.empty())
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

	char buf[4096];
	Client &client = _clients.getClientFromSocket(fd);
	std::string	&readBuff = client.getRbuff();

	for (;;)
	{
		ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
		if (n > 0)
			readBuff.append(buf, n);
		else if (n == 0){
			close(fd);
			// ici remove le client du manager
			return std::string();
		}
		else
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else {
				close(fd);
				// ici remove le client du manager
				return std::string();
			}
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

void Server::setSock(int port){

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
	sin.sin_port = htons(port);
	if (::bind(this->_socketFd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
		throw std::runtime_error("bind failed.");
	if (::listen(this->_socketFd, 1024) < 0)
		throw std::runtime_error("listen failed.");
}

void Server::server() {

	int maxEvents = 1024;
	struct epoll_event ev, events[maxEvents];
	this->setSock();

	ev.events = EPOLLIN;
	ev.data.fd = this->_socketFd;
	int epfd = epoll_create1(0);
	if (epfd < 0)
		throw std::runtime_error("epoll_create1 failed.");

	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->_socketFd, &ev) == -1)
					throw std::runtime_error("wpoll_wait failed");

	for(;;){
		int nfds = epoll_wait(epfd, events, maxEvents, -1);
		if (nfds < 0)
			throw std::runtime_error("epoll_wait failed");

		for (int i = 0; i < nfds; ++i) {

			if (events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
				close(events[i].data.fd);
				epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
			}
			// si le fd est le meme que celui de listen, alors c'est un nv client
			else if (events[i].data.fd == this->_socketFd){
				sockaddr_in client;
				socklen_t len = sizeof(client);
				int connSock = accept(events[i].data.fd, (struct sockaddr*)&client, &len);
				if (connSock < 0){
					std::cerr << "error: accept failed on fd " << events[i].data.fd << '\n' << std::endl;
					continue;
				}

				int oldflags = fcntl(fd, F_GETFL, 0);
				fcntl(connSock, F_SETFL, oldflags | O_NONBLOCK);
				ev.events = EPOLLIN;
				ev.data.fd = connSock;
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, connSock, &ev) == -1)
					throw std::runtime_error("epoll_ctl failed");
				// new connection en gros
			}
			else if (events[i].events & EPOLLIN) {
				handleEvent(events[i].data.fd);
			}
		}
	}
}
