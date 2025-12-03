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

void ReplyMessage::rplUserHost(const std::string& arg){
	std::cout << "302 RPL_USERHOST :" << arg << std::endl;
}

void ReplyMessage::rplIsOn(const std::string& arg){
	std::cout << "303 RPL_ISON :" << arg << std::endl;
}

void ReplyMessage::rplAway(const std::string& arg, const std::string& arg2){
	std::cout << "301 RPL_AWAY " << arg << " :" << arg2 << std::endl;
}

void ReplyMessage::rplUnAway(){
	std::cout << "305 RPL_UNAWAY :You are no longer marked as being away" << std::endl;
}

void ReplyMessage::rplNoAway(){
	std::cout << "306 RPL_NOWAY :You have been marked as being away" << std::endl;
}

void ReplyMessage::rplWhoIsUser(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "311 RPL_WHOISUSER " << arg << " " << arg2 << " " << arg3 << " * :" << arg4 << std::endl;
}

void ReplyMessage::rplWhoIsServer(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "312 RPL_WHOISSERVER " << arg << " " << arg2 << " :" << arg3 << std::endl;
}

void ReplyMessage::rplWhoIsOperator(const std::string& arg){
	std::cout << "313 RPL_WHOISOPERATOR " << arg << " :is an IRC operator" << std::endl;
}

void ReplyMessage::rplWhoIsIdle(const std::string& arg, const std::string& arg2){
	std::cout << "317 RPL_WHOISIDLE " << arg << " " << arg2 << " :seconds idle" << std::endl;
}

void ReplyMessage::rplEndOfWhoIs(const std::string& arg){
	std::cout << "318 RPL_ENDOFWHOIS " << arg << " :End of /WHOIS list" << std::endl;
}

void ReplyMessage::rplWhoIsChannels(const std::string& arg, const std::string& arg2){
	std::cout << "319 RPL_WHOISCHANNELS " << arg << " :" << arg2 << std::endl;
}

void ReplyMessage::rplWhoAsUser(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "314 RPL_WHOASUSER " << arg << " " << arg2 << " " << arg3 << " * :" << arg4 << std::endl;
}

void ReplyMessage::rplEndOfWhoWas(const std::string& arg){
	std::cout << "369 RPL_ENDOFWHOWAS " << arg << " :End of WHOWAS"<< std::endl;
}

void ReplyMessage::rplListStart(){
	std::cout << "321 RPL_LISTSTART Channel :Users  Name" << std::endl;
}

void ReplyMessage::rplList(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "322 RPL_LIST " << arg << " " << arg2 << " :" << arg3 << std::endl;
}

void ReplyMessage::rplListEnd(const std::string& arg){
	std::cout << "323 RPL_LISTEND " << arg << " :End of /LIST" << std::endl;
}

void ReplyMessage::rplChannelModeIs(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "324 RPL_CHANNELMODEIS " << arg << " " << arg2 << " " << arg3 << std::endl;
}

void ReplyMessage::rplNoTopic(const std::string& arg){
	std::cout << "331 RPL_NOTOPIC " << arg << " :No topic is set" << std::endl;
}

void ReplyMessage::rplTopic(const std::string& arg, const std::string& arg2){
	std::cout << "332 RPL_TOPIC " << arg << " :" << arg2 << std::endl;
}

void ReplyMessage::rplInviting(const std::string& arg, const std::string& arg2){
	std::cout << "341 RPL_INVITING " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplSummoning(const std::string& arg){
	std::cout << "342 RPL_SUMMONING " << arg << " :Summoning user to IRC" << std::endl;
}

void ReplyMessage::rplVersion(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "351 RPL_VERSION " << arg << " " << arg2 << " :" << arg3 << std::endl;
}

void ReplyMessage::rplWhoReply(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5, const std::string& arg6, const std::string& arg7, const std::string& arg8){
	std::cout << "352 RPL_WHOREPLY " << arg << " " << arg2 << " " << arg3 << " " << arg4 << " " << arg5 << " " << arg6 << " :" << arg7 << " " << arg8 << std::endl;
}

void ReplyMessage::rplEndOfWho(const std::string& arg){
	std::cout << "315 RPL_ENDOFWHO " << arg << " :End of /WHO list" << std::endl;
}

void ReplyMessage::rplNamReply(const std::string& arg, const std::string& arg2){
	std::cout << "353 RPL_NAMREPLY " << arg << " :" << arg2 << std::endl;
}

void ReplyMessage::rplEndOfNames(const std::string& arg){
	std::cout << "366 RPL_ENDOFNAMES " << arg << " :End of /NAMES list" << std::endl;
}

void ReplyMessage::rplLinks(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "364 RPL_LINKS " << arg << " " << arg2 << " :" << arg3 << " " << arg4 << std::endl;
}

void ReplyMessage::rplEndOfLinks(const std::string& arg){
	std::cout << "365 RPL_ENDOFLINKS " << arg << " :End of /LINKS list" << std::endl;
}

void ReplyMessage::rplBanList(const std::string& arg, const std::string& arg2){
	std::cout << "367 RPL_BANLIST " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplEndOfBanList(const std::string& arg){
	std::cout << "368 RPL_ENDOFBANLIST " << arg << " :End of channel ban list" << std::endl;
}

void ReplyMessage::rplInfo(const std::string& arg){
	std::cout << "371 RPL_INFO :" << arg << std::endl;
}

void ReplyMessage::rplEndOfInfo(const std::string& arg){
	std::cout << "374 RPL_ENDOFINFO " << arg << " :End of /INFO list" << std::endl;
}

void ReplyMessage::rplMOTDStart(const std::string& arg){
	std::cout << "375 RPL_MOTDSTART :- " << arg << " Message of the day - " << std::endl;
}

void ReplyMessage::rplMOTD(const std::string& arg){
	std::cout << "372 RPL_MOTD :- " << arg << std::endl;
}

void ReplyMessage::rplEndOfMOTD(const std::string& arg){
	std::cout << "376 RPL_ENDOFMOTD " << arg << " :End of /MOTD command" << std::endl;
}

void ReplyMessage::rplYoureOper(const std::string& arg){
	std::cout << "381 RPL_YOUREOPER " << arg << " :You are now an IRC operator" << std::endl;
}

void ReplyMessage::rplRehashing(const std::string& arg){
	std::cout << "382 RPL_REHASHING " << arg << " :Rehashing" << std::endl;
}

void ReplyMessage::rplTime(const std::string& arg, const std::string& arg2){
	std::cout << "391 RPL_TIME " << arg << " :" << arg2 << std::endl;
}

void ReplyMessage::rplUpersStart(const std::string& arg){
	std::cout << "392 RPL_USERSSTART " << arg << " :UserID   Terminal  Host" << std::endl;
}

void ReplyMessage::rplUsers(const std::string& arg){
	std::cout << "393 RPL_USERS :" << arg << std::endl;
}

void ReplyMessage::rplEndOfUsers(const std::string& arg){
	std::cout << "394 RPL_ENDOFUSERS " << arg << " :End of users" << std::endl;
}

void ReplyMessage::rplNoUsers(const std::string& arg){
	std::cout << "395 RPL_NOUSERS " << arg << " :Nobody logged in" << std::endl;
}

void ReplyMessage::rplTraceLink(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "200 RPL_TRACELINK Link " << arg << " " << arg2 << " " << arg3 << std::endl;
}

void ReplyMessage::rplTraceConnecting(const std::string& arg, const std::string& arg2){
	std::cout << "201 RPL_TRACECONNECTING Try. " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplTraceHandShake(const std::string& arg, const std::string& arg2){
	std::cout << "202 RPL_TRACEHANDSHAKE H.S. " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplTraceUnknown(const std::string& arg, const std::string& arg2){
	std::cout << "203 RPL_TRACEUNKNOWN ???? " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplTraceOperator(const std::string& arg, const std::string& arg2){
	std::cout << "204 RPL_TRACEOPERATOR Oper " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplTraceUser(const std::string& arg, const std::string& arg2){
	std::cout << "205 RPL_TRACEUSER User " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplTraceServer(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5){
	std::cout << "206 RPL_TRACESERVER Serv " << arg << " " << arg2 << "S " << arg3 << "C " << arg4 << " " << arg5 << std::endl;
}

void ReplyMessage::rplTraceNewType(const std::string& arg, const std::string& arg2){
	std::cout << "208 RPL_TRACENEWTYPE " << arg << " 0 " << arg2 << std::endl;
}

void ReplyMessage::rplTraceLog(const std::string& arg, const std::string& arg2){
	std::cout << "261 RPL_TRACELOG File " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplStatsLinkInfo(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5, const std::string& arg6, const std::string& arg7){
	std::cout << "211 RPL_STATSLINKINFO " << arg << " " << arg2 << " " << arg3 << " " << arg4 << " " << arg5 << " " << arg6 << " " << arg7 << std::endl;
}

void ReplyMessage::rplStatsCommands(const std::string& arg, const std::string& arg2){
	std::cout << "212 RPL_STATSCOMMANDS " << arg << " " << arg2 << std::endl;
}

void ReplyMessage::rplStatsCLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "213 RPL_STATSCLINE C " << arg << " * " << arg2 << " " << arg3 << " " << arg4 << std::endl;
}

void ReplyMessage::rplStatsNLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "214 RPL_STATSNLINE N " << arg << " * " << arg2 << " " << arg3 << " " << arg4 << std::endl;
}

void ReplyMessage::rplStatsILine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "215 RPL_STATSILINE I " << arg << " * " << arg2 << " " << arg3 << " " << arg4 << std::endl;
}

void ReplyMessage::rplStatsKLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "216 RPL_STATSKLINE K " << arg << " * " << arg2 << " " << arg3 << " " << arg4 << std::endl;
}

void ReplyMessage::rplStatsYLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4){
	std::cout << "218 RPL_STATSYLINE Y " << arg << " " << arg2 << " " << arg3 << " " << arg4 << std::endl;
}

void ReplyMessage::rplEndOfStats(const std::string& arg){
	std::cout << "219 RPL_ENDOFSTATS " << arg << " :End of /STATS report" << std::endl;
}

void ReplyMessage::rplStatsLLine(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "241 RPL_STATSLLINE L " << arg << " * " << arg2 << " " << arg3 << std::endl;
}

void ReplyMessage::rplStatsUpTime(const std::string& arg){
	std::cout << "242 RPL_STATSUPTIME :Server Up " << arg << std::endl;
}

void ReplyMessage::rplStatsOLine(const std::string& arg, const std::string& arg2){
	std::cout << "243 RPL_STATSOLINE O " << arg << " * " << arg2 << std::endl;
}

void ReplyMessage::rplStatsHLine(const std::string& arg, const std::string& arg2){
	std::cout << "244 RPL_STATSHLINE H " << arg << " * " << arg2 << std::endl;
}

void ReplyMessage::rplUModeIs(const std::string& arg){
	std::cout << "221 RPL_UMODEIS " << arg << std::endl;
}

void ReplyMessage::rplLUserClient(const std::string& arg, const std::string& arg2, const std::string& arg3){
	std::cout << "251 RPL_LUSERCLIENT :There are " << arg << " users and " << arg2 << " invisible on " << arg3 << " servers" << std::endl;
}

void ReplyMessage::rplLUserOp(const std::string& arg){
	std::cout << "252 RPL_LUSEROP " << arg << " :operator(s) online" << std::endl;
}

void ReplyMessage::rplLUserUnknown(const std::string& arg){
	std::cout << "253 RPL_LUSERUNKNOWN " << arg << " :unknown connection(s)" << std::endl;
}

void ReplyMessage::rplLUserChannels(const std::string& arg){
	std::cout << "254 RPL_LUSERCHANNELS " << arg << " :channels formed" << std::endl;
}

void ReplyMessage::rplLUserMe(const std::string& arg, const std::string& arg2){
	std::cout << "255 RPL_LUSERME :I have " << arg << " clients and " << arg2 << " servers" << std::endl;
}

void ReplyMessage::rplAdminMe(const std::string& arg){
	std::cout << "256 RPL_ADMINME " << arg << " :Administrative info" << std::endl;
}

void ReplyMessage::rplAdminLoc1(const std::string& arg){
	std::cout << "257 RPL_ADMINLOC1 :" << arg << std::endl;
}

void ReplyMessage::rplAdminLoc2(const std::string& arg){
	std::cout << "258 RPL_ADMINLOC2 :" << arg << std::endl;
}

void ReplyMessage::rplAdminMail(const std::string& arg){
	std::cout << "259 RPL_ADMINEMAIL :" << arg << std::endl;
}
