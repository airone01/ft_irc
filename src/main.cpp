/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elagouch <elagouch@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:46:16 by elagouch          #+#    #+#             */
/*   Updated: 2025/12/08 14:43:24 by elagouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sstream>

#include "Channel.hpp"
#include "ChannelManager.hpp"
#include "Client.hpp"
#include "Server.hpp"

bool doQuit = false;

void handleSigint(int sig){
	if (sig == SIGINT)
		doQuit = true;
}

void sigHandler(){
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handleSigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
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
		std::cerr << "error: unvalid port." << std::endl;
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
