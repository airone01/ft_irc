#include "../src/Client.hpp"
#include "../src/IRCMessage.hpp"
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

void testClientCreation() {
	printTest("Test 1: Création d'un Client");
	int fd1 = open("/dev/null", O_RDWR);
	int fd2 = 6;
	if (fd1 < 0) {
		printError("Impossible d'ouvrir /dev/null");
		return;
	}
	Client* client = new Client(fd1, NULL, NULL);
	Client* client2 = new Client(fd2, NULL, NULL);
	printInfo("Socket: " + std::to_string(client->getSocket()));
	printInfo("Nickname: '" + client->getNickname() + "'");
	printInfo("Username: '" + client->getUsername() + "'");
	printInfo("Auth: " + std::string(client->getAuth() ? "oui" : "non"));
	printInfo("Registered: " + std::string(client->getRegistered() ? "oui" : "non"));
	if (client->getSocket() == fd1 && client->getNickname().empty() && !client->getAuth() && !client->getRegistered()) {
		printSuccess("Client créé avec les valeurs par défaut correctes");
	} else {
		printError("Valeurs par défaut incorrectes");
	}
	delete client;
	printInfo("Socket: " + std::to_string(client2->getSocket()));
	printInfo("Nickname: '" + client2->getNickname() + "'");
	printInfo("Username: '" + client2->getUsername() + "'");
	printInfo("Auth: " + std::string(client2->getAuth() ? "oui" : "non"));
	printInfo("Registered: " + std::string(client2->getRegistered() ? "oui" : "non"));
	if (client2->getSocket() == fd2 && client2->getNickname().empty() && !client2->getAuth() && !client2->getRegistered()) {
		printSuccess("Client créé avec les valeurs par défaut correctes");
	} else {
		printError("Valeurs par défaut incorrectes");
	}
	delete client2;
	close(fd1);
	close(fd2);
}

void testBufferExtraction() {
	printTest("Test 2: Buffer et extraction de messages");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	client->appendToBuffer("NICK TestUser\r\n");
	printInfo("Ajout: 'NICK TestUser\\r\\n'");
	std::string msg1 = client->extractMessage();
	printInfo("Extrait: '" + msg1 + "'");
	if (msg1 == "NICK TestUser") {
		printSuccess("Message CRLF correctement extrait");
	} else {
		printError("Échec extraction: attendu 'NICK TestUser', reçu '" + msg1 + "'");
	}
	client->appendToBuffer("USER test 0 * :Test User\n");
	std::string msg2 = client->extractMessage();
	if (msg2 == "USER test 0 * :Test User") {
		printSuccess("Message LF correctement extrait");
	} else {
		printError("Échec extraction LF");
	}
	delete client;
	close(fd);
}

void testPartialMessages() {
	printTest("Test 3: Messages partiels (simulation réseau)");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	printInfo("Envoi morceau 1: 'PRIVMSG #ch'");
	client->appendToBuffer("PRIVMSG #ch");
	std::string partial1 = client->extractMessage();
	if (partial1.empty()) {
		printSuccess("Pas de message extrait (normal, incomplet)");
	}
	printInfo("Envoi morceau 2: 'annel :Hell'");
	client->appendToBuffer("annel :Hell");
	std::string partial2 = client->extractMessage();
	if (partial2.empty()) {
		printSuccess("Toujours pas de message (normal)");
	}
	printInfo("Envoi morceau 3: 'o World\\r\\n'");
	client->appendToBuffer("o World\r\n");
	std::string complete = client->extractMessage();
	printInfo("Message final: '" + complete + "'");
	if (complete == "PRIVMSG #channel :Hello World") {
		printSuccess("Message reconstitué correctement");
	} else {
		printError("Reconstitution échouée: '" + complete + "'");
	}
	delete client;
	close(fd);
}

void testMultipleMessages() {
	printTest("Test 4: Plusieurs messages d'un coup");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	std::string batch = "NICK Alice\r\nUSER alice 0 * :Alice\r\nJOIN #test\r\n";
	client->appendToBuffer(batch);
	printInfo("Envoi de 3 messages d'un coup");
	int count = 0;
	std::string messages[3];
	while (true) {
		std::string msg = client->extractMessage();
		if (msg.empty()) break;
		if (count < 3) {
			messages[count] = msg;
		}
		count++;
		printInfo("Message " + std::to_string(count) + ": '" + msg + "'");
	}
	if (count == 3 &&
		messages[0] == "NICK Alice" &&
		messages[1] == "USER alice 0 * :Alice" &&
		messages[2] == "JOIN #test") {
		printSuccess("3 messages extraits correctement");
	} else {
		printError("Échec: " + std::to_string(count) + " messages extraits");
	}
	delete client;
	close(fd);
}

void testIRCParsing() {
	printTest("Test 5: Parsing IRC avec IRCMessage");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	client->appendToBuffer("PRIVMSG #channel :Hello World\r\n");
	std::string rawMsg = client->extractMessage();
	printInfo("Message brut: '" + rawMsg + "'");
	try {
		IRCMessage msg(rawMsg);
		printInfo("Commande: " + msg.getCommand());
		if (!msg.getParams().empty()) {
			printInfo("Paramètre: " + msg.getParams()[0]);
		}
		printInfo("Trailing: " + msg.getTrailing());

		if (msg.getCommand() == "PRIVMSG" &&
			msg.getParams()[0] == "#channel" &&
			msg.getTrailing() == "Hello World") {
			printSuccess("Message IRC correctement parsé");
		} else {
			printError("Parsing incorrect");
		}
	} catch (const std::exception& e) {
		printError(std::string("Exception: ") + e.what());
	}
	delete client;
	close(fd);
}

void testRegistrationWorkflow() {
	printTest("Test 6: Workflow d'enregistrement (PASS/NICK/USER)");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	printInfo("État initial:");
	printInfo("  Auth: " + std::string(client->getAuth() ? "✓" : "✗"));
	printInfo("  Registered: " + std::string(client->getRegistered() ? "✓" : "✗"));
	client->setAuth(true);
	printInfo("\nAprès PASS:");
	printInfo("  Auth: " + std::string(client->getAuth() ? "✓" : "✗"));
	client->setNickname("MaxTest");
	printInfo("\nAprès NICK:");
	printInfo("  Nickname: " + client->getNickname());
	client->setUsername("maxime");
	printInfo("\nAprès USER:");
	printInfo("  Username: " + client->getUsername());
	bool canRegister = client->getAuth() &&
					   !client->getNickname().empty() &&
					   !client->getUsername().empty();
	if (canRegister) {
		client->setRegistered(true);
		printInfo("\nÉtat final:");
		printInfo("  Auth: ✓");
		printInfo("  Nickname: " + client->getNickname());
		printInfo("  Username: " + client->getUsername());
		printInfo("  Registered: ✓");
		printSuccess("Client enregistré avec succès");
	} else {
		printError("Impossible d'enregistrer le client");
	}
	delete client;
	close(fd);
}

void testBufferResidue() {
	printTest("Test 7: Buffer avec données résiduelles");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	client->appendToBuffer("NICK Alice\r\nUSER alice 0");
	std::string msg1 = client->extractMessage();
	printInfo("Premier message: '" + msg1 + "'");
	if (msg1 == "NICK Alice") {
		printSuccess("Premier message extrait");
	}
	std::string msg2 = client->extractMessage();
	if (msg2.empty()) {
		printSuccess("Deuxième message incomplet (normal)");
	}
	client->appendToBuffer(" * :Alice\r\n");
	std::string msg3 = client->extractMessage();
	printInfo("Message complété: '" + msg3 + "'");
	if (msg3 == "USER alice 0 * :Alice") {
		printSuccess("Message complété correctement");
	} else {
		printError("Échec: '" + msg3 + "'");
	}
	delete client;
	close(fd);
}

void testEmptyBuffer() {
	printTest("Test 8: Extraction sur buffer vide");
	int fd = open("/dev/null", O_RDWR);
	Client* client = new Client(fd, NULL, NULL);
	std::string msg = client->extractMessage();
	if (msg.empty()) {
		printSuccess("Buffer vide retourne chaîne vide");
	} else {
		printError("Buffer vide retourne: '" + msg + "'");
	}
	delete client;
	close(fd);
}

int main() {
	testClientCreation();
	testBufferExtraction();
	testPartialMessages();
	testMultipleMessages();
	testIRCParsing();
	testRegistrationWorkflow();
	testBufferResidue();
	testEmptyBuffer();
	return 0;
}