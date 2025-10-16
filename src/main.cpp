/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/10/16 20:39:36 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Socket.hpp"
#include <ostream>
#include <stdexcept>

int main() {
  logger::Logger::getInstance().setMinLevel(logger::DEBUG);

  try {
    Socket server;
    server.bind(IRC_PORT);

    logger::info() << "Server listening on port " << IRC_PORT << std::endl;
    server.listen();

    for (;;) {
      Socket *client = server.accept();
      logger::debug() << "New client connected" << std::endl;

      // stupid handling which just echoes back the data for now
      std::string data = client->recv(1024);
      if (!data.empty()) {
        logger::debug() << "Received: " << data << std::endl;
        client->send("Echo: " + data);
      }

      client->close();
      delete client;
    }
  } catch (const std::runtime_error &e) {
    logger::error() << e.what() << std::endl;
  }

  return 0;
}
