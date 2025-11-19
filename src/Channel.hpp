/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/19 11:13:17 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <map>
#include <set>

#define ADMIN
#define USER

class Channel {
protected:
  std::map<int, Client *> _users;
  std::map<int, Client *> _admins;
  std::map<int, Client *> _invitedUsers;
  std::set<char> _mode;
  std::string _topic;
  std::string _pswrd;
  std::string _name;
  int _maxCapacity;
  bool _modeSet;

  Channel(void);
  Channel(const Channel &);
  Channel &operator=(const Channel &);

public:
  Channel(Client &);
  Channel(Client &, int capacity);
  ~Channel(void);

  std::string getTopic() const;
  std::string getPswrd() const;
  std::string getName() const;
  int getCapacity() const;
  bool getModeSet() const;
  std::map<int, Client *> getInvitedUsers();

  void setTopic(std::string newTopic);
  void setPswrd(std::string newPswrd);
  void setName(std::string newName);
  void setCapacity(int newCapacity);
  void setModeSet(bool changeMode);

  void newUser(Client &);
  void tryJoin(const Client &, std::string pswrd);
  void updatePriv(const Client &admin, Client &user);

  // modifier les try catch pour les encapsuler dans les cpp des channel et des
  // commandes
  class maxCapacityReached : public std::exception {
    const char *what() const throw();
  };

  class invitationNeeded : public std::exception {
    const char *what() const throw();
  };

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
};

#endif //! CHANNEL_HPP
