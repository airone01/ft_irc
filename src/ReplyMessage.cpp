#include "ReplyMessage.hpp"

void ReplyMessage::errNoSuchNick(const std::string& arg) {
	std::cout << "401 ERR_NOSUCHNICK " << arg << " :No such nick/channel" << std::endl;
}

void ReplyMessage::errNoSuchServer(const std::string& arg) {
	std::cout << "402 ERR_NOSUCHSERVER " << arg << " :No such server" << std::endl;
}

void ReplyMessage::errNoSuckChannel(const std::string& arg) {
	std::cout << "403 ERR_NOSUCHCHANNEL " << arg << " :No such channel" << std::endl;
}

void ReplyMessage::errCannotSendToChannel(const std::string& arg) {
	std::cout << "404 ERR_CANNOTSENDTOCHAN " << arg << " :Cannot send to channel" << std::endl;
}

void ReplyMessage::errTooManyChannels(const std::string& arg) {
	std::cout << "405 ERR_TOOMANYCHANNELS " << arg << " :You have joined too many channels" << std::endl;
}

void ReplyMessage::errWasNoSuckNick(const std::string& arg) {
	std::cout << "406 ERR_WASNOSUCHNICK " << arg << " :There was no such nickname" << std::endl;
}

void ReplyMessage::errTooManyTargets(const std::string& arg) {
	std::cout << "407 ERR_TOOMANYTARGETS " << arg << " :Duplicate recipients. No message delivered" << std::endl;
}

void ReplyMessage::errNoOrigin() {
	std::cout << "409 ERR_NOORIGIN :No origin specified" << std::endl;
}

void ReplyMessage::errNoRecipient(const std::string& arg) {
	std::cout << "411 ERR_NORECIPIENT :No recipient given " << arg << std::endl;
}

void ReplyMessage::errNoTextToSend() {
	std::cout << "412 ERR_NORECIPIENT :No text to send" << std::endl;
}

void ReplyMessage::errNoTopLevel(const std::string& arg) {
	std::cout << "413 ERR_NOTOPLEVEL " << arg << " :No toplevel domain specified" << std::endl;
}

void ReplyMessage::errWildTopLevel(const std::string& arg) {
	std::cout << "414 ERR_WILDTOPLEVEL " << arg << " :Wildcard in toplevel domain" << std::endl;
}

void ReplyMessage::errUnknownCommand(const std::string& arg) {
	std::cout << "421 ERR_UNKNOWNCOMMAND " << arg << " :Unknown command" << std::endl;
}

void ReplyMessage::errNoMOTD() {
	std::cout << "422 ERR_NOMOTD :MOTD File is missing" << std::endl;
}

void ReplyMessage::errNoAdminInfo(const std::string& arg) {
	std::cout << "423 ERR_NOADMININFO " << arg << " :No administrative info available" << std::endl;
}

void ReplyMessage::errFileError(const std::string& arg, const std::string& arg2) {
	std::cout << "424 ERR_FILEERROR :File error doing" << arg << " on " << arg2 << std::endl;
}

void ReplyMessage::errNoNickNameGiven() {
	std::cout << "431 ERR_NONICKNAMEGIVEN :No nickname given" << std::endl;
}

void ReplyMessage::errErroneusNuckname(const std::string& arg) {
	std::cout << "432 ERR_ERRONEUSNICKNAME " << arg << " :Erroneus nickname" << std::endl;
}

void ReplyMessage::errNicknameInUse(const std::string& arg) {
	std::cout << "433 ERR_NICKNAMEINUSE " << arg << " :Nickname is already in use" << std::endl;
}

void ReplyMessage::errNickCollision(const std::string& arg) {
	std::cout << "436 ERR_NICKCOLLISION " << arg << " :Nickname collision KILL" << std::endl;
}

void ReplyMessage::errUserNotInChannel(const std::string& arg, const std::string& arg2) {
	std::cout << "441 ERR_USERNOTINCHANNEL " << arg << " " << arg2 << " :They aren't on that channel" << std::endl;
}

void ReplyMessage::errNotOnChannel(const std::string& arg) {
	std::cout << "442 ERR_NOTONCHANNEL " << arg << " :You're not on that channel" << std::endl;
}

void ReplyMessage::errUserOnChannel(const std::string& arg, const std::string& arg2) {
	std::cout << "443 ERR_USERONCHANNEL " << arg << " " << arg2 << " :is already on channel" << std::endl;
}

void ReplyMessage::errNoLogin(const std::string& arg) {
	std::cout << "444 ERR_NOLOGIN " << arg << " :User not logged in" << std::endl;
}

void ReplyMessage::errSummonDisabled() {
	std::cout << "445 ERR_SUMMONDISABLED :SUMMON has been disabled" << std::endl;
}

void ReplyMessage::errUserDisabled() {
	std::cout << "446 ERR_USERSDISABLED :USERS has been disabled" << std::endl;
}

void ReplyMessage::errNotRegistered() {
	std::cout << "451 ERR_NOTREGISTERED :You have not registered" << std::endl;
}

void ReplyMessage::errNeedMoreParams(const std::string& arg) {
	std::cout << "461 ERR_NEEDMOREPARAMS " << arg << " :Not enough parameters" << std::endl;
}

void ReplyMessage::errAlreadyRegistered() {
	std::cout << "462 ERR_ALREADYREGISTRED :You may not reregister" << std::endl;
}

void ReplyMessage::errNoPermForHost() {
	std::cout << "463 ERR_NOPERMFORHOST :Your host isn't among the privileged" << std::endl;
}

void ReplyMessage::errPasswdMismatch() {
	std::cout << "464 ERR_PASSWDMISMATCH :Password incorrect" << std::endl;
}

void ReplyMessage::errYoureBannedCreep() {
	std::cout << "465 ERR_YOUREBANNEDCREEP :You are banned from this server" << std::endl;
}

void ReplyMessage::errKeySet(const std::string& arg) {
	std::cout << "467 ERR_KEYSET " << arg << " :Channel key already set" << std::endl;
}

void ReplyMessage::errChannelIsFull(const std::string& arg) {
	std::cout << "471 ERR_CHANNELISFULL " << arg << " :Cannot join channel (+l)" << std::endl;
}

void ReplyMessage::errUnknownMode(const std::string& arg) {
	std::cout << "472 ERR_UNKNOWNMODE " << arg << " :is unknown mode char to me" << std::endl;
}

void ReplyMessage::errInviteOnlyChan(const std::string& arg) {
	std::cout << "473 ERR_INVITEONLYCHAN " << arg << " :Cannot join channel (+i)" << std::endl;
}

void ReplyMessage::errBannedFromChan(const std::string& arg) {
	std::cout << "474 ERR_BANNEDFROMCHAN " << arg << " :Cannot join channel (+b)" << std::endl;
}

void ReplyMessage::errBadChannelKey(const std::string& arg) {
	std::cout << "475 ERR_BADCHANNELKEY " << arg << " :Cannot join channel (+k)" << std::endl;
}

void ReplyMessage::errNoPrivileges() {
	std::cout << "481 ERR_NOPRIVILEGES :Permission Denied- You're not an IRC operator" << std::endl;
}

void ReplyMessage::errChaNoPrivsNeeded(const std::string& arg) {
	std::cout << "482 ERR_CHANOPRIVSNEEDED " << arg << " :You're not channel operator" << std::endl;
}

void ReplyMessage::errCantKillServer() {
	std::cout << "483 ERR_CANTKILLSERVER :You cant kill a server!" << std::endl;
}

void ReplyMessage::errNoOPerHost() {
	std::cout << "491 ERR_NOOPERHOST :No O-lines for your host" << std::endl;
}

void ReplyMessage::errUModeUnknownFlag() {
	std::cout << "501 ERR_UMODEUNKNOWNFLAG :Unknown MODE flag" << std::endl;
}

void ReplyMessage::errUserDontMatch() {
	std::cout << "502 ERR_USERSDONTMATCH :Cant change mode for other users" << std::endl;
}

//TODO RPLMSG
