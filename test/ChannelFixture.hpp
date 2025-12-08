#include "../src/Client.hpp"
#include <fcntl.h>
#include <unistd.h>

// fixture to setup an Admin and a User
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md#test-cases-and-subcases
struct ChannelFixture {
  int fd1, fd2;
  Client *admin;
  Client *user;

  ChannelFixture() {
    fd1 = open("/dev/null", O_RDWR);
    fd2 = open("/dev/null", O_RDWR);
    // we cheat and give them different FDs so map insertions work
    admin = new Client(fd1, NULL, NULL);
    user = new Client(fd2, NULL, NULL);

    admin->setNickname("Admin");
    user->setNickname("User");
  }

  ~ChannelFixture() {
    delete admin;
    delete user;
    close(fd1);
    close(fd2);
  }
};
