#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sstream>

#include "Server.hpp"

bool doQuit = false;

void handleSigint(int sig){
	if (sig == SIGINT)
		doQuit = true;
}

void sigHandler(){
	struct sigaction sa = {};
	sa.sa_handler = handleSigint;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	std::istringstream ss(argv[1]);
	unsigned short port; ss >> port;
	if (!ss.eof()){
		std::cerr << "error: invalid port." << std::endl;
		return 1;
	}
	sigHandler();
	try
	{
		Server serv(port, argv[2]);
		serv.serverRoutine();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
