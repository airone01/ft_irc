#include "../src/Client.hpp"
#include "../src/ClientManager.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

void printTest(const std::string& name) {
	std::cout << "\n" << BLUE << "/// " << name << " ///" << RESET << std::endl;
}

void printSuccess(const std::string& msg) {
	std::cout << GREEN << "V " << msg << RESET << std::endl;
}

void printError(const std::string& msg) {
	std::cout << RED << "X " << msg << RESET << std::endl;
}

void printInfo(const std::string& msg) {
	std::cout << YELLOW << "  " << msg << RESET << std::endl;
}

void testClientManagerBasics() {
	printTest("Test 1: ClientManager - Ajout et récupération");
	ClientManager manager;
	int fd1 = open("/dev/null", O_RDWR);
	int fd2 = open("/dev/null", O_RDWR);
	int fd3 = open("/dev/null", O_RDWR);
	Client client1(fd1, NULL, NULL);
	client1.setNickname("Alice");
	client1.setUsername("alice");
	Client client2(fd2, NULL, NULL);
	client2.setNickname("Bob");
	client2.setUsername("bob");
	Client client3(fd3, NULL, NULL);
	client3.setNickname("Charlie");
	client3.setUsername("charlie");
	manager.getClients().push_back(client1);
	manager.getClients().push_back(client2);
	manager.getClients().push_back(client3);
	printInfo("Clients ajoutés: 3");
	printInfo("Nombre total: " + std::to_string(manager.getClients().size()));
	if (manager.getClients().size() == 3) {
		printSuccess("3 clients dans le manager");
	} else {
		printError("Nombre de clients incorrect: " + std::to_string(manager.getClients().size()));
	}
	bool allNicksOk = true;
	std::vector<Client>& clients = manager.getClients();
	if (clients[0].getNickname() != "Alice" ||
		clients[1].getNickname() != "Bob" ||
		clients[2].getNickname() != "Charlie") {
		allNicksOk = false;
	}
	if (allNicksOk) {
		printSuccess("Tous les nicknames sont corrects");
	} else {
		printError("Nicknames incorrects");
	}
	close(fd1);
	close(fd2);
	close(fd3);
}

void testGetClientByUsername() {
	printTest("Test 2: ClientManager - Récupération par username");
	ClientManager manager;
	int fd1 = open("/dev/null", O_RDWR);
	int fd2 = open("/dev/null", O_RDWR);
	Client client1(fd1, NULL, NULL);
	client1.setNickname("Alice");
	client1.setUsername("alice");
	Client client2(fd2, NULL, NULL);
	client2.setNickname("Bob");
	client2.setUsername("bob");
	manager.getClients().push_back(client1);
	manager.getClients().push_back(client2);
	try {
		std::string searchNick = "Alice";
		Client& found = manager.getClientFromUsername(searchNick);
		printInfo("Client trouvé: " + found.getNickname());
		if (found.getNickname() == "Alice" && found.getUsername() == "alice") {
			printSuccess("Client Alice trouvé correctement");
		} else {
			printError("Client trouvé mais données incorrectes");
		}
	} catch (const std::exception& e) {
		printError("Exception lors de la recherche: " + std::string(e.what()));
	}
	try {
		std::string searchNick = "NonExistent";
		Client& found = manager.getClientFromUsername(searchNick);
		(void)found;
		printError("Client inexistant trouvé (ne devrait pas arriver)");
	} catch (const std::exception& e) {
		printSuccess("Exception correcte pour client inexistant: " + std::string(e.what()));
	}
	close(fd1);
	close(fd2);
}

void testGetClientBySocket() {
	printTest("Test 3: ClientManager - Récupération par socket");
	ClientManager manager;
	int fd1 = 42;
	int fd2 = 100;
	Client client1(fd1, NULL, NULL);
	client1.setNickname("Alice");
	Client client2(fd2, NULL, NULL);
	client2.setNickname("Bob");
	manager.getClients().push_back(client1);
	manager.getClients().push_back(client2);
	try {
		int searchSocket = 42;
		Client& found = manager.getClientFromSocket(searchSocket);
		printInfo("Client trouvé avec socket " + std::to_string(searchSocket));
		printInfo("Nickname: " + found.getNickname());
		if (found.getSocket() == 42 && found.getNickname() == "Alice") {
			printSuccess("Client avec socket 42 trouvé correctement");
		} else {
			printError("Client trouvé mais données incorrectes");
		}
	} catch (const std::exception& e) {
		printError("Exception lors de la recherche: " + std::string(e.what()));
	}
	try {
		int searchSocket = 999;
		Client& found = manager.getClientFromSocket(searchSocket);
		(void)found;
		printError("Socket inexistant trouvé (ne devrait pas arriver)");
	} catch (const std::exception& e) {
		printSuccess("Exception correcte pour socket inexistant: " + std::string(e.what()));
	}
}

void testRealisticScenario() {
	printTest("Test 4: Scénario réaliste - Connexions multiples");
	ClientManager manager;
	printInfo("Simulation de connexion de 3 clients...");
	int fd1 = open("/dev/null", O_RDWR);
	Client client1(fd1, NULL, NULL);
	manager.getClients().push_back(client1);
	printInfo("Client 1 connecté (fd=" + std::to_string(fd1) + ")");
	manager.getClients()[0].setAuth(true);
	manager.getClients()[0].setNickname("Alice");
	manager.getClients()[0].setUsername("alice");
	manager.getClients()[0].setRegistered(true);
	printInfo("Client 1 enregistré: Alice");
	int fd2 = open("/dev/null", O_RDWR);
	Client client2(fd2, NULL, NULL);
	manager.getClients().push_back(client2);
	printInfo("Client 2 connecté (fd=" + std::to_string(fd2) + ")");
	manager.getClients()[1].setAuth(true);
	manager.getClients()[1].setNickname("Bob");
	manager.getClients()[1].setUsername("bob");
	manager.getClients()[1].setRegistered(true);
	printInfo("Client 2 enregistré: Bob");
	int fd3 = open("/dev/null", O_RDWR);
	Client client3(fd3, NULL, NULL);
	manager.getClients().push_back(client3);
	printInfo("Client 3 connecté (fd=" + std::to_string(fd3) + ")");
	manager.getClients()[2].setAuth(true);
	manager.getClients()[2].setNickname("Charlie");
	manager.getClients()[2].setUsername("charlie");
	manager.getClients()[2].setRegistered(true);
	printInfo("Client 3 enregistré: Charlie");
	printInfo("\nVérification finale:");
	printInfo("  Nombre de clients: " + std::to_string(manager.getClients().size()));
	bool allRegistered = true;
	for (size_t i = 0; i < manager.getClients().size(); i++) {
		if (!manager.getClients()[i].getRegistered()) {
			allRegistered = false;
			break;
		}
	}
	if (manager.getClients().size() == 3 && allRegistered) {
		printSuccess("Scénario réaliste: 3 clients connectés et enregistrés");
	} else {
		printError("Problème dans le scénario");
	}
	try {
		std::string searchNick = "Bob";
		Client& bob = manager.getClientFromUsername(searchNick);
		if (bob.getNickname() == "Bob") {
			printSuccess("Recherche de Bob dans le contexte multi-clients OK");
		}
	} catch (...) {
		printError("Impossible de trouver Bob");
	}
	close(fd1);
	close(fd2);
	close(fd3);
}

void testMultipleSearches() {
	printTest("Test 5: Recherches multiples simultanées");
	ClientManager manager;
	for (int i = 0; i < 5; i++) {
		int fd = 100 + i;
		Client client(fd, NULL, NULL);
		client.setNickname("User" + std::to_string(i));
		client.setUsername("user" + std::to_string(i));
		manager.getClients().push_back(client);
		printInfo(std::to_string(client.getSocket()));
		close(fd);
	}
	printInfo("5 clients créés: User0, User1, User2, User3, User4");
	int found_count = 0;
	for (int i = 0; i < 5; i++) {
		try {
			std::string searchNick = "User" + std::to_string(i);
			Client& found = manager.getClientFromUsername(searchNick);
			if (found.getNickname() == searchNick) {
				found_count++;
			}
		} catch (...) {}
	}
	if (found_count == 5) {
		printSuccess("Tous les clients retrouvés: " + std::to_string(found_count) + "/5");
	} else {
		printError("Certains clients non trouvés: " + std::to_string(found_count) + "/5");
	}
}

int main() {
	testClientManagerBasics();
	testGetClientByUsername();
	testGetClientBySocket();
	testRealisticScenario();
	testMultipleSearches();
	return 0;
}