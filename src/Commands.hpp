#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "IRCMessage.hpp" 
# include <algorithm>

class Commands
{
    virtual void join(IRCMessage const &param, ChannelManager channels, Client user);
    virtual void part(IRCMessage const &param);
    virtual void mode(IRCMessage const &param);
    virtual void topic(IRCMessage const &param);
    virtual void invite(IRCMessage const &param);
    virtual void kick(IRCMessage const &param, Client admin);
};

#endif