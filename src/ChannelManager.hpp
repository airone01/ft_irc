#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include "Channel.hpp"
#include <map>
#include <vector>
#include <stdexcept>

class ChannelManager {
private:
	std::map<std::string, Channel> _channels;
public:
	ChannelManager();
	~ChannelManager();
	// Exceptions
	class ChannelNotFound: public std::runtime_error {
	public:
		ChannelNotFound(const std::string& msg)
			: std::runtime_error(msg) {}
	};
	class ChannelAlreadyExists: public std::runtime_error {
	public:
		ChannelAlreadyExists(const std::string& msg)
			: std::runtime_error(msg) {}
	};
	// getter
	Channel&					getChannelFromName(const std::string& name);
	std::vector<std::string>	getChannelsOfUser(int socket);
	std::vector<std::string>	getChannelNames();
	size_t						getChannelCount() const;
	// setter
	void		addChannel(const Channel& channel);
	void		removeChannel(const std::string& name);
	void		removeUserFromAllChannels(int socket);
	Channel&	createChannel(const std::string& name, int creatorSocket);
	// utility
	bool	hasChannel(const std::string& name) const;
};

#endif //!CHANNELMANAGER_HPP
