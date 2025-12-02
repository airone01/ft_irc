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

void ReplyMessage::err_TooManyChannels(const std::string& arg) {
	std::cout << "405 ERR_TOOMANYCHANNELS " << arg << " :You have joined too many channels" << std::endl;
}

void ReplyMessage::err_WasNoSuckNick(const std::string& arg) {
	std::cout << "406 ERR_WASNOSUCHNICK " << arg << " :There was no such nickname" << std::endl;
}

void ReplyMessage::err_TooManyTargets(const std::string& arg) {
	std::cout << "407 ERR_TOOMANYTARGETS " << arg << " :Duplicate recipients. No message delivered" << std::endl;
}

void ReplyMessage::err_NoOrigin() {
	std::cout << "409 ERR_NOORIGIN :No origin specified" << std::endl;
}

void ReplyMessage::err_NoRecipient(const std::string& arg) {
	std::cout << "411 ERR_NORECIPIENT :No recipient given " << arg << std::endl;
}

void ReplyMessage::err_NoTextToSend() {
	std::cout << "412 ERR_NORECIPIENT :No text to send" << std::endl;
}

void ReplyMessage::err_NoTopLevel(const std::string& arg) {
	std::cout << "413 ERR_NOTOPLEVEL " << arg << " :No toplevel domain specified" << std::endl;
}

void ReplyMessage::err_WildTopLevel(const std::string& arg) {
	std::cout << "414 ERR_WILDTOPLEVEL " << arg << " :Wildcard in toplevel domain" << std::endl;
}

void ReplyMessage::err_UnknownCommand(const std::string& arg) {
	std::cout << "421 ERR_UNKNOWNCOMMAND " << arg << " :Unknown command" << std::endl;
}

void ReplyMessage::err_NoMOTD() {
	std::cout << "422 ERR_NOMOTD :MOTD File is missing" << std::endl;
}

void ReplyMessage::err_NoAdminInfo(const std::string& arg) {
	std::cout << "423 ERR_NOADMININFO " << arg << " :No administrative info available" << std::endl;
}

void ReplyMessage::err_FileError(const std::string& arg, const std::string& arg2) {
	std::cout << "424 ERR_FILEERROR :File error doing" << arg << " on " << arg2 << std::endl;
}

void ReplyMessage::err_NoNickNameGiven() {
	std::cout << "431 ERR_NONICKNAMEGIVEN :No nickname given" << std::endl;
}

void ReplyMessage::err_ErroneusNuckname(const std::string& arg) {
	std::cout << "432 ERR_ERRONEUSNICKNAME " << arg << " :Erroneus nickname" << std::endl;
}

void ReplyMessage::err_NicknameInUse(const std::string& arg) {
	std::cout << "433 ERR_NICKNAMEINUSE " << arg << " :Nickname is already in use" << std::endl;
}

void ReplyMessage::err_NickCollision(const std::string& arg) {
	std::cout << "436 ERR_NICKCOLLISION " << arg << " :Nickname collision KILL" << std::endl;
}

void ReplyMessage::err_UserNotInChannel(const std::string& arg, const std::string& arg2) {
	std::cout << "441 ERR_USERNOTINCHANNEL " << arg << " " << arg2 << " :They aren't on that channel" << std::endl;
}

void ReplyMessage::err_NotOnChannel(const std::string& arg) {
	std::cout << "442 ERR_NOTONCHANNEL " << arg << " :You're not on that channel" << std::endl;
}

void ReplyMessage::err_UserOnChannel(const std::string& arg, const std::string& arg2) {
	std::cout << "443 ERR_USERONCHANNEL " << arg << " " << arg2 << " :is already on channel" << std::endl;
}

void ReplyMessage::err_NoLogin(const std::string& arg) {
	std::cout << "444 ERR_NOLOGIN " << arg << " :User not logged in" << std::endl;
}

void ReplyMessage::err_SummonDisabled() {
	std::cout << "445 ERR_SUMMONDISABLED :SUMMON has been disabled" << std::endl;
}

void ReplyMessage::err_UserDisabled() {
	std::cout << "446 ERR_USERSDISABLED :USERS has been disabled" << std::endl;
}

void ReplyMessage::err_NotRegistered() {
	std::cout << "451 ERR_NOTREGISTERED :You have not registered" << std::endl;
}

void ReplyMessage::err_NeedMoreParams(const std::string& arg) {
	std::cout << "461 ERR_NEEDMOREPARAMS " << arg << " :Not enough parameters" << std::endl;
}

void ReplyMessage::err_AlreadyRegistered() {
	std::cout << "462 ERR_ALREADYREGISTRED :You may not reregister" << std::endl;
}

void ReplyMessage::err_NoPermForHost() {
	std::cout << "463 ERR_NOPERMFORHOST :Your host isn't among the privileged" << std::endl;
}

void ReplyMessage::err_PasswdMismatch() {
	std::cout << "464 ERR_PASSWDMISMATCH :Password incorrect" << std::endl;
}

void ReplyMessage::err_YoureBannedCreep() {
	std::cout << "465 ERR_YOUREBANNEDCREEP :You are banned from this server" << std::endl;
}

void ReplyMessage::err_KeySet(const std::string& arg) {
	std::cout << "467 ERR_KEYSET " << arg << " :Channel key already set" << std::endl;
}

void ReplyMessage::err_ChannelIsFull(const std::string& arg) {
	std::cout << "471 ERR_CHANNELISFULL " << arg << " :Cannot join channel (+l)" << std::endl;
}

void ReplyMessage::err_UnknownMode(const std::string& arg) {
	std::cout << "472 ERR_UNKNOWNMODE " << arg << " :is unknown mode char to me" << std::endl;
}

void ReplyMessage::err_InviteOnlyChan(const std::string& arg) {
	std::cout << "473 ERR_INVITEONLYCHAN " << arg << " :Cannot join channel (+i)" << std::endl;
}

void ReplyMessage::err_BannedFromChan(const std::string& arg) {
	std::cout << "474 ERR_BANNEDFROMCHAN " << arg << " :Cannot join channel (+b)" << std::endl;
}

void ReplyMessage::err_BadChannelKey(const std::string& arg) {
	std::cout << "475 ERR_BADCHANNELKEY " << arg << " :Cannot join channel (+k)" << std::endl;
}

void ReplyMessage::err_NoPrivileges() {
	std::cout << "481 ERR_NOPRIVILEGES :Permission Denied- You're not an IRC operator" << std::endl;
}

void ReplyMessage::err_ChaNoPrivsNeeded(const std::string& arg) {
	std::cout << "482 ERR_CHANOPRIVSNEEDED " << arg << " :You're not channel operator" << std::endl;
}

void ReplyMessage::err_CantKillServer() {
	std::cout << "483 ERR_CANTKILLSERVER :You cant kill a server!" << std::endl;
}

void ReplyMessage::err_NoOPerHost() {
	std::cout << "491 ERR_NOOPERHOST :No O-lines for your host" << std::endl;
}

void ReplyMessage::err_UModeUnknownFlag() {
	std::cout << "501 ERR_UMODEUNKNOWNFLAG :Unknown MODE flag" << std::endl;
}

void ReplyMessage::err_UserDontMatch() {
	std::cout << "502 ERR_USERSDONTMATCH :Cant change mode for other users" << std::endl;
}
