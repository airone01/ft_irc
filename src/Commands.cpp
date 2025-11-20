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
            //todo: add numeric replies on succes RPL_TOPIC and RPL_NAMREPLY
            pswrdIt++;
        }
        catch(const ChannelManager::noSuchChannel& e)
        {
            channels.addChannels(Channel(user, *roomIt));
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

void Commands::mode(IRCMessage param){

}

void Commands::topic(IRCMessage param){

}

void Commands::invite(IRCMessage const &tmp, ChannelManager channels, Client &user){
    std::vector<std::string> param = tmp.getParams();
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

