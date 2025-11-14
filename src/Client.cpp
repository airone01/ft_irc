/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:31:51 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 17:31:56 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>
#include <unistd.h>

Client::Client(int fd, Reactor *reactor, ConnectionManager *mgr): Connection(fd, reactor, mgr), _socket(fd), _nickname(""), _username(""), _hostname(""), _registered(false) {
}

Client &Client::operator=(const Client &other) {
	if (this != &other) {
		this->_socket = other._socket;
		this->_nickname = other._nickname;
		this->_username = other._username;
		this->_hostname = other._hostname;
		this->_buffer = other._buffer;
		this->_auth = other._auth;
		this->_registered = other._registered;
	}
	return (*this);
}

Client::~Client() {
}

int Client::getSocket() const {
	return (this->_socket);
}

std::string Client::getNickname() const {
	return (this->_nickname);
}

std::string Client::getUsername() const {
	return (this->_username);
}

std::string Client::getHostname() const {
	return (this->_hostname);
}

bool Client::getAuth() const {
	return (this->_auth);
}

bool Client::getRegistered() const {
	return (this->_registered);
}

void Client::setNickname(const std::string &nick) {
	this->_nickname = nick;
}

void Client::setUsername(const std::string &user) {
	this->_username = user;
}

void Client::setAuth(bool auth) {
	this->_auth = auth;
}

void Client::setRegistered(bool reg) {
	this->_registered = reg;
}

void Client::appendToBuffer(const std::string &buffer) {
  this->_buffer += buffer;
}
