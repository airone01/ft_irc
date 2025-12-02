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

void ReplyMessage::rplNone(const std::string& arg){
	std::cout << "300 RPL_NONE :Dummy reply number. Not used. " << arg << std::endl;
}

//TODO RPLMSG  // ":[<reply>{<space><reply>}]"
void ReplyMessage::rplUserHost(const std::string& arg){
	std::cout << "302 RPL_USERHOST :" << arg << std::endl;
}

//TODO RPLMSG // ":[<nick> {<space><nick>}]"
void ReplyMessage::rplIsOn(const std::string& arg){
	std::cout << "303 RPL_ISON :" << arg << std::endl;
}

//TODO RPLMSG // "<nick> :<away message>"
void ReplyMessage::rplAway(const std::string& arg){
	std::cout << "301 RPL_AWAY " << arg << std::endl;
}

void ReplyMessage::rplUnAway(){
	std::cout << "305 RPL_UNAWAY :You are no longer marked as being away" << std::endl;
}

void ReplyMessage::rplNoAway(const std::string& arg){
	std::cout << "306 RPL_NOWAY :You have been marked as being away" << std::endl;
}
//TODO RPLMSG // "<nick> <user> <host> * :<real name>"
void ReplyMessage::rplWhoIsUser(const std::string& arg){
	std::cout << "311 RPL_WHOISUSER " << arg << std::endl;
}
//TODO RPLMSG // "<nick> <server> :<server info>"
void ReplyMessage::rplWhoIsServer(const std::string& arg){
	std::cout << "312 RPL_WHOISSERVER " << arg << std::endl;
}

void ReplyMessage::rplWhoIsOperator(const std::string& arg){
	std::cout << "313 RPL_WHOISOPERATOR " << arg << " :is an IRC operator" << std::endl;
}
//TODO RPLMSG // "<nick> <integer> :seconds idle"
void ReplyMessage::rplWhoIsIdle(const std::string& arg){
	std::cout << "317 RPL_WHOISIDLE " << arg << std::endl;
}

void ReplyMessage::rplEndOfWhoIs(const std::string& arg){
	std::cout << "318 RPL_ENDOFWHOIS " << arg << " :End of /WHOIS list" << std::endl;
}
//TODO RPLMSG // "<nick> :{[@|+]<channel><space>}"
void ReplyMessage::rplWhoIsChannels(const std::string& arg){
	std::cout << "319 RPL_WHOISCHANNELS " << arg << std::endl;
}
//TODO RPLMSG // "<nick> <user> <host> * :<real name>"
void ReplyMessage::rplWhoAsUser(const std::string& arg){
	std::cout << "314 RPL_WHOASUSER " << arg << std::endl;
}

void ReplyMessage::rplEndOfWhoWas(const std::string& arg){
	std::cout << "369 RPL_ENDOFWHOWAS " << arg << " :End of WHOWAS"<< std::endl;
}

void ReplyMessage::rplListStart(){
	std::cout << "321 RPL_LISTSTART Channel :Users  Name" << std::endl;
}

void ReplyMessage::rplList(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> <# visible> :<topic>" 322
void ReplyMessage::rplListEnd(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":End of /LIST" 323
void ReplyMessage::rplChannelModeIs(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> <mode> <mode params>" 324
void ReplyMessage::rplNoTopic(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> :No topic is set" 331
void ReplyMessage::rplTopic(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> :<topic>" 332
void ReplyMessage::rplInviting(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> <nick>" 341
void ReplyMessage::rplSummoning(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<user> :Summoning user to IRC" 342
void ReplyMessage::rplVersion(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<version>.<debuglevel> <server> :<comments>" 351
void ReplyMessage::rplWhoReply(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>" 352
void ReplyMessage::rplEndOfWho(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<name> :End of /WHO list" 315
void ReplyMessage::rplNamReply(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]" 353
void ReplyMessage::rplEndOfNames(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> :End of /NAMES list" 366
void ReplyMessage::rplLinks(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<mask> <server> :<hopcount> <server info>" 364
void ReplyMessage::rplEndOfLinks(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<mask> :End of /LINKS list" 365
void ReplyMessage::rplBanList(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> <banid>" 367
void ReplyMessage::rplEndOfBanList(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<channel> :End of channel ban list" 368
void ReplyMessage::rplInfo(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":<string>" 371
void ReplyMessage::rplEndOfInfo(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":End of /INFO list" 374
void ReplyMessage::rplMOTDStart(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":- <server> Message of the day - " 375
void ReplyMessage::rplMOTD(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":- <text>" 372
void ReplyMessage::rplEndOfMOTD(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":End of /MOTD command" 376
void ReplyMessage::rplYoureOper(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":You are now an IRC operator" 381
void ReplyMessage::rplRehashing(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<config file> :Rehashing" 382
void ReplyMessage::rplTime(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<server> :<string showing server's local time>" 391
void ReplyMessage::rplUpersStart(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":UserID   Terminal  Host" 392
void ReplyMessage::rplUsers(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":%-8s %-9s %-8s" 393
void ReplyMessage::rplEndOfUsers(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":End of users" 394
void ReplyMessage::rplNoUsers(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":Nobody logged in" 395
void ReplyMessage::rplTraceLink(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "Link <version & debug level> <destination> <next server>" 200
void ReplyMessage::rplTraceConnecting(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "Try. <class> <server>" 201
void ReplyMessage::rplTraceHandShake(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "H.S. <class> <server>" 202
void ReplyMessage::rplTraceUnknown(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "???? <class> [<client IP address in dot form>]" 203
void ReplyMessage::rplTraceOperator(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "Oper <class> <nick>" 204
void ReplyMessage::rplTraceUser(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "User <class> <nick>" 205
void ReplyMessage::rplTraceServer(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server>" 206
void ReplyMessage::rplTraceNewType(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<newtype> 0 <client name>" 208
void ReplyMessage::rplTraceLog(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "File <logfile> <debug level>" 261
void ReplyMessage::rplStatsLinkInfo(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<linkname> <sendq> <sent messages> <sent bytes> <received messages> <received bytes> <time open>" 211
void ReplyMessage::rplStatsCommands(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<command> <count>" 212
void ReplyMessage::rplStatsCLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "C <host> * <name> <port> <class>" 213
void ReplyMessage::rplStatsNLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "N <host> * <name> <port> <class>" 214
void ReplyMessage::rplStatsILine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "I <host> * <host> <port> <class>" 215
void ReplyMessage::rplStatsKLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "K <host> * <username> <port> <class>" 216
void ReplyMessage::rplStatsYLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "Y <class> <ping frequency> <connect frequency> <max sendq>" 218
void ReplyMessage::rplEndOfStats(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<stats letter> :End of /STATS report" 219
void ReplyMessage::rplStatsLLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "L <hostmask> * <servername> <maxdepth>" 241
void ReplyMessage::rplStatsUpTime(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":Server Up %d days %d:%02d:%02d" 242
void ReplyMessage::rplStatsOLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "O <hostmask> * <name>" 243
void ReplyMessage::rplStatsHLine(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "H <hostmask> * <servername>" 244
void ReplyMessage::rplUModeIs(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<user mode string>" 221
void ReplyMessage::rplLUserClient(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":There are <integer> users and <integer> invisible on <integer> servers" 251
void ReplyMessage::rplLUserOp(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<integer> :operator(s) online" 252
void ReplyMessage::rplLUserUnknown(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<integer> :unknown connection(s)" 253
void ReplyMessage::rplLUserChannels(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<integer> :channels formed" 254
void ReplyMessage::rplLUserMe(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":I have <integer> clients and <integer> servers" 255
void ReplyMessage::rplAdminMe(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // "<server> :Administrative info" 256
void ReplyMessage::rplAdminLoc1(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":<admin info>" 257
void ReplyMessage::rplAdminLoc2(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":<admin info>" 258
void ReplyMessage::rplAdminMail(const std::string& arg){
	std::cout << "code  " << arg << std::endl;
}
 // ":<admin info>" 259
