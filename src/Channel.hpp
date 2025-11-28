/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/28 17:28:06 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "ClientManager.hpp"
#include "IRCMessage.hpp"
#include <map>
#include <set>
#include <vector>

#define ADMIN
#define USER
// todo: add ERR_NONICKNAMEGIVEN, ERR_NOSUCHCHANNEL replies error check
class Channel {
private:
  std::map<int, Client *> _users;
  std::map<int, Client *> _admins;
  std::map<int, Client *> _invitedUsers;
  std::set<char> _mode;
  std::set<int> _kickedUsers;
  std::string _topic;
  std::string _pswrd;
  std::string _name;
  std::size_t _maxCapacity;
  bool _modeSet;

  Channel(void);

public:
  Channel(const Channel &);
  Channel &operator=(const Channel &);
  // Channel( Client & );
  // Channel( Client & ,int capacity );
  Channel(Client &, std::string name);
  ~Channel(void);

  std::string getTopic() const;
  std::string getPswrd() const;
  std::string getName() const;
  std::size_t getCapacity() const;
  bool getModeSet() const;
  std::map<int, Client *> getInvitedUsers();
  std::map<int, Client *> getUsers();

  std::set<char> getMode() const;
  std::set<int> getKickedUsers() const;

  void setTopic(const std::string newTopic);
  void setPswrd(const std::string newPswrd);
  void setName(const std::string newName);
  void setCapacity(const int newCapacity);
  void setModeSet(const bool changeMode);
  void setMode(const char c);
  void setKickedUsers(const int socket);
  void setInvitedUsers(Client &);

  void newUser(Client &);
  void tryJoin(const Client &, std::string pswrd);
  void tryKick(std::vector<std::string> param, IRCMessage const &tmp,
               Client &admin);
  void tryInvite(std::vector<std::string> param, ClientManager clients,
                 int adminSocket, int userSocket);
  void updatePriv(const Client &admin, Client &user);
  void leaveChannel(Client const &user);
  void changeTopic(IRCMessage const &tmp, Client const &user);
  void updateMode(IRCMessage const &tmp, Client const &user);

  class insufficientPrivilege : public std::exception {
    const char *what() const throw();
  };

  class invalidChannelName : public std::exception {
    const char *what() const throw();
  };

  class errorMode : public std::exception {
  private:
    std::string _errMsg;

  public:
    errorMode(std::string error) : _errMsg(std::string("error: ") + error) {}
    const char *what() const throw();
    ~errorMode() throw() {};
  };

  class errorKick : public std::exception {
  private:
    std::string _errMsg;

  public:
    errorKick(std::string error) : _errMsg(std::string("error: ") + error) {}
    const char *what() const throw();
    ~errorKick() throw() {};
  };

  class errorPart : public std::exception {
  private:
    std::string _errMsg;

  public:
    errorPart(std::string error) : _errMsg(std::string("error: ") + error) {}
    const char *what() const throw();
    ~errorPart() throw() {};
  };

  class errorInvite : public std::exception {
  private:
    std::string _errMsg;

  public:
    errorInvite(std::string error) : _errMsg(std::string("error: ") + error) {}
    const char *what() const throw();
    ~errorInvite() throw() {};
  };

  class errorTopic : public std::exception {
  private:
    std::string _errMsg;

  public:
    errorTopic(std::string error) : _errMsg(std::string("error: ") + error) {}
    const char *what() const throw();
    ~errorTopic() throw() {};
  };
};

#endif
