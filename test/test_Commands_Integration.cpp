#include "../src/ChannelManager.hpp"
#include "../src/ClientManager.hpp"
#include "../src/Commands.hpp"
#include "../src/net/ConnectionManager.hpp"
#include "TestClient.hpp"
#include "doctest.h"
#include <fcntl.h>
#include <unistd.h>

struct IntegrationFixture {
  int fd_root, fd_user1, fd_user2;
  ConnectionManager *connMgr;
  ClientManager *clientMgr;
  ChannelManager *chanMgr;
  TestClient *admin, *user1, *user2;

  IntegrationFixture() {
    fd_root = open("/dev/null", O_RDWR);
    fd_user1 = open("/dev/null", O_RDWR);
    fd_user2 = open("/dev/null", O_RDWR);

    connMgr = new ConnectionManager();
    clientMgr = new ClientManager();
    chanMgr = new ChannelManager();

    admin = new TestClient(fd_root, connMgr);
    user1 = new TestClient(fd_user1, connMgr);
    user2 = new TestClient(fd_user2, connMgr);

    admin->setNickname("Admin");
    admin->setUsername("admin");
    admin->setRegistered(true);
    user1->setNickname("Alice");
    user1->setUsername("alice");
    user1->setRegistered(true);
    user2->setNickname("Bob");
    user2->setUsername("bob");
    user2->setRegistered(true);

    // Note: ClientManager might need an 'add' method,
    // or we push to the vector if it exposes a reference.
    clientMgr->getClients().push_back(*admin);
    clientMgr->getClients().push_back(*user1);
    clientMgr->getClients().push_back(*user2);
  }

  ~IntegrationFixture() {
    delete admin;
    delete user1;
    delete user2;
    delete connMgr;
    delete clientMgr;
    delete chanMgr;
    close(fd_root);
    close(fd_user1);
    close(fd_user2);
  }
};

TEST_CASE_FIXTURE(IntegrationFixture, "Multiple JOIN Scenarios") {

  SUBCASE("Join multiple channels (comma separated)") {
    std::string raw = "JOIN #foo,#bar\r\n";
    IRCMessage msg(raw);

    Commands::join(msg, *chanMgr, *user1);

    std::string name_foo = "#foo";
    std::string name_bar = "#bar";
    CHECK_NOTHROW(chanMgr->getChannelFromName(name_foo));
    CHECK_NOTHROW(chanMgr->getChannelFromName(name_bar));

    Channel &foo = chanMgr->getChannelFromName(name_foo);
    Channel &bar = chanMgr->getChannelFromName(name_bar);

    CHECK(foo.getUsers().size() == 1);
    CHECK(bar.getUsers().size() == 1);
  }

  SUBCASE("Join multiple channels with keys") {
    std::string raw = "JOIN #locked,#open secret\r\n";
    IRCMessage msg(raw);

    Commands::join(msg, *chanMgr, *user1);

    // verify #locked was created with key "secret" (implied by success, or
    // check logic) since we are creating them, we can check the channel
    // properties
    try {
      std::string name = "#locked";
      Channel &locked = chanMgr->getChannelFromName(name);
      CHECK(locked.getPswrd() == "secret"); // should auto-set +k with key
      CHECK(locked.getUsers().count(user1->getSocket()) == 1);
    } catch (...) {
      CHECK(false); // should not fail
    }
  }
}

TEST_CASE_FIXTURE(IntegrationFixture, "NAMES command") {
  std::string joinRaw = "JOIN #test\r\n";
  IRCMessage joinMsg(joinRaw);
  Commands::join(joinMsg, *chanMgr, *user1);
  Commands::join(joinMsg, *chanMgr, *user2);

  // clear buffer from JOIN responses
  user1->popSentData();

  SUBCASE("NAMES #test") {
    std::string raw = "NAMES #test\r\n";
    IRCMessage msg(raw);

    Commands::names(msg, *chanMgr, *user1);

    std::string response = user1->popSentData();

    // RFC 1459: 353 (RPL_NAMREPLY) and 366 (RPL_ENDOFNAMES)
    CHECK(response.find("353") != std::string::npos);
    CHECK(response.find("366") != std::string::npos);
    CHECK(response.find("Alice") != std::string::npos);
    CHECK(response.find("Bob") != std::string::npos);
  }
}

TEST_CASE_FIXTURE(IntegrationFixture, "WHO command") {
  std::string joinRaw = "JOIN #test\r\n";
  IRCMessage joinMsg(joinRaw);
  Commands::join(joinMsg, *chanMgr, *user1);
  user1->popSentData();

  SUBCASE("WHO #test") {
    std::string raw = "WHO #test\r\n";
    IRCMessage msg(raw);

    Commands::who(msg, *chanMgr, *clientMgr, *user1);

    std::string response = user1->popSentData();

    // RFC: 352 (RPL_WHOREPLY) and 315 (RPL_ENDOFWHO)
    CHECK(response.find("352") != std::string::npos);
    CHECK(response.find("Alice") != std::string::npos);
    CHECK(response.find("315") != std::string::npos);
  }
}

TEST_CASE_FIXTURE(IntegrationFixture, "Admin commands") {

  SUBCASE("OPER success") {
    std::string raw = "OPER admin rootpass\r\n";
    IRCMessage msg(raw);

    Commands::oper(msg, *admin);

    std::string response = admin->popSentData();
    // RFC: 381 RPL_YOUREOPER
    CHECK(response.find("381") != std::string::npos);
  }

  SUBCASE("KILL user") {
    // Admin kills Alice
    // we need to verify Admin has privs first (mocking or setting state)
    // let's assume this test runs assuming Admin IS an operator

    std::string raw = "KILL Alice :Spamming\r\n";
    IRCMessage msg(raw);

    // we capture Alice's state before
    // Note: Logic requires Commands::kill to look up Alice in ClientManager
    // and call close() on her.

    Commands::kill(msg, *clientMgr, *admin);

    // So we got:
    // 1. Alice should receive an ERROR message or closing link
    // 2. Alice's fd might be closed (but bs to check with dummy fd)
    // 3. Alice should be removed from ClientManager

    // for now, check if Alice got a message
    std::string aliceMsg = user1->popSentData();
    // RFC says target receives: ERROR :Closing Link...
    CHECK(aliceMsg.find("Closing Link") != std::string::npos);
  }
}
