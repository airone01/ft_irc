#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "Client.hpp"
#include <map>
#include <stdexcept>
#include <vector>

//NE PLUS TOUCHER CE FICHIER SAUF POUR METTRE DES METHODS UTILES

class ClientManager {
private:
	std::map<int, Client> _clients;
public:
	// exception
	class ClientNotFound: public std::runtime_error {
	public:
		ClientNotFound(const std::string& msg)
		: std::runtime_error(msg) {}
	};
	// getter
	Client&	getClientFromUsername(const std::string& username);
	Client&	getClientFromNickname(const std::string& nickname);
	Client&	getClientFromSocket(const int& socket);
	size_t	getClientCount() const;
	std::vector<Client*> getAllClients();
	// setter
	void	addClient(const Client& client);
	void	removeClient(int socket);
	// utility
	bool	isNicknameUsed(const std::string& nickname);
	bool	isUsernameUsed(const std::string& username);
};

#endif // !CLIENTMANAGER_HPP
