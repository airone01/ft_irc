/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:22:32 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 20:36:36 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Logger.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

Socket::Socket() : _fd(-1), _is_connected(false) {
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd == -1) {
    throw std::runtime_error("Failed to create socket");
  }
}

Socket::~Socket() {
  if (_fd != -1) {
    ::close(_fd);
  }
}

void Socket::bind(int port) {
  if (_fd == -1) {
    throw std::runtime_error("Socket not created");
  }

  struct sockaddr_in serv_addr;
  std::memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(static_cast<unsigned short>(port));

  if (::bind(_fd, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) <
      0) {
    throw std::runtime_error("Bind failed");
  }
}

void Socket::listen(int backlog) {
  if (_fd == -1) {
    throw std::runtime_error("Socket not created");
  }

  if (::listen(_fd, backlog) < 0) {
    throw std::runtime_error("Listen failed");
  }
}

Socket *Socket::accept() {
  if (_fd == -1) {
    throw std::runtime_error("Socket not created");
  }

  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);
  int newsockfd =
      ::accept(_fd, reinterpret_cast<sockaddr *>(&cli_addr), &clilen);
  if (newsockfd < 0) {
    throw std::runtime_error("Accept failed");
  }

  Socket *new_socket = new Socket();
  new_socket->_fd = newsockfd;
  new_socket->_is_connected = true;

  logger::debug() << "Accepted new connection." << std::endl;
  return new_socket;
}

void Socket::send(const std::string &data) {
  if (_fd == -1 || !_is_connected) {
    throw std::runtime_error("Socket not connected");
  }

  int n = static_cast<int>(::send(_fd, data.c_str(), data.size(), 0));
  if (n < 0) {
    throw std::runtime_error("Send failed");
  }
}

std::string Socket::recv(int max_size) {
  if (_fd == -1 || !_is_connected) {
    throw std::runtime_error("Socket not connected");
  }

  std::vector<char> buffer(max_size);
  int n = static_cast<int>(::recv(_fd, buffer.data(), max_size, 0));
  if (n < 0) {
    throw std::runtime_error("Recv failed");
  } else if (n == 0) {
    _is_connected = false;
    return "";
  }

  return std::string(buffer.begin(), buffer.begin() + n);
}

void Socket::close() {
  if (_fd == -1) {
    logger::warning() << "Tried to close a socket with an fd of -1."
                      << std::endl;
  } else {
    int old_fd = _fd;
    ::close(_fd);
    _fd = -1;
    _is_connected = false;
    logger::debug() << "Closed socket with fd " << old_fd << "." << std::endl;
  }
}

int Socket::get_fd() const { return _fd; }
