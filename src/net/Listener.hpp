/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:08:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 11:32:03 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_HPP
#define LISTENER_HPP

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

  Listener();
  Listener(const Listener &);
  Listener(const std::string &addr, unsigned short port, Reactor *reactor,
           ConnectionManager *cm);

  Listener &operator=(const Listener &);

  /**
   * @brief Destructor.
   */
  virtual ~Listener();

  /**
   * @brief Start listening (bind + listen) and register with reactor.
   * @throw runtime error
   */
  void start();

  /**
   * @brief Set a default message callback that will be installed on each
   * accepted Connection.
   */
  void setDefaultMessageCallback(Connection::MessageCallback cb);

private:
  Socket _socket;
  std::string _addr;
  unsigned short _port;

  Connection::MessageCallback _defaultMsgCb;
};

#endif // !LISTENER_HPP
