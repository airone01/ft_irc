/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:10:50 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/28 17:35:41 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <sys/types.h>
#include <vector>

#include "ConnectionManager.hpp"
#include "EventHandler.hpp"
#include "Reactor.hpp"

/**
 * @brief Connection represents per-client I/O state and logic.
 */
class Connection : public EventHandler {
public:
  /**
   * @brief Message callback type. Implemented by external dispatcher.
   * @param conn Connection pointer
   * @param data Received bytes (not nul-terminated)
   */
  typedef void (*MessageCallback)(Connection *conn,
                                  const std::vector<char> &data);

  Connection();
  Connection(const Connection &);
  Connection(int fd, Reactor *reactor, ConnectionManager *mgr);
  virtual ~Connection();
  Connection &operator=(const Connection &);

  /**
   * @brief Handle epoll events for this connection.
   */
  virtual void handleEvent(uint32_t events);

  /**
   * @brief Attempt to read until EAGAIN. Appends to internal read buffer and
   * calls message callback if set.
   */
  ssize_t handleRead();

  /**
   * @brief Attempt to write as much of the write buffer as possible.
   */
  ssize_t handleWrite();

  /**
   * @brief Queue bytes for sending.
   */
  void send(const std::vector<char> &data);

  /**
   * @brief Close connection immediately.
   */
  void close();

  /**
   * @brief Set message callback (dispatcher provided by peer).
   */
  void setMessageCallback(MessageCallback cb);

  /**
   * @brief Update last-activity timestamp to now.
   */
  void touch();

  /**
   * @brief Get raw fd.
   */
  int getFd() const;

  /**
   * @brief Get last-activity timestamp.
   */
  time_t getLastActivity() const;

protected:
  int _fd;
  Reactor *_reactor;
  ConnectionManager *_manager;
  std::vector<char> _readBuf;
  std::vector<char> _writeBuf;
  MessageCallback _msgCb;
  bool _closed;
  time_t _lastActivity;
};

#endif // !CONNECTION_HPP
