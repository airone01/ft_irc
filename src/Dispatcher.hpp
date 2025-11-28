#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "ChannelManager.hpp"
#include "Client.hpp"
#include "net/Connection.hpp"
#include "net/ConnectionManager.hpp"
#include <vector>

class Dispatcher {
public:
  Dispatcher(ConnectionManager *clients, ChannelManager *channels);
  ~Dispatcher();

  /**
   * @brief Entry point for new data from a connection.
   * Buffers data, extracts lines, parses, and executes commands.
   */
  void handleData(Connection *conn, const std::vector<char> &data);

private:
  ConnectionManager *_clients;
  ChannelManager *_channels;

  /**
   * @brief Execute a single parsed command.
   */
  void executeCommand(Client &client, const std::string &line);
};

#endif