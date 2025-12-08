#include "../src/IRCMessage.hpp"
#include "doctest.h"
#include <string>
#include <vector>

TEST_CASE("RFC 1459 message parsing") {

  SUBCASE("Simple command") {
    std::string raw = "PING :12345\r\n";
    IRCMessage msg(raw);
    CHECK(msg.getCommand() == "PING");
    CHECK(msg.getParams().empty());
    CHECK(msg.getTrailing() == "12345");
  }

  SUBCASE("Command w/ parameters and trailing") {
    std::string raw = "PRIVMSG #test :Hello World\r\n";
    IRCMessage msg(raw);
    CHECK(msg.getCommand() == "PRIVMSG");
    CHECK(msg.getParams().size() == 1);
    CHECK(msg.getParams()[0] == "#test");
    CHECK(msg.getTrailing() == "Hello World");
  }

  SUBCASE("Message with prefix") {
    std::string raw = ":nick!user@host JOIN #channel\r\n";
    IRCMessage msg(raw);
    CHECK(msg.getPrefix() == "nick!user@host");
    CHECK(msg.getCommand() == "JOIN");
    CHECK(msg.getParams()[0] == "#channel");
  }

  SUBCASE("Complex trailing (colon inside message)") {
    std::string raw =
        "PRIVMSG #channel :This is a colon : inside a message\r\n";
    IRCMessage msg(raw);
    CHECK(msg.getTrailing() == "This is a colon : inside a message");
  }

  SUBCASE("Multiple parameters") {
    std::string raw = "MODE #channel +ok user1 key123\r\n";
    IRCMessage msg(raw);
    CHECK(msg.getCommand() == "MODE");
    CHECK(msg.getParams()[0] == "#channel");
    CHECK(msg.getParams()[1] == "+ok");
    CHECK(msg.getParams()[2] == "user1");
    CHECK(msg.getParams()[3] == "key123");
  }
}

TEST_CASE("Edge Cases") {
  SUBCASE("Empty Message") {
    std::string raw = "";
    CHECK_THROWS_AS(IRCMessage msg(raw), IRCMessage::MsgEmptyException);
  }

  SUBCASE("Whitespaces handling") {
    // RFC says multiple spaces should be treated as one separator, unless
    // inside the trailing section.
    std::string raw = "PRIVMSG    #test    :spaced   out   message\r\n";
    IRCMessage msg(raw);
    CHECK(msg.getCommand() == "PRIVMSG");
    CHECK(msg.getParams()[0] == "#test");
    CHECK(msg.getTrailing() == "spaced   out   message");
  }
}

TEST_CASE("IRCMessage hard tests") {
  SUBCASE("Colon as a parameter (not trailing)") {
    // "RESTART" command often looks like: RESTART :
    // but also: PRIVMSG #chan : :D
    std::string raw = "PRIVMSG #chan ::D\r\n";
    IRCMessage msg(raw);

    CHECK(msg.getCommand() == "PRIVMSG");
    CHECK(msg.getParams().size() == 1);
    CHECK(msg.getParams()[0] == "#chan");
    CHECK(msg.getTrailing() == ":D"); // should capture the smiley
  }

  SUBCASE("Only Command") {
    std::string raw = "QUIT\r\n";
    IRCMessage msg(raw);

    CHECK(msg.getCommand() == "QUIT");
    CHECK(msg.getParams().empty());
    CHECK(msg.getTrailing().empty());
  }

  SUBCASE("Prefix with no parameters") {
    std::string raw = ":prefix COMMAND\r\n";
    IRCMessage msg(raw);

    CHECK(msg.getPrefix() == "prefix");
    CHECK(msg.getCommand() == "COMMAND");
  }

  SUBCASE("Multiple Spaces in Trailing") {
    // Trailing should PRESERVE spaces
    std::string raw = "PRIVMSG #chan :   keep   these   spaces   \r\n";
    IRCMessage msg(raw);

    CHECK(msg.getTrailing() == "   keep   these   spaces   ");
  }
}
