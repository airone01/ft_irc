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
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void Commands::part(IRCMessage param){

}

void Commands::mode(IRCMessage param){

}

void Commands::topic(IRCMessage param){

}

void Commands::invite(IRCMessage param){

}

void Commands::kick(IRCMessage const &param, Client admin){
    
}

