/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:11:37 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 20:36:25 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#define IRC_PORT 6697

#include <string>

class Socket {
private:
  int _fd;
  bool _is_connected;

public:
  /**
   * @throw std::runtime_error
   */
  Socket();
  ~Socket();

  /**
   * Binds to a port
   *
   * @param int port Port to bind to
   *
   * @throw std::runtime_error
   */
  void bind(int port);

  /**
   * Listen for entering connections
   *
   * @param int backlog Number of TCP connections before overloaded
   *
   * @throw std::runtime_error
   * @see https://cmlabs.co/en/seo-terms/backlog
   */
  void listen(int backlog = 5);

  /**
   * Accepts a new connection
   *
   * @throw std::runtime_error
   * @return Socket* New Socket for the client
   */
  Socket *accept();

  /**
   * Send data to a client
   *
   * @param const std::string& data Data to send
   *
   * @throw std::runtime_error
   */
  void send(const std::string &data);

  /**
   * Receive data from a client
   *
   * @param int max_size Maximum size of data to receive. Defaults to 1024.
   *
   * @throw std::runtime_error
   * @return std::string Data
   */
  std::string recv(int max_size = 1024);

  /**
   * Close the socket.
   */
  void close();

  /**
   * Gets the underlying file descriptor
   * This is used for more fine-grained control.
   *
   * @return int const File descriptor
   */
  int get_fd() const;
};

#endif // SOCKET_HPP
