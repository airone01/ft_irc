#include "Client.hpp"
#include <sys/socket.h>
#include <unistd.h>

Client::Client() : _socket(-1), _nickname(""), _username(""), _hostname(""), _buffer(""), _auth(false), _registered(false) {
}

Client::Client(int fd) : _socket(fd), _nickname(""), _username(""), _hostname(""), _buffer(""), _auth(false), _registered(false) {
}

Client::Client(const Client& copy) : _socket(copy._socket), _nickname(copy._nickname), _username(copy._username),
	_hostname(copy._hostname), _buffer(copy._buffer), _auth(copy._auth), _registered(copy._registered) {
}

Client& Client::operator=(const Client& other) {
	if (this != &other) {
		this->_socket = other._socket;
		this->_nickname = other._nickname;
		this->_username = other._username;
		this->_hostname = other._hostname;
		this->_buffer = other._buffer;
		this->_auth = other._auth;
		this->_registered = other._registered;
	}
	return (*this);
}

Client::~Client() {
	if (this->_socket != -1) {
		close(this->_socket);
	}
}

int	Client::getSocket() const {
	return (this->_socket);
}

std::string	Client::getNickname() const {
	return (this->_nickname);
}

std::string	Client::getUsername() const {
	return (this->_username);
}

std::string	Client::getHostname() const {
	return (this->_hostname);
}

bool Client::getAuth() const {
	return (this->_auth);
}

bool Client::getRegistered() const {
	return (this->_registered);
}

void Client::setNickname(const std::string& nick) {
	this->_nickname = nick;
}

void Client::setUsername(const std::string& user) {
	this->_username = user;
}

void Client::setAuth(bool auth) {
	this->_auth = auth;
}

void Client::setRegistered(bool reg) {
	this->_registered = reg;
}

void Client::appendToBuffer(const std::string& buffer) {
	this->_buffer += buffer;
}

std::string	Client::extractMessage() {
	size_t pos = this->_buffer.find("\r\n");
	if (pos == std::string::npos) {
		pos = this->_buffer.find('\n');
		if (pos == std::string::npos) {
			return ("");
		}
	}
	std::string message = this->_buffer.substr(0, pos);
	size_t count;
	if (this->_buffer[pos] == '\r')
		count = 2;
	else
		count = 1;
	this->_buffer.erase(0, pos + count);
	return (message);
}

void Client::sendMessage(const std::string& buffer) {
	if (this->_socket != -1) {
		send(this->_socket, buffer.c_str(), buffer.length(), 0);
	}
}