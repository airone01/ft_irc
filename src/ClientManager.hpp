#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

# include <vector>
# include "Client.hpp"

class ClientManager 
{
    private:
    
        std::vector<Client> _clients;

    public:

        std::vector<Client> getClients();
        Client &getClientFromUsername(std::string &Username);
        Client &getClientFromSocket(int &socket);
};

#endif