/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:08:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 16:32:30 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Connection.hpp"
#include "EventHandler.hpp"
#include "Socket.hpp"
#include <string>

class Reactor;
class ConnectionManager;

/**
 * @brief Listener accepts new clients and registers them with the Reactor and
 * ConnectionManager.
 */
class Listener : public EventHandler {
public:
  typedef Connection *(*ConnectionFactory)(int fd, Reactor *reactor,
                                           ConnectionManager *mgr);

  /**
   * @brief Construct a listener bound to address/port.
   */
  Listener(const std::string &addr, unsigned short port, Reactor *reactor,
           ConnectionManager *cm);

  /**
   * @brief Destructor.
   */
  virtual ~Listener();

  /**
   * @brief Start listening (bind + listen) and register with reactor.
   * @return true on success
   */
  bool start();

  /**
   * @brief EventHandler implementation.
   */
  virtual void handleEvent(uint32_t events);

  /**
   * @brief Set a default message callback that will be installed on each
   * accepted Connection.
   */
  void setDefaultMessageCallback(Connection::MessageCallback cb);

  /**
   * @brief Set a factory used to construct Connection-derived objects for each
   * accepted fd.
   *
   * If not set, Listener will create plain Connection objects.
   */
  void setConnectionFactory(ConnectionFactory f);

private:
  Socket m_socket;
  std::string m_addr;
  unsigned short m_port;
  Reactor *m_reactor;
  ConnectionManager *m_connManager;

  Connection::MessageCallback m_defaultMsgCb;
  ConnectionFactory m_factory;
};
