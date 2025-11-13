/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:13:42 by elagouch          #+#    #+#             */
/*   Updated: 2025/11/13 15:08:48 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "Client.hpp"

#ifndef CLIENTMANAGER_HPP

// This was meant to be static, but it's too much trouble.
class ClientManager {
public:
  std::vector<Client> getClients();
  Client &getClientFromNickname(std::string &nickname);

private:
  std::vector<Client> _clients;
};

#endif // !CLIENTMANAGER_HPP
