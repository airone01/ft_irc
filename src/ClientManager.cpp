#include "ClientManager.hpp"
#include "Client.hpp"

Client& ClientManager::getClientFromUsername(const std::string& username){
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator ite = _clients.end();
	for (; it != ite; ++it) {
		if (it->second.getUsername() == username)
			return it->second;
	}
	throw ClientNotFound("ERR_NOSUCHNICK");
}

Client& ClientManager::getClientFromNickname(const std::string& nickname){
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator ite = _clients.end();
	for (; it != ite; ++it) {
		if (it->second.getNickname() == nickname)
			return it->second;
	}
	throw ClientNotFound("ERR_NOSUCHNICK");
}

Client& ClientManager::getClientFromSocket(const int& socket){
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator ite = _clients.end();
	for (; it != ite; ++it) {
		if (it->second.getSocket() == socket)
			return it->second;
	}
	throw ClientNotFound("ERR_NOSUCHNICK");
}

size_t ClientManager::getClientCount() const {
	return _clients.size();
}

void ClientManager::addClient(const Client& client) {
	this->_clients[client.getSocket()] = client;
}

void ClientManager::removeClient(int socket) {
	this->_clients.erase(socket);
}

bool ClientManager::isNicknameUsed(const std::string& nickname){
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator ite = _clients.end();
	for (; it != ite; ++it) {
		if (it->second.getNickname() == nickname)
			return true;
	}
	//TODO NEED TO THROW ERR_NICKNAMEINUSE ? OR IF VERIFICATION LATER ?
	return false;
}
bool ClientManager::isUsernameUsed(const std::string& username){
	std::map<int, Client>::iterator it = _clients.begin();
	std::map<int, Client>::iterator ite = _clients.end();
	for (; it != ite; ++it) {
		if (it->second.getUsername() == username)
			return true;
	}
	//TODO NEED TO THROW ERR_NICKNAMEINUSE ? OR IF VERIFICATION LATER ?
	return false;
}
