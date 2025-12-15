#include "ChannelManager.hpp"

//NE PLUS TOUCHER CE FICHIER SAUF POUR METTRE DES METHODS UTILES

ChannelManager::ChannelManager() {}

ChannelManager::~ChannelManager() {}

Channel& ChannelManager::getChannelFromName(const std::string& name) {
	std::map<std::string, Channel>::iterator it = _channels.begin();
	std::map<std::string, Channel>::iterator ite = _channels.end();
	for (; it != ite; ++it) {
		if (it->second.getName() == name)
			return it->second;
	}
	throw ChannelNotFound("ERR_NOSUCHCHANNEL");
}

std::vector<std::string> ChannelManager::getChannelsOfUser(int socket) {
	std::vector<std::string> channelsOfUser;
	std::map<std::string, Channel>::iterator it = _channels.begin();
	std::map<std::string, Channel>::iterator ite = _channels.end();
	for (; it != ite; ++it) {
		if (it->second.hasUser(socket))
			channelsOfUser.push_back(it->second.getName());
	}
	return channelsOfUser;
}

std::vector<std::string> ChannelManager::getChannelNames() {
	std::vector<std::string> result;
	std::map<std::string, Channel>::iterator it = _channels.begin();
	std::map<std::string, Channel>::iterator ite = _channels.end();
	for (; it != ite; ++it)
		result.push_back(it->second.getName());
	return result;
}

size_t ChannelManager::getChannelCount() const {
	return _channels.size();
}

void ChannelManager::addChannel(const Channel& channel) {
	const std::string& name = channel.getName();
	if (hasChannel(name))
		throw ChannelAlreadyExists("ERR_");
	_channels[channel.getName()] = channel;
}

void ChannelManager::removeChannel(const std::string& name) {
	_channels.erase(name);
}

void ChannelManager::removeUserFromAllChannels(int socket) {
	std::vector<std::string> channelsToRemove;
	std::map<std::string, Channel>::iterator it = _channels.begin();
	std::map<std::string, Channel>::iterator ite = _channels.end();
	for (; it != ite; ++it) {
		if (it->second.hasUser(socket)) {
			try {
				it->second.removeUser(socket);
				if (it->second.getUserCount() == 0)
					channelsToRemove.push_back(it->first);
			} catch (...) {}
		}
	}
	for (size_t i = 0; i < channelsToRemove.size(); i++)
		_channels.erase(channelsToRemove[i]);
}

Channel& ChannelManager::createChannel(const std::string& name, int creatorSocket) {
	if (hasChannel(name)) {
		throw ChannelAlreadyExists("ERR_CHANNELALREADYEXISTS");
	}
	Channel newChannel(creatorSocket, name);
	_channels[name] = newChannel;
	return _channels[name];
}

bool ChannelManager::hasChannel(const std::string& name) const {
	return _channels.find(name) != _channels.end();
}