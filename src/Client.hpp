#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
private:
	int			_socket;
	std::string	_nickname;
	std::string	_username;
	std::string	_hostname;
	std::string	_buffer;
	bool		_auth;
	bool		_registered;
public:
	Client();
	Client(int fd);
	Client(const Client& copy);
	Client& operator=(const Client& other);
	~Client();
	// getter
	int			getSocket() const;
	std::string	getNickname() const;
	std::string	getUsername() const;
	std::string	getHostname() const;
	bool		getAuth() const;
	bool		getRegistered() const;
	// setter
	void setNickname(const std::string& nick);
	void setUsername(const std::string& user);
	void setAuth(bool auth);
	void setRegistered(bool reg);
	// method
	void		appendToBuffer(const std::string& buffer);
	std::string	extractMessage();
	void		sendMessage(const std::string& buffer);
};

#endif
