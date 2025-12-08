#include "../src/Client.hpp"
#include "../src/net/ConnectionManager.hpp"
#include "doctest.h"
#include <fcntl.h>
#include <unistd.h>

// helper to create a dummy client without a full server
// we use /dev/null for the fd so write() calls don't crash
struct DummyClientFixture {
  int dummy_fd;
  ConnectionManager *cm;

  DummyClientFixture() {
    dummy_fd = open("/dev/null", O_RDWR);
    cm = new ConnectionManager();
  }

  ~DummyClientFixture() {
    close(dummy_fd);
    delete cm;
  }

  Client *create() { return new Client(dummy_fd, NULL, cm); }
};

TEST_CASE_FIXTURE(DummyClientFixture, "Client state management") {
  Client *c = create();

  SUBCASE("Initial state") {
    CHECK(c->getNickname() == "");
    CHECK(c->getRegistered() == false);
    CHECK(c->getAuth() == false);
  }

  SUBCASE("Registration") {
    c->setNickname("Tester");
    c->setUsername("testuser");
    CHECK(c->getNickname() == "Tester");
    CHECK(c->getUsername() == "testuser");

    // manual registration trigger (logic usually in Commands::user/nick)
    if (!c->getNickname().empty() && !c->getUsername().empty()) {
      c->setRegistered(true);
    }
    CHECK(c->getRegistered() == true);
  }

  SUBCASE("Buffer handling") {
    std::string partial1 = "NICK ";
    std::string partial2 = "newnick\r\nUSER";

    c->appendToBuffer(partial1);
    CHECK(c->extractMessage() == ""); // TODO

    c->appendToBuffer(partial2);

    // first message should be ready
    std::string msg = c->extractMessage();
    CHECK(msg == "NICK newnick");

    // "USER" is left in buffer
    c->appendToBuffer(" guest 0 * :Real Name\r\n");
    CHECK(c->extractMessage() == "USER guest 0 * :Real Name");
  }

  delete c;
}
