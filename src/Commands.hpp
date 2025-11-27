#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "IRCMessage.hpp"
# include <algorithm>

class Commands
{
    virtual void join(IRCMessage const &param, ChannelManager channels, Client user);
    virtual void part(IRCMessage const &param, ChannelManager channels, Client &user);
    virtual void mode(IRCMessage const &tmp, ChannelManager channels, Client &user);
    virtual void topic(IRCMessage const &tmp, ChannelManager channels, Client &user);
    virtual void invite(IRCMessage const &tmp, ClientManager clients, ChannelManager channels, Client &user);
    
    virtual void kick(IRCMessage const &param, ChannelManager channels, Client &admin);
};

#endif