/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:13:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/28 17:41:14 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "Client.hpp"

#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

class ClientManager {
public:
  std::vector<Client> &getClients();
  Client &getClientFromUsername(std::string &nickname);
  Client &getClientFromSocket(int &socket);

private:
  std::vector<Client> _clients;
};

#endif // !CLIENTMANAGER_HPP
