#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "net/Connection.hpp"
#include <string>

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
	Client(const Client &copy);
	Client &operator=(const Client &other);
public:
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
};

#endif