#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

#include <vector>
#include "Channel.hpp"


// This was meant to be static, but it's too much trouble.
class ChannelManager {
public:
  // ChannelManager();
  // ChannelManager(const ChannelManager &);
  // ~ChannelManager();
  //
  // ChannelManager &operator=(const ChannelManager &);

  std::vector<Channel> getChannels();
  void addChannels( const Channel & );
  void rmChannels( const Channel & );
  Channel &getChannelFromName(std::string &name);

  class noSuchChannel : public std::exception{
		const char *what() const throw();
	};

private:
  std::vector<Channel> _channels;
};

#endif