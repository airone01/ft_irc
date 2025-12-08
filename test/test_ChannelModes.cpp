#include "../src/Channel.hpp"
#include "ChannelFixture.hpp"
#include "doctest.h"

// Advanced channel mode tests

TEST_CASE_FIXTURE(ChannelFixture, "Complex mode parsing") {
  Channel chan(*admin, "#complex");

  SUBCASE("Mixed add/remove with arguments") {
    // +k takes an arg, -l takes no arg, +l takes an arg.
    // running: MODE #complex +k-l+l secret 50
    // expected: key="secret", limit=50.

    std::string raw = "MODE #complex +k-l+l secret 50\r\n";
    IRCMessage msg(raw);
    chan.updateMode(msg, *admin);

    CHECK(chan.getPswrd() == "secret");
    CHECK(chan.getCapacity() == 50);

    std::set<char> modes = chan.getMode();
    CHECK(modes.count('k') == 1);
    CHECK(modes.count('l') == 1);
  }

  SUBCASE("Missing arguments safety") {
    // +k requires an argument. if missing, it should NOT crash.
    // it might throw or just ignore.

    std::string raw = "MODE #complex +k\r\n"; // missing key
    IRCMessage msg(raw);

    try {
      chan.updateMode(msg, *admin);
    } catch (...) {
      // it's okay if it throws, as long as it doesn't segfault
    }
  }

  SUBCASE("Switching polarity") {
    // +i (add), -t (remove), +t (add again)
    std::string raw = "MODE #complex +i-t+t\r\n";
    IRCMessage msg(raw);
    chan.updateMode(msg, *admin);

    std::set<char> modes = chan.getMode();
    CHECK(modes.count('i') == 1);
    CHECK(modes.count('t') == 1);
  }
}
