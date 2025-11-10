/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:22:07 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/10 16:22:09 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int fd, Reactor *reactor, ConnectionManager *mgr)
    : Connection(fd, reactor, mgr), m_nick(), m_username(), m_realname(),
      m_registered(false) {}

User::~User() {}

void User::setNick(const std::string &nick) { m_nick = nick; }
const std::string &User::nick() const { return m_nick; }

void User::setUsername(const std::string &u) { m_username = u; }
const std::string &User::username() const { return m_username; }

void User::setRealname(const std::string &r) { m_realname = r; }
const std::string &User::realname() const { return m_realname; }

void User::setRegistered(bool v) { m_registered = v; }
bool User::isRegistered() const { return m_registered; }
