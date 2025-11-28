#include "Commands.hpp"
#include "ChannelManager.hpp"
#include "ClientManager.hpp"
#include <vector> 

std::vector<std::string> paramHandler(std::string params){
    std::vector<std::string>    newParam;
    std::string tmp;
    for (size_t i = 0; ;){
        if (i == 0)
            tmp = params.substr(i, params.find(','));
        else
            tmp = params.substr(i);
        newParam.push_back(tmp);
        i = params.find(',', i);
        if (i == std::string::npos)
            break ;
        i++;
    }
    return newParam;
}

void Commands::join(IRCMessage const &tmp, ChannelManager channels, Client user){
        
    std::vector<std::string> param = tmp.getParams();
    std::vector<std::string>::iterator it = param.begin();
    std::vector<std::string>::iterator roomIt;
    std::vector<std::string>::iterator pswrdIt;

    std::vector<std::string> rooms = tmp.getParams();
    std::vector<std::string> pswrd = tmp.getParams();

    if (std::find(it->begin(), it->end(), ',') != it->end())
        rooms = paramHandler(*it);
    it++;
    if (it != param.end() && std::find(it->begin(), it->end(), ',') != it->end())
        pswrd = paramHandler(*it);
        
    for (roomIt = rooms.begin(), pswrdIt = pswrd.begin(); roomIt != rooms.end() ;roomIt++){
        try
        {
            Channel &actual = channels.getChannelFromName(*roomIt);
            actual.tryJoin(user, *pswrdIt);
            actual.newUser(user);
            std::cout << "success\n";
            //todo: add numeric replies on succes RPL_TOPIC and RPL_NAMREPLY
            pswrdIt++;
        }
        catch(const ChannelManager::noSuchChannel& e)
        {
            channels.addChannels(Channel(user, *roomIt));
            std::cout << "success\n";
        }
        catch(const std::exception& e)
        {
            //todo: add replies sender for error or valid commands 
            std::cerr << e.what() << '\n';
        }
    }
}

void Commands::part(IRCMessage const &tmp, ChannelManager channels, Client &user){
    std::vector<std::string> param = tmp.getParams();
    std::vector<std::string>::iterator it;
    for (it = param.begin(); it != param.end();it++){
        try
        {
            Channel &actual = channels.getChannelFromName(*it);
            actual.leaveChannel(user);
            if (actual.getUsers().empty()){
                channels.rmChannels(actual);
                actual.~Channel();
            //todo: reply
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

/*
flags: 
        i(set/unset invite only)
        t(set/unset topic priv to admin)
        k(set/unset password)
        o(give/take admin priv)
        l(set/unset limit size)

replies :
           ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           RPL_BANLIST                     RPL_ENDOFBANLIST
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL
           ERR_USERSDONTMATCH              ERR_UMODEUNKNOWNFLAG
           RPL_UMODEIS
*/
void Commands::mode(IRCMessage const &param, ChannelManager channels, Client &user){
    std::vector<std::string> tmp = param.getParams();
    try
    {
        Channel &actual = channels.getChannelFromName(tmp[0]);
        actual.updateMode(param, user);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void Commands::topic(IRCMessage const &tmp, ChannelManager channels, Client &user){
    if (tmp.getCountParams() != 1)
        std::cerr << "ERR_NEEDMOREPARAMS\n";
    std::vector<std::string> param = paramHandler(tmp.getParams()[0]);
    std::vector<std::string>::iterator it = param.begin();

    for ( ; it != param.end(); it++){
        try
        {
            Channel &actual = channels.getChannelFromName(*it);
            actual.changeTopic(tmp, user);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void Commands::invite(IRCMessage const &tmp, ClientManager clients, ChannelManager channels, Client &user){
    std::vector<std::string> param = tmp.getParams();
    try{
        Channel &actual = channels.getChannelFromName(param[1]);
        actual.tryInvite(param, clients, user.getSocket(), 
            clients.getClientFromUsername(param[0]).getSocket());
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void Commands::kick(IRCMessage const &tmp, ChannelManager channels, Client &admin){
    std::vector<std::string> param = tmp.getParams();
    try
    {
        Channel &actual = channels.getChannelFromName(param[0]);
        actual.tryKick(param, tmp, admin);
        std::map<int, Client*>::iterator victimIt;
        for (victimIt = actual.getUsers().begin(); victimIt != actual.getUsers().end();victimIt++)
            if (victimIt->second->getUsername() == param[1])
                actual.setKickedUsers(victimIt->second->getSocket());
        }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
}

