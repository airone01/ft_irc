/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:21:59 by nahamida          #+#    #+#             */
/*   Updated: 2025/11/16 10:30:48 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>

#include "Client.hpp"

#define ADMIN
#define USER

class Channel {
private:
  std::map<int, Client> _users;
  std::map<int, Client> _admins;
  std::map<int, Client> _invitedUsers;
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
  Channel(const Client &);
  Channel(const Client &, int capacity);
  ~Channel(void);

  // getters
  std::string getTopic() const;
  std::string getPswrd() const;
  std::string getName() const;
  int getCapacity() const;
  bool getInvite() const;

  // setters
  void setTopic(std::string newTopic);
  void setPswrd(std::string newPswrd);
  void setName(std::string newName);
  void setCapacity(int newCapacity);
  void setInvite(bool changeMode);

  void newUser(const Client &);
  /**
   * @brief Attempts to make a client join the channel
   */
  bool tryJoin(const Client &);
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
};

#endif // !CHANNEL_HPP
