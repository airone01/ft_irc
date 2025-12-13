/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:23:02 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/08 10:32:43 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "Reactor.hpp"

#include "Listener.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdexcept>
#include <unistd.h>

Connection::Connection()
	: _fd(-1),  _manager(NULL), _readBuf(), _writeBuf(),
	   _closed(false), _lastActivity(std::time(NULL)) {}

Connection::Connection(const Connection &other)
	: _fd(other._fd), _manager(other._manager),
		_readBuf(other._readBuf), _writeBuf(other._writeBuf),
		_closed(other._closed),
		_lastActivity(other._lastActivity) {}

Connection::Connection(int fd, Reactor *reactor, ConnectionManager *mgr)
	: _fd(fd), _manager(mgr), _readBuf(), _writeBuf(),
	   _closed(false), _lastActivity(std::time(NULL)) {}

Connection::~Connection() {
  // this should be safe even if closed previously
  if (_fd >= 0)
	::close(_fd);
}

Connection &Connection::operator=(const Connection &other) {
  if (this != &other) {
	this->_fd = other._fd;
	this->_manager = other._manager;
	this->_readBuf = other._readBuf;
	this->_writeBuf = other._writeBuf;
	this->_closed = other._closed;
	this->_lastActivity = other._lastActivity;
  }
  return (*this);
}

int Connection::getFd() const { return _fd; }

void Connection::touch() { _lastActivity = std::time(NULL); }

std::time_t Connection::getLastActivity() const { return _lastActivity; }

/**
 * @note the conditions in this function need to be executed in the specific
 * order they are as of this commit, otherwise what could happen is if the
 * function catches EPOLLRDHUP upon entering the function, the fd is destroyed
 * immediately, discarding the receive buffer and never processing the rest of
 * the packages.
 */
void Connection::handleEvent(uint32_t events) {
  if (_closed)
	return;

  if (events & EPOLLIN) {
	if (handleRead() <= 0)
	  // handleRead calls close(), which now sets _disconnecting if writeBuf is
	  // not empty. We should return here to stop processing this event loop
	  // iteration.
	  return;
  }

  if (events & EPOLLOUT) {
	handleWrite(); // this will close() if buffer becomes empty and
				   // _disconnecting is true
  }

  if (events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
	close(); // if it's a hard error (HUP/ERR), we might want to force close,
			 // but standard RDHUP (half-close) should be graceful.
	return;
  }
}

ssize_t Connection::handleRead() {
  char buf[4096];
  bool keepReading = true;
  ssize_t totalRead = 0;
  int loopCount = 0;

  if (_fd < 0)
	return -1;

  while (keepReading) {
	loopCount++;
	ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);

	if (n > 0) {
	  _lastActivity = std::time(NULL);
	  _readBuf.insert(_readBuf.end(), buf, buf + n);
	  totalRead += n;

	  if (static_cast<size_t>(n) < sizeof(buf)) {
		keepReading = false;
	  }

	  if (loopCount > 50) {
		keepReading = false;
	  }

	} else if (n == 0) {
	  close();
	  return 0; // Signals handleEvent to stop
	} else {
	  if (errno == EAGAIN || errno == EWOULDBLOCK) {
		keepReading = false;
	  } else {
		perror("recv");
		close();
		return -1; // Signals handleEvent to stop
	  }
	}
  }

  return totalRead;
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

void Connection::close() {
  if (_closed)
	return;
  // if we have data available, mark as dc'ing but don't close yet
  if (!_writeBuf.empty()) {
	_disconnecting = true;
	return;
  }
  _closed = true;
  if (_fd >= 0)
	::close(_fd);
  _fd = -1;
  // remove from manager last
  // this triggers 'delete this', so we must not touch any member variables
  // after this line.
  if (_manager)
	_manager->remove(this);
}

int setSock(){
	/*
	Note d'erwann
	Apparement bind peut fail, et on doit le faire tourner en boucle sur toutes les configurations
	generees par getaddrinfo jusqu'a ce qu'on reussisse a bind.
	Ca a l'air d'etre le flow general de bind.

	Note 2: On ne peut pas utiliser `abort()`, qui est cense etre call quand getaddrinfo fail: forbidden function
	Note 3: exemple sur https://stackoverflow.com/a/52728208
	Note 4: je suis en aquarium
	*/

	int socketFd = ::socket(AF_INET, SOCK_NONBLOCK, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htonl(0);
	// ::bind(socketFd, reinterpret_cast<struct sockaddr *>(&sin), sizeof(socketFd));
	::bind(socketFd, (struct sockaddr*)&sin, sizeof(socketFd));
	::listen(socketFd, 1024);
	return socketFd;
}

void server(sockaddr_in sin) {

	int maxEvents = 1024;
	epoll_event ev, events[maxEvents];
	ev.events = EPOLLIN;
	int epfd = epoll_create1(0);
	if (epfd < 0)
		throw std::runtime_error("epoll_create1 failed.");

		
	int socketFd = setSock();
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, socketFd, &ev) == -1)
					throw std::runtime_error("wpoll_wait failed");

	for(;;){
		int nfds = epoll_wait(epfd, events, maxEvents, -1);
		if (nfds < 0) {
			// TODO: handle error for real
			throw std::runtime_error("epoll_wait failed");
		}
		for (int i = 0; i < nfds; ++i) {
			unsigned int len = sizeof(socketFd);
			// si le fd est le meme que celui de listen, alors c'est un nv client
			if (events[i].data.fd == socketFd){
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
				if (events[i].events == EPOLLIN)
					read();
				else if (events[i].events == EPOLLOUT)
					write();
				// en gros, recv et call le dispatcher depuis ici
				// read();

			}
		}
	} 
}