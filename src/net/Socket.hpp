/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:11:37 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 16:32:39 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**
 * @file Socket.hpp
 * @brief Thin platform-specific socket wrapper.
 */

#include <string>
#include <sys/types.h>

/**
 * @brief Lightweight wrapper around a raw socket fd.
 *
 * Responsibilities:
 * - create / bind / listen sockets
 * - accept incoming connections
 * - manage fd lifetime
 */
class Socket {
public:
  /**
   * @brief Construct an invalid socket.
   */
  Socket();

  /**
   * @brief Construct from existing file descriptor (takes ownership).
   * @param fd file descriptor
   */
  explicit Socket(int fd);

  /**
   * @brief Destructor closes the fd if valid.
   */
  ~Socket();

  /**
   * @brief Create a listening TCP socket bound to the given address and port.
   * @param addr IP address to bind (use "0.0.0.0" for all interfaces)
   * @param port TCP port
   * @return true on success
   */
  bool createAndBind(const std::string &addr, unsigned short port);

  /**
   * @brief Start listening on the bound socket.
   * @param backlog listen backlog
   * @return true on success
   */
  bool listen(int backlog = 128);

  /**
   * @brief Accept a connection in non-blocking mode.
   * @return file descriptor of accepted socket, or -1 on error.
   */
  int accept();

  /**
   * @brief Close the underlying socket.
   */
  void close();

  /**
   * @brief Get the raw file descriptor.
   */
  int fd() const;

  /**
   * @brief Set the socket to non-blocking mode.
   */
  bool setNonBlocking(bool nonBlocking);

  /**
   * @brief Set SO_REUSEADDR on the socket.
   */
  bool setReuseAddr(bool on = true);

  /**
   * @brief Set TCP_NODELAY on the socket (disable Nagle).
   */
  bool setNoDelay(bool on = true);

  /**
   * @brief Take ownership of an fd (close existing first).
   */
  void attach(int fd);

  /**
   * @brief Release ownership and return fd. Caller becomes responsible.
   */
  int detach();

  /**
   * @brief Whether the socket currently holds a valid fd.
   */
  bool valid() const;

private:
  int m_fd;
};
