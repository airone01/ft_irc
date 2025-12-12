#include "Channel.hpp"
#include <algorithm>
#include <sstream>

//NE PLUS TOUCHER CE FICHIER SAUF POUR METTRE DES METHODS UTILES

bool	Channel::checkPassword(const std::string& password) const {
	return !password.empty();
}
bool	Channel::checkCapacity() const {
	return _maxCapacity != 0;
}

Channel::Channel()
	: _name(""), _topic(""), _password(""), _maxCapacity(0) {}

Channel::Channel(int creatorSocket, const std::string &name)
	: _name(name), _topic(""), _password(""), _maxCapacity(0) {
	if (!isValidName(name)) {
		throw InvalidChanName("ERR_NOSUCHCHANNEL");
	}
	_users.insert(creatorSocket);
	_admins.insert(creatorSocket);
}

Channel::Channel(const Channel &copy)
	: _name(copy._name), _topic(copy._topic), _password(copy._password),
	_maxCapacity(copy._maxCapacity), _modes(copy._modes), _users(copy._users),
	_admins(copy._admins), _invitedUsers(copy._invitedUsers),
	_kickedUsers(copy._kickedUsers) {}

Channel &Channel::operator=(const Channel &other) {
	if (this != &other) {
		_name = other._name;
		_topic = other._topic;
		_password = other._password;
		_maxCapacity = other._maxCapacity;
		_modes = other._modes;
		_users = other._users;
		_admins = other._admins;
		_invitedUsers = other._invitedUsers;
		_kickedUsers = other._kickedUsers;
	}
	return *this;
}

Channel::~Channel() {}

const std::string& Channel::getName() const {
	return _name;
}

const std::string& Channel::getTopic() const {
	return _topic;
}

const std::string& Channel::getPassword() const {
	return _password;
}

size_t Channel::getMaxCapacity() const {
	return _maxCapacity;
}

bool Channel::hasMode(char mode) const {
	return _modes.find(mode) != _modes.end();
}

const std::set<char>& Channel::getMode() const {
	return _modes;
}

const std::set<int>& Channel::getUsers() const {
	return _users;
}

const std::set<int>& Channel::getAdmins() const {
	return _admins;
}

const std::set<int>& Channel::getInvitedUsers() const {
	return _invitedUsers;
}

const std::set<int>& Channel::getKickedUsers() const {
	return _kickedUsers;
}

bool Channel::hasUser(int socket) const {
	return _users.find(socket) != _users.end();
}

bool Channel::isAdmin(int socket) const {
	return _admins.find(socket) != _admins.end();
}

bool Channel::isInvited(int socket) const {
	return _invitedUsers.find(socket) != _invitedUsers.end();
}

bool Channel::isKicked(int socket) const {
	return _kickedUsers.find(socket) != _kickedUsers.end();
}

size_t Channel::getUserCount() const {
	return _users.size();
}

void	Channel::setName(const std::string& name) {
	_name = name;
}

void Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

void Channel::setPassword(const std::string &password) {
	_password = password;
}

void Channel::setCapacity(size_t capacity) {
	_maxCapacity = capacity;
}

void Channel::addMode(char mode) {
	_modes.insert(mode);
}

void Channel::removeMode(char mode) {
	_modes.erase(mode);
}

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
	DEFAULT
};

CHNGMODE applyMode(const std::string& tmp) {
	if (tmp == "+i")
		return SINVITE;
	if (tmp == "-i")
		return UINVITE;
	if (tmp == "+t")
		return STOPIC;
	if (tmp == "-t")
		return UTOPIC;
	if (tmp == "+k")
		return SPASSWORD;
	if (tmp == "-k")
		return UPASSWORD;
	if (tmp == "+o")
		return SPRIV;
	if (tmp == "-o")
		return UPRIV;
	if (tmp == "+l")
		return SLIMIT;
	if (tmp == "-l")
		return ULIMIT;
	return DEFAULT;
}

void Channel::updateMode(const IRCMessage& msg, int senderSocket, ClientManager& clients) {
	std::vector<std::string> params = msg.getParams();
	if(msg.getCountParams() < 2)
		throw std::runtime_error("ERR_NEEDMOREPARAMS");
	if (!isAdmin(senderSocket))
		throw std::runtime_error("ERR_CHANOPRIVSNEEDED");
	std::string modeString = params[1];
	std::string changes = "";
	std::string currentMode = "";
	std::string temp = "+";
	size_t argIndex = 2;
	for (size_t i = 0; i < modeString.size(); i++) {
		if (modeString[i] == '+' || modeString[i] == '-') {
			temp[0] = modeString[i];
			continue;
		}
		temp[1] = modeString[i];
		currentMode = temp[0];
		currentMode += temp[1];
		CHNGMODE mode = applyMode(currentMode);
		switch (mode) {
			case SINVITE:
				addMode('i');
				break;
			case UINVITE:
				removeMode('i');
				break;
			case STOPIC:
				addMode('t');
				break;
			case UTOPIC:
				removeMode('t');
				break;
			case SPASSWORD:
				if (argIndex >= params.size())
					throw std::runtime_error("ERR_NEEDMOREPARAMS");
				_password = params[argIndex++];
				addMode('k');
				break;
			case UPASSWORD:
				_password = "";
				removeMode('k');
				break;
			case SPRIV:
				if (argIndex >= params.size())
					throw std::runtime_error("ERR_NEEDMOREPARAMS");
				{
					std::string& targetNickname = params[argIndex++];
					try {
						Client& client = clients.getClientFromNickname(targetNickname);
						int socket = client.getSocket();
						if (!hasUser(socket))
							throw std::runtime_error("ERR_USERNOTINCHANNEL");
						addAdmin(socket);
					} catch (const ClientManager::ClientNotFound& ) {
						throw std::runtime_error("ERR_NOSUCHNICK");
					}
				}
				break;
			case UPRIV:
				if (argIndex >= params.size())
					throw std::runtime_error("ERR_NEEDMOREPARAMS");
				{
					std::string& targetNickname = params[argIndex++];
					try {
						Client& client = clients.getClientFromNickname(targetNickname);
						int socket = client.getSocket();
						if (!hasUser(socket))
							throw std::runtime_error("ERR_USERNOTINCHANNEL");
						if (_admins.size() <= 1)
							throw std::runtime_error("ERR_CHANOPRIVSNEEDED");
						removeAdmin(socket);
					} catch (const ClientManager::ClientNotFound& ) {
						throw std::runtime_error("ERR_NOSUCHNICK");
					}
				}
				break;
			case SLIMIT:
				if (argIndex >= params.size())
					throw std::runtime_error("ERR_NEEDMOREPARAMS");
				{
					size_t limit = 0;
					std::stringstream ss(params[argIndex++]);
					ss >> limit;
					if (limit <= 0)
						throw std::runtime_error("ERR_NEEDMOREPARAMS");
					_maxCapacity = limit;
					addMode('l');
				}
				break;
			case ULIMIT:
				_maxCapacity = 0;
				removeMode('l');
				break;
			default:
				throw std::runtime_error("ERR_UNKNOWNMODE");
		}
	}
}

bool Channel::tryJoin(int socket, const std::string& password) const {
	if (isKicked(socket))
		throw std::runtime_error("ERR_BANNEDFROMCHAN");
	if (hasMode('i') && !isInvited(socket))
		throw std::runtime_error("ERR_INVITEONLYCHAN");
	if (hasMode('k') && password != getPassword())
		throw std::runtime_error("ERR_BADCHANNELKEY");
	if (hasMode('l') && getUserCount() >= _maxCapacity)
		throw std::runtime_error("ERR_CHANNELISFULL");
	return true;
}

void Channel::addUser(int socket) {
	_users.insert(socket);
	_invitedUsers.erase(socket);
}

void Channel::removeUser(int socket) {
	if (!hasUser(socket)) {
		throw UserNotInChan("ERR_USERNOTINCHANNEL");
	}
	_users.erase(socket);
	_admins.erase(socket);
}

void Channel::addAdmin(int socket) {
	if (!hasUser(socket)) {
		throw UserNotInChan("ERR_USERNOTINCHANNEL");
	}
	_admins.insert(socket);
}

void Channel::removeAdmin(int socket) {
	_admins.erase(socket);
}

void Channel::inviteUser(int socket) {
	_invitedUsers.insert(socket);
}

void Channel::kickUser(int socket) {
	removeUser(socket);
	_kickedUsers.insert(socket);
}

bool Channel::changeTopic(const std::string& topicName, int sender) {
	if (!hasUser(sender))
		throw std::runtime_error("ERR_NOTONCHANNEL");
	if (hasMode('t') && !isAdmin(sender))
		throw std::runtime_error("ERR_CHANOPRIVSNEEDED");
	if (topicName.empty() && _topic.empty())
		throw std::runtime_error("RPL_NOTOPIC");
	_topic = topicName;
	return true;
}

bool Channel::isValidName(const std::string& name) {
	if (name.empty() || name.size() > 200) {
		return false;
	}
	if (name[0] != '#' && name[0] != '&') {
		return false;
	}
	if (name.find(' ') != std::string::npos || name.find(',') != std::string::npos
			|| name.find(7) != std::string::npos) {
		return false;
	}
	return true;
}