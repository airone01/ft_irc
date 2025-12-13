/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:31:51 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/08 11:51:34 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>
#include <unistd.h>

Client::Client(int fd, Reactor *reactor, ConnectionManager *mgr)
    : Connection(fd, reactor, mgr), _socket(fd), _nickname(""), _username(""),
      _hostname(""), _rBuffer(""), _auth(false), _registered(false) {}

Client::Client(const Client &copy)
    : Connection(copy._socket, copy._manager),
      _socket(copy._socket), _nickname(copy._nickname),
      _username(copy._username), _hostname(copy._hostname),
      _rBuffer(copy._rBuffer), _auth(copy._auth), _registered(copy._registered) {}

Client &Client::operator=(const Client &other) {
  if (this != &other) {
    this->_socket = other._socket;
    this->_nickname = other._nickname;
    this->_username = other._username;
    this->_hostname = other._hostname;
    this->_rBuffer = other._rBuffer;
    this->_auth = other._auth;
    this->_registered = other._registered;
  }
  return (*this);
}

Client::~Client() {}

int Client::getSocket() const { return (this->_socket); }

std::string Client::getNickname() const { return (this->_nickname); }

std::string Client::getUsername() const { return (this->_username); }

std::string Client::getHostname() const { return (this->_hostname); }

std::string Client::getRbuff() const { return (this->_rBuffer); }

bool Client::getAuth() const { return (this->_auth); }

bool Client::getRegistered() const { return (this->_registered); }

void Client::setNickname(const std::string &nick) { this->_nickname = nick; }

void Client::setUsername(const std::string &user) { this->_username = user; }

void Client::setAuth(bool auth) { this->_auth = auth; }

void Client::setRegistered(bool reg) { this->_registered = reg; }

void Client::appendToBuffer(const std::string &buffer) {
  this->_rBuffer += buffer;
}

std::string Client::extractMessage() {
  size_t pos = _rBuffer.find("\n");
  if (pos == std::string::npos) {
    return "";
  }

  // Extract the line including the newline
  std::string line = _rBuffer.substr(0, pos + 1); // +1 to include \n

  // Remove extracted line from buffer
  _rBuffer.erase(0, pos + 1);

  // Trim \r if present (CRLF -> LF)
  if (!line.empty() && line[line.size() - 1] == '\n')
    line.erase(line.size() - 1);
  if (!line.empty() && line[line.size() - 1] == '\r')
    line.erase(line.size() - 1);

  return line;
}
