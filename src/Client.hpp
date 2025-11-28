/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:57:04 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/28 17:33:59 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "net/Connection.hpp"

/**
 * @brief A light subclass of Connection that stores basic IRC user fields.
 *
 * The class adds nick/user/realname and registration state. It does not
 * implement full IRC parsing — that is intended for your Dispatcher.
 */
class Client : public Connection {
private:
  int _socket;
  std::string _nickname;
  std::string _username;
  std::string _hostname;
  std::string _buffer;
  bool _auth;
  bool _registered;
  Client();
  Client &operator=(const Client &other);

public:
  Client(const Client &copy);
  Client(int fd, Reactor *reactor, ConnectionManager *mgr);
  virtual ~Client();
  // getter
  int getSocket() const;
  std::string getNickname() const;
  std::string getUsername() const;
  std::string getHostname() const;
  bool getAuth() const;
  bool getRegistered() const;
  // setter
  void setNickname(const std::string &nick);
  void setUsername(const std::string &user);
  void setAuth(bool auth);
  void setRegistered(bool reg);
  // method
  void appendToBuffer(const std::string &buffer);
  std::string extractMessage();
};

#endif // !CLIENT_HPP
