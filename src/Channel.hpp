#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ClientManager.hpp"
#include "IRCMessage.hpp"
#include <set>

//NE PLUS TOUCHER CE FICHIER SAUF POUR METTRE DES METHODS UTILES

class Channel {
private:
	std::string		_name;
	std::string		_topic;
	std::string		_password;
	std::size_t		_maxCapacity;
	std::set<char>	_modes;
	std::set<int>	_users;
	std::set<int>	_admins;
	std::set<int>	_invitedUsers;
	std::set<int>	_kickedUsers;
	// utility
	bool	checkPassword(const std::string& password) const;
	bool	checkCapacity() const;
public:
	Channel();
	Channel(int socket, const std::string& name);
	Channel(const Channel& copy);
	Channel& operator=(const Channel& other);
	~Channel();
	// exception
	class InvalidChanName: public std::runtime_error {
	public:
		InvalidChanName(const std::string& msg)
		: std::runtime_error(msg) {}
	};
	class UserNotInChan: public std::runtime_error {
	public:
		UserNotInChan(const std::string& msg)
		: std::runtime_error(msg) {}
	};
	// getter
	const std::string&		getName() const;
	const std::string&		getTopic() const;
	const std::string&		getPassword() const;
	size_t					getMaxCapacity() const;
	size_t					getUserCount() const;
	const std::set<char>&	getMode() const;
	const std::set<int>&	getUsers() const;
	const std::set<int>&	getAdmins() const;
	const std::set<int>&	getInvitedUsers() const;
	const std::set<int>&	getKickedUsers() const;
	// setter
	void	setName(const std::string& name);
	void	setTopic(const std::string& topic);
	void	setPassword(const std::string& password);
	void	setCapacity(size_t capacity);
	// utility
	bool	hasMode(char mode) const;
	bool	hasUser(int socket) const;
	bool	isAdmin(int socket) const;
	bool	isInvited(int socket) const;
	bool	isKicked(int socket) const;
	// mode
	void	addMode(char mode);
	void	removeMode(char mode);
	void	updateMode(const IRCMessage& msg, int sender, ClientManager& clients);
	// user
	bool	tryJoin(int socket, const std::string& password);
	void	addUser(int socket);
	void	removeUser(int socket);
	void	inviteUser(int socket);
	// admin
	void	addAdmin(int socket);
	void	removeAdmin(int socket);
	void	kickUser(int socket);
	// topic
	bool	changeTopic(const std::string& topicName, int sender);
	// validation for all channels
	static bool	isValidName(const std::string& name);
};

enum CHNGMODE {
	SINVITE,
	UINVITE,
	STOPIC,
	UTOPIC,
	SPASSWORD,
	UPASSWORD,
	SPRIV,
	UPRIV,
	SLIMIT,
	ULIMIT,
	DEFAULT1
};

CHNGMODE applyMode(const std::string& tmp);

#endif // !CHANNEL_HPP
