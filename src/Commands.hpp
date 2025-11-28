#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "IRCMessage.hpp"
# include "ChannelManager.hpp"
# include <algorithm>

class Commands
{
    public:
    static void join(IRCMessage const &param, ChannelManager channels, Client user);
    void part(IRCMessage const &param, ChannelManager channels, Client &user);
    void mode(IRCMessage const &tmp, ChannelManager channels, Client &user);
    void topic(IRCMessage const &tmp, ChannelManager channels, Client &user);
    void invite(IRCMessage const &tmp, ClientManager clients, ChannelManager channels, Client &user);
    void kick(IRCMessage const &param, ChannelManager channels, Client &admin);
};

#endif