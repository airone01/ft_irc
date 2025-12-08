#include "../src/Channel.hpp"
#include "../src/Client.hpp"
#include "../src/IRCMessage.hpp"
#include "ChannelFixture.hpp"
#include "doctest.h"
#include <fcntl.h>
#include <unistd.h>

TEST_CASE_FIXTURE(ChannelFixture, "RFC 1459 channel modes and logic") {
  Channel chan(*admin, "#test");

  SUBCASE("Basic join/part") {
    CHECK(chan.getName() == "#test");
    CHECK(chan.getUsers().size() == 1);

    chan.newUser(*user);
    CHECK(chan.getUsers().size() == 2);

    chan.leaveChannel(*user);
    CHECK(chan.getUsers().size() == 1);
  }

  SUBCASE("OP privileges") {
    // ddmin should be OP
    Client user2(999, NULL, NULL);

    // admin sets topic (should be allowed)
    chan.setTopic("New Topic");
    CHECK(chan.getTopic() == "New Topic");

    // mode +t (topic lock)
    chan.setMode('t');
    chan.setModeSet(true);

    // normal user tries to change topic (via updateMode logic simulation)
    // note: We are testing the Exception logic here
    std::string raw = "TOPIC #test :Hacker Topic\r\n";
    IRCMessage msg(raw);

    // should throw because 'user' is not admin and +t is set
    CHECK_THROWS_AS(chan.changeTopic(msg, *user), Channel::errorTopic);
  }

  SUBCASE("Mode +k") {
    chan.setMode('k');
    chan.setPswrd("secret");
    chan.setModeSet(true);

    // wrong password
    CHECK_THROWS_AS(chan.tryJoin(*user, "wrong"), Channel::errorMode);
    // correct password
    chan.tryJoin(*user, "secret");
  }

  SUBCASE("Mode +l") {
    chan.setMode('l');
    chan.setCapacity(1); // limit to 1 user
    chan.setModeSet(true);

    // channel already has 1 admin
    CHECK_THROWS_AS(chan.tryJoin(*user, ""), Channel::errorMode);

    chan.setCapacity(2);     // increase limit
    chan.tryJoin(*user, ""); // should not throw now
  }

  SUBCASE("Mode +i") {
    chan.setMode('i');
    chan.setModeSet(true);

    // not invited
    CHECK_THROWS_AS(chan.tryJoin(*user, ""), Channel::errorMode);

    chan.setInvitedUsers(*user); // invite user
    chan.tryJoin(*user, "");     // should work
  }

  SUBCASE("Kicking") {
    chan.newUser(*user);

    // admin kicks User
    std::vector<std::string> params;
    params.push_back("#test");
    params.push_back("User");

    // need dummy message for the function signature
    std::string raw = "KICK #test User\r\n";
    IRCMessage msg(raw);

    // should not throw when admin kicks
    chan.tryKick(params, msg, *admin);
    // user tries to kick admin which should throw
    CHECK_THROWS_AS(chan.tryKick(params, msg, *user), Channel::errorKick);
  }
}
