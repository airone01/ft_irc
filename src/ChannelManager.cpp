#include "ChannelManager.hpp"
#include "Channel.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

// ChannelManager::ChannelManager() : _channels() {}
//
// ChannelManager::ChannelManager(const ChannelManager &other)
//     : _channels(other._channels) {}
//
// ChannelManager::~ChannelManager() { /* vector should delete itself */ }
//
// ChannelManager &ChannelManager::operator=(const ChannelManager &other) {
//   if (this != &other) {
//     this->_channels = other._channels;
//   }
//   return (*this);
// }

struct ChannelNameMatcher
{
    ChannelNameMatcher(const std::string &n) : name(n) {}
    bool operator()(const Channel &chan) const { return chan.getName() == name; }
    std::string name;
};

Channel &ChannelManager::getChannelFromName(std::string &name) {
  const std::vector<Channel>::iterator it = std::find_if(
      _channels.begin(), _channels.end(), ChannelNameMatcher(name));

	if (it == _channels.end())
		throw noSuchChannel();
  return (*it);
}

std::vector<Channel> ChannelManager::getChannels( void ) { return this->_channels; }

void ChannelManager::addChannels(const Channel &tmp){
	_channels.push_back(tmp);
}

void ChannelManager::rmChannels(const Channel &tmp){
	std::vector<Channel>::iterator it = _channels.begin();
	for (;it != _channels.end();it++)
		if (it->getName() == tmp.getName()){
			_channels.erase(it);
			break;
		}
}

const char *ChannelManager::noSuchChannel::what() const throw(){
	return ("");
}