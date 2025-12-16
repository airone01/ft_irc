#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

//NE PLUS TOUCHER CE FICHIER SAUF POUR METTRE DES METHODS UTILES

class Client {
private:
	int			_socket;
	std::string	_nickname;
	std::string	_username;
	std::string	_hostname;
	std::string	_buffer;
	std::string	_rBuff;
	bool		_auth; // mot de passe validé
	bool		_registered; // mdp + nick + user
public:
	Client();
	Client(int fd);
	Client(const Client& copy);
	Client& operator=(const Client &other);
	~Client();
	// getter
	int					getSocket() const;
	const std::string&	getNickname() const;
	const std::string&	getUsername() const;
	const std::string&	getHostname() const;
	std::string&		getRbuff() ;
	bool				getAuth() const;
	bool				getRegistered() const;
	// setter
	void	setNickname(const std::string& nick);
	void	setUsername(const std::string& user);
	void	setHostname(const std::string& hostname);
	void	setAuth(bool auth);
	void	setRegistered(bool reg);
	void	setSocket(int fd);
	// method
	void		appendToBuffer(const std::string& buffer);
	void		clearBuffer();
	std::string	extractMessage();
	void		sendMessage(const std::string& buffer) const;
};

#endif // !CLIENT_HPP
