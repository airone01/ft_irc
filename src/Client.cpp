#include "Client.hpp"
#include <sys/socket.h>

//NE PLUS TOUCHER CE FICHIER SAUF POUR METTRE DES METHODS UTILES

Client::Client()
	: _socket(-1), _nickname(""), _username(""),
	_hostname(""), _buffer(""), _auth(false), _registered(false) {}

Client::Client(int fd)
	: _socket(fd), _nickname(""), _username(""),
	_hostname(""), _buffer(""), _auth(false), _registered(false) {}

Client::Client(const Client& copy)
	: _socket(copy._socket), _nickname(copy._nickname),
	_username(copy._username), _hostname(copy._hostname), _buffer(copy._buffer),
	_auth(copy._auth), _registered(copy._registered) {}

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
	return *this;
}

Client::~Client() {}

int Client::getSocket() const {
	return (this->_socket);
}

const std::string& Client::getNickname() const {
	return (this->_nickname);
}

const std::string& Client::getUsername() const {
	return (this->_username);
}

const std::string& Client::getHostname() const {
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

void Client::setHostname(const std::string& hostname) {
	this->_hostname = hostname;
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
void Client::clearBuffer() {
	this->_buffer.clear();
}

std::string Client::extractMessage() {
	size_t pos = _buffer.find("\n");
	if (pos == std::string::npos) {
	return "";
	}
	std::string line = _buffer.substr(0, pos + 1);
	_buffer.erase(0, pos + 1);
	if (!line.empty() && line[line.size() - 1] == '\n')
		line.erase(line.size() - 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return line;
}

void Client::sendMessage(const std::string& buffer) const {
	if (buffer.length() < 2) {
		//TODO Need to send an empty msg or just a return is usefull ?
		return ;
	}
	if (this->_socket != -1) {
		send(this->_socket, buffer.c_str(), buffer.length(), 0);
	}
}
