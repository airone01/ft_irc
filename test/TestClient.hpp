#ifndef TESTCLIENT_HPP
#define TESTCLIENT_HPP

#include "../src/Client.hpp"
#include <string>
#include <vector>

/**
 * @brief subclass Client to access the protected _writeBuf
 * @note this should and will only be used for testing
 */
class TestClient : public Client {
public:
  TestClient(int fd, ConnectionManager *mgr) : Client(fd, NULL, mgr) {}

  /**
   * @brief helper to read what the server "sent" to this client
   */
  std::string popSentData() {
    if (_writeBuf.empty())
      return "";
    std::string s(_writeBuf.begin(), _writeBuf.end());
    _writeBuf.clear(); // so next message is clean
    return s;
  }

  /**
   * @brief helper to check if a specific substring was sent
   */
  bool received(const std::string &substring) {
    std::string s = popSentData();
    return s.find(substring) != std::string::npos;
  }
};

#endif
