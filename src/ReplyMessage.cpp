#include "ReplyMessage.hpp"
#include "Logger.hpp"

#include <sstream>
#include <string>

std::string ReplyMessage::errNoSuchNick(const std::string &arg) {
  std::ostringstream oss;
  oss << "401 ERR_NOSUCHNICK " << arg << " :No such nick/channel";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoSuchServer(const std::string &arg) {
  std::ostringstream oss;
  oss << "402 ERR_NOSUCHSERVER " << arg << " :No such server";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoSuckChannel(const std::string &arg) {
  std::ostringstream oss;
  oss << "403 ERR_NOSUCHCHANNEL " << arg << " :No such channel";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errCannotSendToChannel(const std::string &arg) {
  std::ostringstream oss;
  oss << "404 ERR_CANNOTSENDTOCHAN " << arg << " :Cannot send to channel";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errTooManyChannels(const std::string &arg) {
  std::ostringstream oss;
  oss << "405 ERR_TOOMANYCHANNELS " << arg
      << " :You have joined too many channels";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errWasNoSuckNick(const std::string &arg) {
  std::ostringstream oss;
  oss << "406 ERR_WASNOSUCHNICK " << arg << " :There was no such nickname";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errTooManyTargets(const std::string &arg) {
  std::ostringstream oss;
  oss << "407 ERR_TOOMANYTARGETS " << arg
      << " :Duplicate recipients. No message delivered";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoOrigin() {
  std::ostringstream oss;
  oss << "409 ERR_NOORIGIN :No origin specified";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoRecipient(const std::string &arg) {
  std::ostringstream oss;
  oss << "411 ERR_NORECIPIENT :No recipient given " << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoTextToSend() {
  std::ostringstream oss;
  oss << "412 ERR_NORECIPIENT :No text to send";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoTopLevel(const std::string &arg) {
  std::ostringstream oss;
  oss << "413 ERR_NOTOPLEVEL " << arg << " :No toplevel domain specified";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errWildTopLevel(const std::string &arg) {
  std::ostringstream oss;
  oss << "414 ERR_WILDTOPLEVEL " << arg << " :Wildcard in toplevel domain";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUnknownCommand(const std::string &arg) {
  std::ostringstream oss;
  oss << "421 ERR_UNKNOWNCOMMAND " << arg << " :Unknown command";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoMOTD() {
  std::ostringstream oss;
  oss << "422 ERR_NOMOTD :MOTD File is missing";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoAdminInfo(const std::string &arg) {
  std::ostringstream oss;
  oss << "423 ERR_NOADMININFO " << arg << " :No administrative info available";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errFileError(const std::string &arg,
                                       const std::string &arg2) {
  std::ostringstream oss;
  oss << "424 ERR_FILEERROR :File error doing" << arg << " on " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoNickNameGiven() {
  std::ostringstream oss;
  oss << "431 ERR_NONICKNAMEGIVEN :No nickname given";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errErroneusNuckname(const std::string &arg) {
  std::ostringstream oss;
  oss << "432 ERR_ERRONEUSNICKNAME " << arg << " :Erroneus nickname";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNicknameInUse(const std::string &arg) {
  std::ostringstream oss;
  oss << "433 ERR_NICKNAMEINUSE " << arg << " :Nickname is already in use";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNickCollision(const std::string &arg) {
  std::ostringstream oss;
  oss << "436 ERR_NICKCOLLISION " << arg << " :Nickname collision KILL";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUserNotInChannel(const std::string &arg,
                                              const std::string &arg2) {
  std::ostringstream oss;
  oss << "441 ERR_USERNOTINCHANNEL " << arg << " " << arg2
      << " :They aren't on that channel";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNotOnChannel(const std::string &arg) {
  std::ostringstream oss;
  oss << "442 ERR_NOTONCHANNEL " << arg << " :You're not on that channel";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUserOnChannel(const std::string &arg,
                                           const std::string &arg2) {
  std::ostringstream oss;
  oss << "443 ERR_USERONCHANNEL " << arg << " " << arg2
      << " :is already on channel";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoLogin(const std::string &arg) {
  std::ostringstream oss;
  oss << "444 ERR_NOLOGIN " << arg << " :User not logged in";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errSummonDisabled() {
  std::ostringstream oss;
  oss << "445 ERR_SUMMONDISABLED :SUMMON has been disabled";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUserDisabled() {
  std::ostringstream oss;
  oss << "446 ERR_USERSDISABLED :USERS has been disabled";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNotRegistered() {
  std::ostringstream oss;
  oss << "451 ERR_NOTREGISTERED :You have not registered";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNeedMoreParams(const std::string &arg) {
  std::ostringstream oss;
  oss << "461 ERR_NEEDMOREPARAMS " << arg << " :Not enough parameters";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errAlreadyRegistered() {
  std::ostringstream oss;
  oss << "462 ERR_ALREADYREGISTRED :You may not reregister";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoPermForHost() {
  std::ostringstream oss;
  oss << "463 ERR_NOPERMFORHOST :Your host isn't among the privileged";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errPasswdMismatch() {
  std::ostringstream oss;
  oss << "464 ERR_PASSWDMISMATCH :Password incorrect";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errYoureBannedCreep() {
  std::ostringstream oss;
  oss << "465 ERR_YOUREBANNEDCREEP :You are banned from this server";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errKeySet(const std::string &arg) {
  std::ostringstream oss;
  oss << "467 ERR_KEYSET " << arg << " :Channel key already set";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errChannelIsFull(const std::string &arg) {
  std::ostringstream oss;
  oss << "471 ERR_CHANNELISFULL " << arg << " :Cannot join channel (+l)";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUnknownMode(const std::string &arg) {
  std::ostringstream oss;
  oss << "472 ERR_UNKNOWNMODE " << arg << " :is unknown mode char to me";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errInviteOnlyChan(const std::string &arg) {
  std::ostringstream oss;
  oss << "473 ERR_INVITEONLYCHAN " << arg << " :Cannot join channel (+i)";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errBannedFromChan(const std::string &arg) {
  std::ostringstream oss;
  oss << "474 ERR_BANNEDFROMCHAN " << arg << " :Cannot join channel (+b)";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errBadChannelKey(const std::string &arg) {
  std::ostringstream oss;
  oss << "475 ERR_BADCHANNELKEY " << arg << " :Cannot join channel (+k)";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoPrivileges() {
  std::ostringstream oss;
  oss << "481 ERR_NOPRIVILEGES :Permission Denied- You're not an IRC operator";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errChaNoPrivsNeeded(const std::string &arg) {
  std::ostringstream oss;
  oss << "482 ERR_CHANOPRIVSNEEDED " << arg << " :You're not channel operator";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errCantKillServer() {
  std::ostringstream oss;
  oss << "483 ERR_CANTKILLSERVER :You cant kill a server!";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errNoOPerHost() {
  std::ostringstream oss;
  oss << "491 ERR_NOOPERHOST :No O-lines for your host";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUModeUnknownFlag() {
  std::ostringstream oss;
  oss << "501 ERR_UMODEUNKNOWNFLAG :Unknown MODE flag";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::errUserDontMatch() {
  std::ostringstream oss;
  oss << "502 ERR_USERSDONTMATCH :Cant change mode for other users";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplNone(const std::string &arg) {
  std::ostringstream oss;
  oss << "300 RPL_NONE :Dummy reply number. Not used. " << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplUserHost(const std::string &arg) {
  std::ostringstream oss;
  oss << "302 RPL_USERHOST :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplIsOn(const std::string &arg) {
  std::ostringstream oss;
  oss << "303 RPL_ISON :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplAway(const std::string &arg,
                                  const std::string &arg2) {
  std::ostringstream oss;
  oss << "301 RPL_AWAY " << arg << " :" << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplUnAway() {
  std::ostringstream oss;
  oss << "305 RPL_UNAWAY :You are no longer marked as being away";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplNoAway() {
  std::ostringstream oss;
  oss << "306 RPL_NOWAY :You have been marked as being away";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplWhoIsUser(const std::string &arg,
                                       const std::string &arg2,
                                       const std::string &arg3,
                                       const std::string &arg4) {
  std::ostringstream oss;
  oss << "311 RPL_WHOISUSER " << arg << " " << arg2 << " " << arg3
      << " * :" << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplWhoIsServer(const std::string &arg,
                                         const std::string &arg2,
                                         const std::string &arg3) {
  std::ostringstream oss;
  oss << "312 RPL_WHOISSERVER " << arg << " " << arg2 << " :" << arg3;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplWhoIsOperator(const std::string &arg) {
  std::ostringstream oss;
  oss << "313 RPL_WHOISOPERATOR " << arg << " :is an IRC operator";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplWhoIsIdle(const std::string &arg,
                                       const std::string &arg2) {
  std::ostringstream oss;
  oss << "317 RPL_WHOISIDLE " << arg << " " << arg2 << " :seconds idle";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfWhoIs(const std::string &arg) {
  std::ostringstream oss;
  oss << "318 RPL_ENDOFWHOIS " << arg << " :End of /WHOIS list";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplWhoIsChannels(const std::string &arg,
                                           const std::string &arg2) {
  std::ostringstream oss;
  oss << "319 RPL_WHOISCHANNELS " << arg << " :" << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplWhoAsUser(const std::string &arg,
                                       const std::string &arg2,
                                       const std::string &arg3,
                                       const std::string &arg4) {
  std::ostringstream oss;
  oss << "314 RPL_WHOASUSER " << arg << " " << arg2 << " " << arg3
      << " * :" << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfWhoWas(const std::string &arg) {
  std::ostringstream oss;
  oss << "369 RPL_ENDOFWHOWAS " << arg << " :End of WHOWAS";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplListStart() {
  std::ostringstream oss;
  oss << "321 RPL_LISTSTART Channel :Users  Name";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplList(const std::string &arg,
                                  const std::string &arg2,
                                  const std::string &arg3) {
  std::ostringstream oss;
  oss << "322 RPL_LIST " << arg << " " << arg2 << " :" << arg3;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplListEnd(const std::string &arg) {
  std::ostringstream oss;
  oss << "323 RPL_LISTEND " << arg << " :End of /LIST";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplChannelModeIs(const std::string &arg,
                                           const std::string &arg2,
                                           const std::string &arg3) {
  std::ostringstream oss;
  oss << "324 RPL_CHANNELMODEIS " << arg << " " << arg2 << " " << arg3;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplNoTopic(const std::string &arg) {
  std::ostringstream oss;
  oss << "331 RPL_NOTOPIC " << arg << " :No topic is set";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTopic(const std::string &arg,
                                   const std::string &arg2) {
  std::ostringstream oss;
  oss << "332 RPL_TOPIC " << arg << " :" << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplInviting(const std::string &arg,
                                      const std::string &arg2) {
  std::ostringstream oss;
  oss << "341 RPL_INVITING " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplSummoning(const std::string &arg) {
  std::ostringstream oss;
  oss << "342 RPL_SUMMONING " << arg << " :Summoning user to IRC";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplVersion(const std::string &version,
                                     const std::string &server_name,
                                     const std::string &comment) {
  std::ostringstream oss;
  oss << "351 RPL_VERSION " << version << " " << server_name << " :" << comment
      << "\r\n";
  std::string v = oss.str();
  return v;
}

std::string
ReplyMessage::rplWhoReply(const std::string &arg, const std::string &arg2,
                          const std::string &arg3, const std::string &arg4,
                          const std::string &arg5, const std::string &arg6,
                          const std::string &arg7, const std::string &arg8) {
  std::ostringstream oss;
  oss << "352 RPL_WHOREPLY " << arg << " " << arg2 << " " << arg3 << " " << arg4
      << " " << arg5 << " " << arg6 << " :" << arg7 << " " << arg8;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfWho(const std::string &arg) {
  std::ostringstream oss;
  oss << "315 RPL_ENDOFWHO " << arg << " :End of /WHO list";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplNamReply(const std::string &arg,
                                      const std::string &arg2) {
  std::ostringstream oss;
  oss << "353 RPL_NAMREPLY " << arg << " :" << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfNames(const std::string &arg) {
  std::ostringstream oss;
  oss << "366 RPL_ENDOFNAMES " << arg << " :End of /NAMES list";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplLinks(const std::string &arg,
                                   const std::string &arg2,
                                   const std::string &arg3,
                                   const std::string &arg4) {
  std::ostringstream oss;
  oss << "364 RPL_LINKS " << arg << " " << arg2 << " :" << arg3 << " " << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfLinks(const std::string &arg) {
  std::ostringstream oss;
  oss << "365 RPL_ENDOFLINKS " << arg << " :End of /LINKS list";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplBanList(const std::string &arg,
                                     const std::string &arg2) {
  std::ostringstream oss;
  oss << "367 RPL_BANLIST " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfBanList(const std::string &arg) {
  std::ostringstream oss;
  oss << "368 RPL_ENDOFBANLIST " << arg << " :End of channel ban list";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplInfo(const std::string &arg) {
  std::ostringstream oss;
  oss << "371 RPL_INFO :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfInfo(const std::string &arg) {
  std::ostringstream oss;
  oss << "374 RPL_ENDOFINFO " << arg << " :End of /INFO list";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplMOTDStart(const std::string &arg) {
  std::ostringstream oss;
  oss << "375 RPL_MOTDSTART :- " << arg << " Message of the day - ";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplMOTD(const std::string &arg) {
  std::ostringstream oss;
  oss << "372 RPL_MOTD :- " << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfMOTD(const std::string &arg) {
  std::ostringstream oss;
  oss << "376 RPL_ENDOFMOTD " << arg << " :End of /MOTD command";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplYoureOper(const std::string &arg) {
  std::ostringstream oss;
  oss << "381 RPL_YOUREOPER " << arg << " :You are now an IRC operator";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplRehashing(const std::string &arg) {
  std::ostringstream oss;
  oss << "382 RPL_REHASHING " << arg << " :Rehashing";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTime(const std::string &arg,
                                  const std::string &arg2) {
  std::ostringstream oss;
  oss << "391 RPL_TIME " << arg << " :" << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplUpersStart(const std::string &arg) {
  std::ostringstream oss;
  oss << "392 RPL_USERSSTART " << arg << " :UserID   Terminal  Host";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplUsers(const std::string &arg) {
  std::ostringstream oss;
  oss << "393 RPL_USERS :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfUsers(const std::string &arg) {
  std::ostringstream oss;
  oss << "394 RPL_ENDOFUSERS " << arg << " :End of users";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplNoUsers(const std::string &arg) {
  std::ostringstream oss;
  oss << "395 RPL_NOUSERS " << arg << " :Nobody logged in";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceLink(const std::string &arg,
                                       const std::string &arg2,
                                       const std::string &arg3) {
  std::ostringstream oss;
  oss << "200 RPL_TRACELINK Link " << arg << " " << arg2 << " " << arg3;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceConnecting(const std::string &arg,
                                             const std::string &arg2) {
  std::ostringstream oss;
  oss << "201 RPL_TRACECONNECTING Try. " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceHandShake(const std::string &arg,
                                            const std::string &arg2) {
  std::ostringstream oss;
  oss << "202 RPL_TRACEHANDSHAKE H.S. " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceUnknown(const std::string &arg,
                                          const std::string &arg2) {
  std::ostringstream oss;
  oss << "203 RPL_TRACEUNKNOWN ???? " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceOperator(const std::string &arg,
                                           const std::string &arg2) {
  std::ostringstream oss;
  oss << "204 RPL_TRACEOPERATOR Oper " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceUser(const std::string &arg,
                                       const std::string &arg2) {
  std::ostringstream oss;
  oss << "205 RPL_TRACEUSER User " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceServer(const std::string &arg,
                                         const std::string &arg2,
                                         const std::string &arg3,
                                         const std::string &arg4,
                                         const std::string &arg5) {
  std::ostringstream oss;
  oss << "206 RPL_TRACESERVER Serv " << arg << " " << arg2 << "S " << arg3
      << "C " << arg4 << " " << arg5;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceNewType(const std::string &arg,
                                          const std::string &arg2) {
  std::ostringstream oss;
  oss << "208 RPL_TRACENEWTYPE " << arg << " 0 " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplTraceLog(const std::string &arg,
                                      const std::string &arg2) {
  std::ostringstream oss;
  oss << "261 RPL_TRACELOG File " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string
ReplyMessage::rplStatsLinkInfo(const std::string &arg, const std::string &arg2,
                               const std::string &arg3, const std::string &arg4,
                               const std::string &arg5, const std::string &arg6,
                               const std::string &arg7) {
  std::ostringstream oss;
  oss << "211 RPL_STATSLINKINFO " << arg << " " << arg2 << " " << arg3 << " "
      << arg4 << " " << arg5 << " " << arg6 << " " << arg7;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsCommands(const std::string &arg,
                                           const std::string &arg2) {
  std::ostringstream oss;
  oss << "212 RPL_STATSCOMMANDS " << arg << " " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsCLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::ostringstream oss;
  oss << "213 RPL_STATSCLINE C " << arg << " * " << arg2 << " " << arg3 << " "
      << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsNLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::ostringstream oss;
  oss << "214 RPL_STATSNLINE N " << arg << " * " << arg2 << " " << arg3 << " "
      << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsILine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::ostringstream oss;
  oss << "215 RPL_STATSILINE I " << arg << " * " << arg2 << " " << arg3 << " "
      << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsKLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::ostringstream oss;
  oss << "216 RPL_STATSKLINE K " << arg << " * " << arg2 << " " << arg3 << " "
      << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsYLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::ostringstream oss;
  oss << "218 RPL_STATSYLINE Y " << arg << " " << arg2 << " " << arg3 << " "
      << arg4;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplEndOfStats(const std::string &arg) {
  std::ostringstream oss;
  oss << "219 RPL_ENDOFSTATS " << arg << " :End of /STATS report";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsLLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3) {
  std::ostringstream oss;
  oss << "241 RPL_STATSLLINE L " << arg << " * " << arg2 << " " << arg3;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsUpTime(const std::string &arg) {
  std::ostringstream oss;
  oss << "242 RPL_STATSUPTIME :Server Up " << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsOLine(const std::string &arg,
                                        const std::string &arg2) {
  std::ostringstream oss;
  oss << "243 RPL_STATSOLINE O " << arg << " * " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplStatsHLine(const std::string &arg,
                                        const std::string &arg2) {
  std::ostringstream oss;
  oss << "244 RPL_STATSHLINE H " << arg << " * " << arg2;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplUModeIs(const std::string &arg) {
  std::ostringstream oss;
  oss << "221 RPL_UMODEIS " << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplLUserClient(const std::string &arg,
                                         const std::string &arg2,
                                         const std::string &arg3) {
  std::ostringstream oss;
  oss << "251 RPL_LUSERCLIENT :There are " << arg << " users and " << arg2
      << " invisible on " << arg3 << " servers";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplLUserOp(const std::string &arg) {
  std::ostringstream oss;
  oss << "252 RPL_LUSEROP " << arg << " :operator(s) online";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplLUserUnknown(const std::string &arg) {
  std::ostringstream oss;
  oss << "253 RPL_LUSERUNKNOWN " << arg << " :unknown connection(s)";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplLUserChannels(const std::string &arg) {
  std::ostringstream oss;
  oss << "254 RPL_LUSERCHANNELS " << arg << " :channels formed";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplLUserMe(const std::string &arg,
                                     const std::string &arg2) {
  std::ostringstream oss;
  oss << "255 RPL_LUSERME :I have " << arg << " clients and " << arg2
      << " servers";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplAdminMe(const std::string &arg) {
  std::ostringstream oss;
  oss << "256 RPL_ADMINME " << arg << " :Administrative info";
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplAdminLoc1(const std::string &arg) {
  std::ostringstream oss;
  oss << "257 RPL_ADMINLOC1 :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplAdminLoc2(const std::string &arg) {
  std::ostringstream oss;
  oss << "258 RPL_ADMINLOC2 :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}

std::string ReplyMessage::rplAdminMail(const std::string &arg) {
  std::ostringstream oss;
  oss << "259 RPL_ADMINEMAIL :" << arg;
  std::string v = oss.str();
  logger::error() << v;
  return v;
}
