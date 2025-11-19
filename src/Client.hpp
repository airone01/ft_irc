/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:57:04 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/19 11:16:35 by elagouch         ###   ########.fr       */
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
public:
  Client(int fd, Reactor *reactor, ConnectionManager *mgr);
  // Client(const Client &copy); // copy constructor is too annoying to maintain
  Client &operator=(const Client &);
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
  void sendMessage(const std::string &buffer);

private:
  int _socket;
  std::string _nickname;
  std::string _username;
  std::string _hostname;
  std::string _buffer;
  bool _auth;
  bool _registered;
};

#endif // !CLIENT_HPP
