#include "Commands.hpp"
#include "ChannelManager.hpp"
#include "ClientManager.hpp"
#include <vector> 

std::vector<std::string> paramHandler(std::string params){
    std::vector<std::string>    newParam;

}

void Commands::join(IRCMessage const &tmp, ChannelManager channels, Client user){
    
    bool simpleCmdLine = true;
    
    std::vector<std::string> param = tmp.getParams();
    std::vector<std::string>::iterator it = param.begin();

    std::vector<std::string> rooms = tmp.getParams();
    std::vector<std::string> pswrd = tmp.getParams();

    if (std::find(it->begin(), it->end(), ',') != it->end()){
        rooms = paramHandler(*it);
        simpleCmdLine = false;
    }
    it++;
    if (it != param.end() && std::find(it->begin(), it->end(), ',') != it->end())
        pswrd = paramHandler(*it);
    
    Channel &actual = channels.getChannelFromName(*rooms.begin());
    if (std::find(actual.getInvitedUsers().begin(), actual.getInvitedUsers().end(), user.getSocket()) 
        != actual.getInvitedUsers().end())
}

void Commands::part(IRCMessage param){

}

void Commands::mode(IRCMessage param){

}

void Commands::topic(IRCMessage param){

}

void Commands::invite(IRCMessage param){

}

void Commands::kick(IRCMessage param){

}

