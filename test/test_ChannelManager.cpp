#include "../src/Client.hpp"
#include "../src/ClientManager.hpp"
#include "../src/Channel.hpp"
#include "../src/ChannelManager.hpp"
#include <iostream>
#include <string>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define RESET "\033[0m"

void printSuccess(const std::string& msg) {
	std::cout << GREEN << "  ✓ " << msg << RESET << std::endl;
}

void printError(const std::string& msg) {
	std::cout << RED << "  ✗ " << msg << RESET << std::endl;
}

void printInfo(const std::string& msg) {
	std::cout << YELLOW << "    → " << msg << RESET << std::endl;
}

void testChannelCreation() {
	int fd = 42;
	Client creator(fd, NULL, NULL);
	creator.setNickname("Alice");
	creator.setUsername("alice");
	creator.setRegistered(true);
	printInfo("Créateur du channel :");
	printInfo("  Nickname : " + creator.getNickname());
	printInfo("  Socket   : " + std::to_string(creator.getSocket()));
	std::string channelName = "#general";
	Channel channel(creator, channelName);
	printInfo("\nChannel créé :");
	printInfo("  Nom : " + channel.getName());
	if (channel.getName() == channelName) {
		printSuccess("Channel créé avec le bon nom");
	} else {
		printError("Nom du channel incorrect");
	}
	if (channel.isOperator(creator)) {
		printSuccess("Le créateur est automatiquement opérateur");
	} else {
		printError("Le créateur devrait être opérateur");
	}
	std::map<int, Client*>& users = channel.getUsers();
	printInfo("\nNombre d'utilisateurs : " + std::to_string(users.size()));
	if (users.size() >= 1) {
		printSuccess("Le créateur est dans la liste des utilisateurs");
	} else {
		printError("Le créateur n'est pas dans la liste");
	}
}

void testAddingUsers() {
	Client creator(42, NULL, NULL);
	creator.setNickname("Alice");
	creator.setUsername("alice");
	creator.setRegistered(true);
	Channel channel(creator, "#general");
	printInfo("Channel #general créé par Alice");
	Client bob(43, NULL, NULL);
	bob.setNickname("Bob");
	bob.setUsername("bob");
	bob.setRegistered(true);
	channel.newUser(bob);
	printInfo("Bob a rejoint #general");
	std::map<int, Client*>& users = channel.getUsers();
	printInfo("Nombre d'utilisateurs : " + std::to_string(users.size()));
	if (users.size() == 2) {
		printSuccess("Bob ajouté avec succès (2 utilisateurs)");
	} else {
		printError("Problème lors de l'ajout de Bob");
	}
	if (!channel.isOperator(bob)) {
		printSuccess("Bob n'est pas opérateur (normal)");
	} else {
		printError("Bob ne devrait pas être opérateur");
	}
	Client charlie(44, NULL, NULL);
	charlie.setNickname("Charlie");
	charlie.setUsername("charlie");
	charlie.setRegistered(true);
	channel.newUser(charlie);
	printInfo("Charlie a rejoint #general");
	if (users.size() == 3) {
		printSuccess("Charlie ajouté avec succès (3 utilisateurs)");
	} else {
		printError("Problème lors de l'ajout de Charlie");
	}
}

void testChannelTopic() {
	Client creator(42, NULL, NULL);
	creator.setNickname("Alice");
	creator.setRegistered(true);
	Channel channel(creator, "#general");
	printInfo("Topic initial : '" + channel.getTopic() + "'");
	std::string newTopic = "Bienvenue sur #general !";
	channel.setTopic(newTopic);
	printInfo("Nouveau topic : '" + channel.getTopic() + "'");
	if (channel.getTopic() == newTopic) {
		printSuccess("Topic modifié avec succès");
	} else {
		printError("Échec de la modification du topic");
	}
	std::string anotherTopic = "Channel de discussion général";
	channel.setTopic(anotherTopic);
	printInfo("Topic mis à jour : '" + channel.getTopic() + "'");
	if (channel.getTopic() == anotherTopic) {
		printSuccess("Topic mis à jour correctement");
	} else {
		printError("Échec de la mise à jour du topic");
	}
}

void testChannelPassword() {
	Client creator(42, NULL, NULL);
	creator.setNickname("Alice");
	creator.setRegistered(true);
	Channel channel(creator, "#private");
	printInfo("Channel privé : #private");
	std::string password = "secret123";
	channel.setPswrd(password);
	printInfo("Mot de passe défini : '" + password + "'");
	if (channel.getPswrd() == password) {
		printSuccess("Mot de passe enregistré correctement");
	} else {
		printError("Problème avec le mot de passe");
	}
	std::string retrievedPwd = channel.getPswrd();
	printInfo("Mot de passe récupéré : '" + retrievedPwd + "'");
	if (retrievedPwd == password) {
		printSuccess("Mot de passe récupéré avec succès");
	} else {
		printError("Le mot de passe récupéré est incorrect");
	}
}

void testOperatorPrivileges() {
	Client alice(42, NULL, NULL);
	alice.setNickname("Alice");
	alice.setRegistered(true);
	Channel channel(alice, "#ops");
	printInfo("Channel #ops créé par Alice");
	Client bob(43, NULL, NULL);
	bob.setNickname("Bob");
	bob.setRegistered(true);
	channel.newUser(bob);
	printInfo("Bob a rejoint #ops");
	if (channel.isOperator(alice)) {
		printSuccess("Alice est opérateur (créateur du channel)");
	} else {
		printError("Alice devrait être opérateur");
	}
	if (!channel.isOperator(bob)) {
		printSuccess("Bob n'est pas opérateur (utilisateur normal)");
	} else {
		printError("Bob ne devrait pas être opérateur");
	}
	channel.updatePriv(alice, bob);
	printInfo("Alice donne les privilèges opérateur à Bob");
	if (channel.isOperator(bob)) {
		printSuccess("Bob est maintenant opérateur");
	} else {
		printError("Bob devrait maintenant être opérateur");
	}
}

void testGetClientInChannel() {
	Client creator(42, NULL, NULL);
	creator.setNickname("Alice");
	creator.setRegistered(true);
	Channel channel(creator, "#search");
	Client bob(43, NULL, NULL);
	bob.setNickname("Bob");
	bob.setRegistered(true);
	channel.newUser(bob);
	Client charlie(44, NULL, NULL);
	charlie.setNickname("Charlie");
	charlie.setRegistered(true);
	channel.newUser(charlie);
	printInfo("Channel #search avec 3 utilisateurs : Alice, Bob, Charlie");
	Client* foundAlice = channel.getClientInChannel("Alice");
	if (foundAlice && foundAlice->getNickname() == "Alice") {
		printSuccess("Alice trouvée dans le channel");
		printInfo("  Socket : " + std::to_string(foundAlice->getSocket()));
	} else {
		printError("Alice non trouvée");
	}
	Client* foundBob = channel.getClientInChannel("Bob");
	if (foundBob && foundBob->getNickname() == "Bob") {
		printSuccess("Bob trouvé dans le channel");
		printInfo("  Socket : " + std::to_string(foundBob->getSocket()));
	} else {
		printError("Bob non trouvé");
	}
	Client* notFound = channel.getClientInChannel("David");
	if (notFound == NULL) {
		printSuccess("Utilisateur inexistant correctement non trouvé (NULL)");
	} else {
		printError("Un utilisateur inexistant a été retourné");
	}
}

void testLeaveChannel() {
	Client creator(42, NULL, NULL);
	creator.setNickname("Alice");
	creator.setRegistered(true);
	Channel channel(creator, "#leave");
	Client bob(43, NULL, NULL);
	bob.setNickname("Bob");
	bob.setRegistered(true);
	channel.newUser(bob);
	Client charlie(44, NULL, NULL);
	charlie.setNickname("Charlie");
	charlie.setRegistered(true);
	channel.newUser(charlie);
	printInfo("3 utilisateurs dans #leave : Alice, Bob, Charlie");
	std::map<int, Client*>& users = channel.getUsers();
	printInfo("Nombre d'utilisateurs : " + std::to_string(users.size()));
	channel.leaveChannel(bob);
	printInfo("Bob a quitté #leave");
	printInfo("Nombre d'utilisateurs : " + std::to_string(users.size()));
	if (users.size() == 2) {
		printSuccess("Bob a bien quitté le channel (2 utilisateurs restants)");
	} else {
		printError("Problème lors du départ de Bob");
	}
	Client* stillThere = channel.getClientInChannel("Bob");
	if (stillThere == NULL) {
		printSuccess("Bob n'est plus dans le channel");
	} else {
		printError("Bob est encore dans le channel");
	}
}

void testChannelManagerBasics() {
	ChannelManager manager;
	printInfo("Création de 3 channels...");
	Client alice(42, NULL, NULL);
	alice.setNickname("Alice");
	alice.setRegistered(true);
	Channel channel1(alice, "#general");
	manager.addChannels(channel1);
	printInfo("  ✓ #general créé par Alice");
	Client bob(43, NULL, NULL);
	bob.setNickname("Bob");
	bob.setRegistered(true);
	Channel channel2(bob, "#random");
	manager.addChannels(channel2);
	printInfo("  ✓ #random créé par Bob");
	Client charlie(44, NULL, NULL);
	charlie.setNickname("Charlie");
	charlie.setRegistered(true);
	Channel channel3(charlie, "#help");
	manager.addChannels(channel3);
	printInfo("  ✓ #help créé par Charlie");
	std::vector<Channel> channels = manager.getChannels();
	printInfo("\nNombre de channels : " + std::to_string(channels.size()));
	if (channels.size() == 3) {
		printSuccess("3 channels correctement ajoutés");
	} else {
		printError("Nombre de channels incorrect : " + std::to_string(channels.size()));
	}
	printInfo("\nListe des channels :");
	for (size_t i = 0; i < channels.size(); i++) {
		printInfo("  " + std::to_string(i + 1) + ". " + channels[i].getName());
	}
}

void testGetChannelByName() {
	ChannelManager manager;
	Client alice(42, NULL, NULL);
	alice.setNickname("Alice");
	alice.setRegistered(true);
	Channel general(alice, "#general");
	manager.addChannels(general);
	Channel random(alice, "#random");
	manager.addChannels(random);
	printInfo("Channels créés : #general, #random");
	try {
		std::string searchName = "#general";
		Channel& found = manager.getChannelFromName(searchName);
		printInfo("  Trouvé : " + found.getName());
		printInfo("  Topic  : " + found.getTopic());
		if (found.getName() == "#general") {
			printSuccess("#general trouvé correctement");
		} else {
			printError("Channel incorrect trouvé");
		}
	} catch (const std::exception& e) {
		printError("Exception : " + std::string(e.what()));
	}
	try {
		std::string searchName = "#inexistant";
		Channel& found = manager.getChannelFromName(searchName);
		(void)found;
		printError("Channel inexistant trouvé (ne devrait pas arriver)");
	} catch (const std::exception& e) {
		printSuccess("Exception attendue : " + std::string(e.what()));
	}
}

void testRemoveChannel() {
	ChannelManager manager;
	Client alice(42, NULL, NULL);
	alice.setNickname("Alice");
	alice.setRegistered(true);
	Channel general(alice, "#general");
	Channel random(alice, "#random");
	Channel help(alice, "#help");
	manager.addChannels(general);
	manager.addChannels(random);
	manager.addChannels(help);
	printInfo("3 channels créés : #general, #random, #help");
	printInfo("Nombre initial : " + std::to_string(manager.getChannels().size()));
	manager.rmChannels(random);
	printInfo("Nombre après suppression : " + std::to_string(manager.getChannels().size()));
	if (manager.getChannels().size() == 2) {
		printSuccess("#random supprimé (2 channels restants)");
	} else {
		printError("Problème lors de la suppression");
	}
	try {
		std::string searchName = "#random";
		Channel& found = manager.getChannelFromName(searchName);
		(void)found;
		printError("#random encore accessible après suppression");
	} catch (...) {
		printSuccess("#random n'est plus accessible");
	}
}

void testRealisticIRCScenario() {
	printInfo("Simulation d'un serveur IRC avec plusieurs utilisateurs et channels\n");
	ClientManager clientMgr;
	ChannelManager channelMgr;
	Client alice(42, NULL, NULL);
	alice.setNickname("Alice");
	alice.setUsername("alice");
	alice.setAuth(true);
	alice.setRegistered(true);
	clientMgr.getClients().push_back(alice);
	printInfo("Alice connectée (opérateur expérimenté)");
	Client bob(43, NULL, NULL);
	bob.setNickname("Bob");
	bob.setUsername("bob");
	bob.setAuth(true);
	bob.setRegistered(true);
	clientMgr.getClients().push_back(bob);
	printInfo("Bob connecté (utilisateur régulier)");
	Client charlie(44, NULL, NULL);
	charlie.setNickname("Charlie");
	charlie.setUsername("charlie");
	charlie.setAuth(true);
	charlie.setRegistered(true);
	clientMgr.getClients().push_back(charlie);
	printInfo("Charlie connecté (nouveau venu)");
	printSuccess(std::to_string(clientMgr.getClients().size()) + " clients connectés");
	Channel general(alice, "#general");
	general.setTopic("Discussions générales");
	channelMgr.addChannels(general);
	printInfo("#general créé par Alice");
	printInfo("  Topic : " + general.getTopic());
	Channel coding(bob, "#coding");
	coding.setTopic("Programmation et développement");
	channelMgr.addChannels(coding);
	printInfo("#coding créé par Bob");
	printInfo("  Topic : " + coding.getTopic());
	printSuccess(std::to_string(channelMgr.getChannels().size()) + " channels créés");
	std::string generalName = "#general";
	Channel& generalRef = channelMgr.getChannelFromName(generalName);
	generalRef.newUser(bob);
	printInfo("Bob a rejoint #general");
	printInfo("Utilisateurs dans #general : " + std::to_string(generalRef.getUsers().size()));
	generalRef.newUser(charlie);
	printInfo("Charlie a rejoint #general");
	std::string codingName = "#coding";
	Channel& codingRef = channelMgr.getChannelFromName(codingName);
	codingRef.newUser(charlie);
	printInfo("Charlie a rejoint #coding");
	printInfo("\nUtilisateurs dans #general : " + std::to_string(generalRef.getUsers().size()));
	printInfo("Utilisateurs dans #coding  : " + std::to_string(codingRef.getUsers().size()));
	generalRef.updatePriv(alice, bob);
	printInfo("Bob a reçu les privilèges opérateur");
	if (generalRef.isOperator(bob)) {
		printSuccess("Bob est maintenant opérateur sur #general");
	} else {
		printError("Bob n'est pas opérateur");
	}
	printInfo("Clients connectés : " + std::to_string(clientMgr.getClients().size()));
	printInfo("Channels actifs   : " + std::to_string(channelMgr.getChannels().size()));
	printInfo("\nDétails #general :");
	printInfo("  Utilisateurs  : " + std::to_string(generalRef.getUsers().size()));
	printInfo("  Opérateurs    : Alice, Bob");
	printInfo("  Topic         : " + generalRef.getTopic());
	printInfo("\nDétails #coding :");
	printInfo("  Utilisateurs  : " + std::to_string(codingRef.getUsers().size()));
	printInfo("  Opérateur     : Bob");
	printInfo("  Topic         : " + codingRef.getTopic());
	printSuccess("Scénario IRC réaliste complété avec succès !");
}

void testMultiChannelManagement() {
	ClientManager clientMgr;
	ChannelManager channelMgr;
	Client admin(100, NULL, NULL);
	admin.setNickname("Admin");
	admin.setRegistered(true);
	printInfo("Création de 5 channels thématiques...\n");
	std::string channelNames[] = {"#general", "#help", "#random", "#coding", "#gaming"};
	std::string topics[] = {
		"Discussions générales",
		"Aide et support",
		"Discussions libres",
		"Programmation",
		"Jeux vidéo"
	};
	for (int i = 0; i < 5; i++) {
		Channel ch(admin, channelNames[i]);
		ch.setTopic(topics[i]);
		channelMgr.addChannels(ch);
		printInfo("  " + std::to_string(i + 1) + ". " + channelNames[i] + " → " + topics[i]);
	}
	printSuccess("5 channels créés et configurés");
	int found = 0;
	for (int i = 0; i < 5; i++) {
		try {
			std::string name = channelNames[i];
			Channel& ch = channelMgr.getChannelFromName(name);
			if (ch.getName() == name) {
				found++;
				printInfo("  ✓ " + name + " trouvé");
			}
		} catch (...) {
			printError("  ✗ " + channelNames[i] + " non trouvé");
		}
	}
	if (found == 5) {
		printSuccess("Tous les channels sont accessibles : " + std::to_string(found) + "/5");
	} else {
		printError("Seulement " + std::to_string(found) + "/5 channels trouvés");
	}
}
int main() {
	testChannelCreation();
	testAddingUsers();
	testChannelTopic();
	testChannelPassword();
	testOperatorPrivileges();
	testGetClientInChannel();
	testLeaveChannel();
	testChannelManagerBasics();
	testGetChannelByName();
	testRemoveChannel();
	testRealisticIRCScenario();
	testMultiChannelManagement();
	return 0;
}