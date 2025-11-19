#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>
# include <map>
# include <Client.hpp>
# include <vector>
# include <set>
# include <algorithm>

# define ADMIN
# define USER

class Channel
{
    protected:

    std::map<int, Client*> _users;
    std::map<int, Client*> _admins;
    std::map<int, Client*> _invitedUsers;
	std::set<char>		  _mode;
	std::set<int>		  _kickedUsers;
    std::string           _topic;
    std::string           _pswrd;
    std::string           _name;
    int                   _maxCapacity;
    bool                  _modeSet;

    Channel( void );
    Channel(const Channel & );
    Channel &operator=(const Channel & );

    public:

    Channel( Client & );
    Channel( Client & , int capacity );
    ~Channel( void );

    std::string getTopic() const;
    std::string getPswrd() const;
    std::string getName() const;
    int getCapacity() const;
    bool getModeSet() const;
    std::map<int, Client*> getInvitedUsers();

    std::set<char> getMode() const;
    std::set<int> getKickedUsers() const;


    void    setTopic(const std::string newTopic );
    void    setPswrd(const std::string newPswrd );
    void    setName(const std::string newName );
    void    setCapacity(const int newCapacity );
    void    setModeSet(const bool changeMode );
    void    setMode(const char c);
    void    setKickedUsers(const int socket);

	void	newUser( Client & );
	void	tryJoin( const Client &, std::string pswrd );
	void	updatePriv( const Client &admin, Client &user );

	// modifier les try catch pour les encapsuler dans les cpp des channel et des commandes
	class maxCapacityReached : public std::exception{
		const char *what() const throw();
	};

	class invitationNeeded : public std::exception{
		const char *what() const throw();
	};

	class insufficientPrivilege : public std::exception{
		const char *what() const throw();
	};

	class invalidChannelName : public std::exception{
		const char *what() const throw();
	};

    class errorMode : public std::exception{
        private:
            std::string _errMsg;
		public:
            errorMode(std::string error) : _errMsg(std::string("error: ") + error) {}
            const char *what() const throw();
            ~errorMode() throw() {};

    };
};

#endif
