/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:21:10 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 16:25:10 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __USER_HPP__
#define __USER_HPP__

#include "net/Connection.hpp"
#include <string>

/**
 * @brief A light subclass of Connection that stores basic IRC user fields.
 *
 * The class adds nick/user/realname and registration state. It does not
 * implement full IRC parsing — that is intended for your Dispatcher.
 */
class User : public Connection {
public:
  User(int fd, Reactor *reactor, ConnectionManager *mgr);
  virtual ~User();

  /** Set / get nick */
  void setNick(const std::string &nick);
  const std::string &nick() const;

  /** Set / get username (USER command) */
  void setUsername(const std::string &u);
  const std::string &username() const;

  /** Set / get real name */
  void setRealname(const std::string &r);
  const std::string &realname() const;

  /** Registration state */
  void setRegistered(bool v);
  bool isRegistered() const;

private:
  std::string m_nick;
  std::string m_username;
  std::string m_realname;
  bool m_registered;
};

#endif // __USER_HPP__
