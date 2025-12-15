#include "ReplyMessage.hpp"

#include <sstream>
#include <string>

std::string ReplyMessage::errNoSuchNick(const std::string &arg) {
  std::string tmp = "401 ERR_NOSUCHNICK " + arg + " :No such nick/channel\r\n";
  return tmp;
}

std::string ReplyMessage::errNoSuchServer(const std::string &arg) {
  std::string tmp = "402 ERR_NOSUCHSERVER " + arg + " :No such server\r\n";
  return tmp;
}

std::string ReplyMessage::errNoSuckChannel(const std::string &arg) {
  std::string tmp = "403 ERR_NOSUCHCHANNEL " + arg + " :No such channel\r\n";
  return tmp;
}

std::string ReplyMessage::errCannotSendToChannel(const std::string &arg) {
  std::string tmp = "404 ERR_CANNOTSENDTOCHAN " + arg + " :Cannot send to channel\r\n";
  return tmp;
}

std::string ReplyMessage::errTooManyChannels(const std::string &arg) {
  std::string tmp = "405 ERR_TOOMANYCHANNELS " + arg + " :You have joined too many channels\r\n";  
  return tmp;
}

std::string ReplyMessage::errWasNoSuckNick(const std::string &arg) {
  std::string tmp = "406 ERR_WASNOSUCHNICK " + arg + " :There was no such nickname\r\n";
  return tmp;
}

std::string ReplyMessage::errTooManyTargets(const std::string &arg) {
  std::string tmp = "407 ERR_TOOMANYTARGETS " + arg
      + " :Duplicate recipients. No message delivered\r\n";
  return tmp;
}

std::string ReplyMessage::errNoOrigin() {
  std::string tmp = "409 ERR_NOORIGIN :No origin specified\r\n";
  return tmp;
}

std::string ReplyMessage::errNoRecipient(const std::string &arg) {
  std::string tmp = "411 ERR_NORECIPIENT :No recipient given " + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::errNoTextToSend() {
  std::string tmp = "412 ERR_NORECIPIENT :No text to send\r\n";
  return tmp;
}

std::string ReplyMessage::errNoTopLevel(const std::string &arg) {
  std::string tmp = "413 ERR_NOTOPLEVEL " + arg + " :No toplevel domain specified\r\n";
  return tmp;
}

std::string ReplyMessage::errWildTopLevel(const std::string &arg) {
  std::string tmp = "414 ERR_WILDTOPLEVEL " + arg + " :Wildcard in toplevel domain\r\n";
  return tmp;
}

std::string ReplyMessage::errUnknownCommand(const std::string &arg) {
  std::string tmp = "421 ERR_UNKNOWNCOMMAND " + arg + " :Unknown command\r\n";
  return tmp;
}

std::string ReplyMessage::errNoMOTD() {
  std::string tmp = "422 ERR_NOMOTD :MOTD File is missing\r\n";
  return tmp;
}

std::string ReplyMessage::errNoAdminInfo(const std::string &arg) {
  std::string tmp = "423 ERR_NOADMININFO " + arg
      + " :No administrative info available\r\n";
  return tmp;
}

std::string ReplyMessage::errFileError(const std::string &arg,
                                       const std::string &arg2) {
  std::string tmp = "424 ERR_FILEERROR :File error doing" + arg + " on " + arg2
      + "\r\n";
  return tmp;
}

std::string ReplyMessage::errNoNickNameGiven() {
  std::string tmp = "431 ERR_NONICKNAMEGIVEN :No nickname given\r\n";
  return tmp;
}

std::string ReplyMessage::errErroneusNuckname(const std::string &arg) {
  std::string tmp = "432 ERR_ERRONEUSNICKNAME " + arg + " :Erroneus nickname\r\n";
  return tmp;
}

std::string ReplyMessage::errNicknameInUse(const std::string &arg) {
  std::string tmp = "433 ERR_NICKNAMEINUSE " + arg + " :Nickname is already in use\r\n";
  return tmp;
}

std::string ReplyMessage::errNickCollision(const std::string &arg) {
  std::string tmp = "436 ERR_NICKCOLLISION " + arg + " :Nickname collision KILL";
  return tmp;
}

std::string ReplyMessage::errUserNotInChannel(const std::string &arg,
                                              const std::string &arg2) {
  std::string tmp = "441 ERR_USERNOTINCHANNEL " + arg + " " + arg2
      + " :They aren't on that channel\r\n";
  return tmp;
}

std::string ReplyMessage::errNotOnChannel(const std::string &arg) {
  std::string tmp = "442 ERR_NOTONCHANNEL " + arg + " :You're not on that channel\r\n";
  return tmp;
}

std::string ReplyMessage::errUserOnChannel(const std::string &arg,
                                           const std::string &arg2) {
  std::string tmp = "443 ERR_USERONCHANNEL " + arg + " " + arg2
      + " :is already on channel\r\n";
  return tmp;
}

std::string ReplyMessage::errNoLogin(const std::string &arg) {
  std::string tmp = "444 ERR_NOLOGIN " + arg + " :User not logged in\r\n";
  return tmp;
}

std::string ReplyMessage::errSummonDisabled() {
  std::string tmp = "445 ERR_SUMMONDISABLED :SUMMON has been disabled\r\n";
  return tmp;
}

std::string ReplyMessage::errUserDisabled() {
  std::string tmp = "446 ERR_USERSDISABLED :USERS has been disabled\r\n";
  return tmp;
}

std::string ReplyMessage::errNotRegistered() {
  std::string tmp = "451 ERR_NOTREGISTERED :User not logged in\r\n";
  return tmp;
}

std::string ReplyMessage::errNeedMoreParams(const std::string &arg) {
  std::string tmp = "461 ERR_NEEDMOREPARAMS " + arg + " :Not enough parameters\r\n";
  return tmp;
}

std::string ReplyMessage::errAlreadyRegistered() {
  std::string tmp = "462 ERR_ALREADYREGISTRED :You may not reregister\r\n";
  return tmp;
}

std::string ReplyMessage::errNoPermForHost() {
  std::string tmp = "463 ERR_NOPERMFORHOST :Your host isn't among the privileged\r\n";
  return tmp;
}

std::string ReplyMessage::errPasswdMismatch() {
  std::string tmp = "464 ERR_PASSWDMISMATCH :Password incorrect\r\n";
  return tmp;
}

std::string ReplyMessage::errYoureBannedCreep() {
  std::string tmp = "465 ERR_YOUREBANNEDCREEP :You are banned from this server\r\n";
  return tmp;
}

std::string ReplyMessage::errKeySet(const std::string &arg) {
  std::string tmp = "467 ERR_KEYSET " + arg + " :Channel key already set\r\n";
  return tmp;
}

std::string ReplyMessage::errChannelIsFull(const std::string &arg) {
  std::string tmp = "471 ERR_CHANNELISFULL " + arg + " :Cannot join channel (+l)\r\n";
  return tmp;
}

std::string ReplyMessage::errUnknownMode(const std::string &arg) {
  std::string tmp = "472 ERR_UNKNOWNMODE " + arg + " :is unknown mode char to me\r\n";
  return tmp;
}

std::string ReplyMessage::errInviteOnlyChan(const std::string &arg) {
  std::string tmp = "473 ERR_INVITEONLYCHAN " + arg + " :Cannot join channel (+i)\r\n";
  return tmp;
}

std::string ReplyMessage::errBannedFromChan(const std::string &arg) {
  std::string tmp = "474 ERR_BANNEDFROMCHAN " + arg + " :Cannot join channel (+b)\r\n";
  return tmp;
}

std::string ReplyMessage::errBadChannelKey(const std::string &arg) {
  std::string tmp = "475 ERR_BADCHANNELKEY " + arg + " :Cannot join channel (+k)\r\n";
  return tmp;
}

std::string ReplyMessage::errNoPrivileges() {
  std::string tmp = "481 ERR_NOPRIVILEGES :Permission Denied- You're not an IRC "
         "operator\r\n";
  return tmp;
}

std::string ReplyMessage::errChaNoPrivsNeeded(const std::string &arg) {
  std::string tmp = "482 ERR_CHANOPRIVSNEEDED " + arg
      + " :You're not channel operator\r\n";
  return tmp;
}

std::string ReplyMessage::errCantKillServer() {
  std::string tmp = "483 ERR_CANTKILLSERVER :You cant kill a server!\r\n";
  return tmp;
}

std::string ReplyMessage::errNoOPerHost() {
  std::string tmp = "491 ERR_NOOPERHOST :No O-lines for your host\r\n";
  return tmp;
}

std::string ReplyMessage::errUModeUnknownFlag() {
  std::string tmp = "501 ERR_UMODEUNKNOWNFLAG :Unknown MODE flag\r\n";
  return tmp;
}

std::string ReplyMessage::errUsersDontMatch() {
  std::string tmp = "502 ERR_USERSDONTMATCH :Cant change mode for other users\r\n";
  return tmp;
}

std::string ReplyMessage::rplNone(const std::string &arg) {
  std::string tmp = "300 RPL_NONE :Dummy reply number. Not used. " + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplUserHost(const std::string &arg) {
  std::string tmp = "302 RPL_USERHOST :" + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplIsOn(const std::string &arg) {
  std::string tmp = "303 RPL_ISON :" + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplAway(const std::string &arg,
                                  const std::string &arg2) {
  std::string tmp = "301 RPL_AWAY " + arg + " :" + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplUnAway() {
  std::string tmp = "305 RPL_UNAWAY :You are no longer marked as being away\r\n";
  return tmp;
}

std::string ReplyMessage::rplNoAway() {
  std::string tmp = "306 RPL_NOWAY :You have been marked as being away\r\n";
  return tmp;
}

std::string ReplyMessage::rplWhoIsUser(const std::string &arg,
                                       const std::string &arg2,
                                       const std::string &arg3,
                                       const std::string &arg4) {
  std::string tmp = "311 RPL_WHOISUSER " + arg + " " + arg2 + " " + arg3
      + " * :" + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplWhoIsServer(const std::string &arg,
                                         const std::string &arg2,
                                         const std::string &arg3) {
  std::string tmp = "312 RPL_WHOISSERVER " + arg + " " + arg2 + " :" + arg3 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplWhoIsOperator(const std::string &arg) {
  std::string tmp = "313 RPL_WHOISOPERATOR " + arg + " :is an IRC operator\r\n";
  return tmp;
}

std::string ReplyMessage::rplWhoIsIdle(const std::string &arg,
                                       const std::string &arg2) {
  std::string tmp = "317 RPL_WHOISIDLE " + arg + " " + arg2 + " :seconds idle\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfWhoIs(const std::string &arg) {
  std::string tmp = "318 RPL_ENDOFWHOIS " + arg + " :End of /WHOIS list\r\n";
  return tmp;
}

std::string ReplyMessage::rplWhoIsChannels(const std::string &arg,
                                           const std::string &arg2) {
  std::string tmp = "319 RPL_WHOISCHANNELS " + arg + " :" + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplWhoAsUser(const std::string &arg,
                                       const std::string &arg2,
                                       const std::string &arg3,
                                       const std::string &arg4) {
  std::string tmp = "314 RPL_WHOASUSER " + arg + " " + arg2 + " " + arg3
      + " * :" + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfWhoWas(const std::string &arg) {
  std::string tmp = "369 RPL_ENDOFWHOWAS " + arg + " :End of WHOWAS\r\n";
  return tmp;
}

std::string ReplyMessage::rplListStart() {
  std::string tmp = "321 RPL_LISTSTART Channel :Users  Name\r\n";
  return tmp;
}

std::string ReplyMessage::rplList(const std::string &arg,
                                  const std::string &arg2,
                                  const std::string &arg3) {
  std::string tmp = "322 RPL_LIST " + arg + " " + arg2 + " :" + arg3 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplListEnd(const std::string &arg) {
  std::string tmp = "323 RPL_LISTEND " + arg + " :End of /LIST\r\n";
  return tmp;
}

std::string ReplyMessage::rplChannelModeIs(const std::string &arg,
                                           const std::string &arg2,
                                           const std::string &arg3) {
  std::string tmp = "324 RPL_CHANNELMODEIS " + arg + " " + arg2 + " " + arg3
      + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplNoTopic(const std::string &arg) {
  std::string tmp = "331 RPL_NOTOPIC " + arg + " :No topic is set\r\n";
  return tmp;
}

std::string ReplyMessage::rplTopic(const std::string &arg,
                                   const std::string &arg2) {
  std::string tmp = "332 RPL_TOPIC " + arg + " :" + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplInviting(const std::string &arg,
                                      const std::string &arg2) {
  std::string tmp = "341 RPL_INVITING " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplSummoning(const std::string &arg) {
  std::string tmp = "342 RPL_SUMMONING " + arg + " :Summoning user to IRC\r\n";
  return tmp;
}

std::string ReplyMessage::rplVersion(const std::string &version,
                                     const std::string &server_name,
                                     const std::string &comment) {
  std::string tmp = "351 RPL_VERSION " + version + " " + server_name + " :" + comment
      + "\r\n";
  return tmp;
}

std::string
ReplyMessage::rplWhoReply(const std::string &arg, const std::string &arg2,
                          const std::string &arg3, const std::string &arg4,
                          const std::string &arg5, const std::string &arg6,
                          const std::string &arg7, const std::string &arg8) {
  std::string tmp = "352 RPL_WHOREPLY " + arg + " " + arg2 + " " + arg3 + " " + arg4
      + " " + arg5 + " " + arg6 + " :" + arg7 + " " + arg8 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfWho(const std::string &arg) {
  std::string tmp = "315 RPL_ENDOFWHO " + arg + " :End of /WHO list\r\n";
  return tmp;
}

std::string ReplyMessage::rplNamReply(const std::string &arg,
                                      const std::string &arg2) {
  std::string tmp = "353 RPL_NAMREPLY " + arg + " :" + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfNames(const std::string &arg) {
  std::string tmp = "366 RPL_ENDOFNAMES " + arg + " :End of /NAMES list\r\n";
  return tmp;
}

std::string ReplyMessage::rplLinks(const std::string &arg,
                                   const std::string &arg2,
                                   const std::string &arg3,
                                   const std::string &arg4) {
  std::string tmp = "364 RPL_LINKS " + arg + " " + arg2 + " :" + arg3 + " " + arg4
      + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfLinks(const std::string &arg) {
  std::string tmp = "365 RPL_ENDOFLINKS " + arg + " :End of /LINKS list\r\n";
  return tmp;
}

std::string ReplyMessage::rplBanList(const std::string &arg,
                                     const std::string &arg2) {
  std::string tmp = "367 RPL_BANLIST " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfBanList(const std::string &arg) {
  std::string tmp = "368 RPL_ENDOFBANLIST " + arg + " :End of channel ban list\r\n";
  return tmp;
}

std::string ReplyMessage::rplInfo(const std::string &arg) {
  std::string tmp = "371 RPL_INFO :" + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfInfo(const std::string &arg) {
  std::string tmp = "374 RPL_ENDOFINFO " + arg + " :End of /INFO list\r\n";
  return tmp;
}

std::string ReplyMessage::rplMOTDStart(const std::string &arg) {
  std::string tmp = "375 RPL_MOTDSTART :- " + arg + " Message of the day - \r\n";
  return tmp;
}

std::string ReplyMessage::rplMOTD(const std::string &arg) {
  std::string tmp = "372 RPL_MOTD :- " + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfMOTD(const std::string &arg) {
  std::string tmp = "376 RPL_ENDOFMOTD " + arg + " :End of /MOTD command\r\n";
  return tmp;
}

std::string ReplyMessage::rplYoureOper(const std::string &arg) {
  std::string tmp = "381 RPL_YOUREOPER " + arg + " :You are now an IRC operator\r\n";
  return tmp;
}

std::string ReplyMessage::rplRehashing(const std::string &arg) {
  std::string tmp = "382 RPL_REHASHING " + arg + " :Rehashing\r\n";
  return tmp;
}

std::string ReplyMessage::rplTime(const std::string &arg,
                                  const std::string &arg2) {
  std::string tmp = "391 RPL_TIME " + arg + " :" + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplUpersStart(const std::string &arg) {
  std::string tmp = "392 RPL_USERSSTART " + arg + " :UserID   Terminal  Host\r\n";
  return tmp;
}

std::string ReplyMessage::rplUsers(const std::string &arg) {
  std::string tmp = "393 RPL_USERS :" + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfUsers(const std::string &arg) {
  std::string tmp = "394 RPL_ENDOFUSERS " + arg + " :End of users\r\n";
  return tmp;
}

std::string ReplyMessage::rplNoUsers(const std::string &arg) {
  std::string tmp = "395 RPL_NOUSERS " + arg + " :Nobody logged in\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceLink(const std::string &arg,
                                       const std::string &arg2,
                                       const std::string &arg3) {
  std::string tmp = "200 RPL_TRACELINK Link " + arg + " " + arg2 + " " + arg3
      + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceConnecting(const std::string &arg,
                                             const std::string &arg2) {
  std::string tmp = "201 RPL_TRACECONNECTING Try. " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceHandShake(const std::string &arg,
                                            const std::string &arg2) {
  std::string tmp = "202 RPL_TRACEHANDSHAKE H.S. " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceUnknown(const std::string &arg,
                                          const std::string &arg2) {
  std::string tmp = "203 RPL_TRACEUNKNOWN ???? " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceOperator(const std::string &arg,
                                           const std::string &arg2) {
  std::string tmp = "204 RPL_TRACEOPERATOR Oper " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceUser(const std::string &arg,
                                       const std::string &arg2) {
  std::string tmp = "205 RPL_TRACEUSER User " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceServer(const std::string &arg,
                                         const std::string &arg2,
                                         const std::string &arg3,
                                         const std::string &arg4,
                                         const std::string &arg5) {
  std::string tmp = "206 RPL_TRACESERVER Serv " + arg + " " + arg2 + "S " + arg3
      + "C " + arg4 + " " + arg5 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceNewType(const std::string &arg,
                                          const std::string &arg2) {
  std::string tmp = "208 RPL_TRACENEWTYPE " + arg + " 0 " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplTraceLog(const std::string &arg,
                                      const std::string &arg2) {
  std::string tmp = "261 RPL_TRACELOG File " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string
ReplyMessage::rplStatsLinkInfo(const std::string &arg, const std::string &arg2,
                               const std::string &arg3, const std::string &arg4,
                               const std::string &arg5, const std::string &arg6,
                               const std::string &arg7) {
  std::string tmp = "211 RPL_STATSLINKINFO " + arg + " " + arg2 + " " + arg3 + " "
      + arg4 + " " + arg5 + " " + arg6 + " " + arg7 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsCommands(const std::string &arg,
                                           const std::string &arg2) {
  std::string tmp = "212 RPL_STATSCOMMANDS " + arg + " " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsCLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::string tmp = "213 RPL_STATSCLINE C " + arg + " * " + arg2 + " " + arg3 + " "
      + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsNLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::string tmp = "214 RPL_STATSNLINE N " + arg + " * " + arg2 + " " + arg3 + " "
      + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsILine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::string tmp = "215 RPL_STATSILINE I " + arg + " * " + arg2 + " " + arg3 + " "
      + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsKLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::string tmp = "216 RPL_STATSKLINE K " + arg + " * " + arg2 + " " + arg3 + " "
      + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsYLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3,
                                        const std::string &arg4) {
  std::string tmp = "218 RPL_STATSYLINE Y " + arg + " " + arg2 + " " + arg3 + " "
      + arg4 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplEndOfStats(const std::string &arg) {
  std::string tmp = "219 RPL_ENDOFSTATS " + arg + " :End of /STATS report\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsLLine(const std::string &arg,
                                        const std::string &arg2,
                                        const std::string &arg3) {
  std::string tmp = "241 RPL_STATSLLINE L " + arg + " * " + arg2 + " " + arg3
      + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsUpTime(const std::string &arg) {
  std::string tmp = "242 RPL_STATSUPTIME :Server Up " + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsOLine(const std::string &arg,
                                        const std::string &arg2) {
  std::string tmp = "243 RPL_STATSOLINE O " + arg + " * " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplStatsHLine(const std::string &arg,
                                        const std::string &arg2) {
  std::string tmp = "244 RPL_STATSHLINE H " + arg + " * " + arg2 + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplUModeIs(const std::string &arg) {
  std::string tmp = "221 RPL_UMODEIS " + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplLUserClient(const std::string &arg,
                                         const std::string &arg2,
                                         const std::string &arg3) {
  std::string tmp = "251 RPL_LUSERCLIENT :There are " + arg + " users and " + arg2
      + " invisible on " + arg3 + " servers\r\n";
  return tmp;
}

std::string ReplyMessage::rplLUserOp(const std::string &arg) {
  std::string tmp = "252 RPL_LUSEROP " + arg + " :operator(s) online\r\n";
  return tmp;
}

std::string ReplyMessage::rplLUserUnknown(const std::string &arg) {
  std::string tmp = "253 RPL_LUSERUNKNOWN " + arg + " :unknown connection(s)\r\n";
  return tmp;
}

std::string ReplyMessage::rplLUserChannels(const std::string &arg) {
  std::string tmp = "254 RPL_LUSERCHANNELS " + arg + " :channels formed\r\n";
  return tmp;
}

std::string ReplyMessage::rplLUserMe(const std::string &arg,
                                     const std::string &arg2) {
  std::string tmp = "255 RPL_LUSERME :I have " + arg + " clients and " + arg2
      + " servers\r\n";
  return tmp;
}

std::string ReplyMessage::rplAdminMe(const std::string &arg) {
  std::string tmp = "256 RPL_ADMINME " + arg + " :Administrative info\r\n";
  return tmp;
}

std::string ReplyMessage::rplAdminLoc1(const std::string &arg) {
  std::string tmp = "257 RPL_ADMINLOC1 :" + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplAdminLoc2(const std::string &arg) {
  std::string tmp = "258 RPL_ADMINLOC2 :" + arg + "\r\n";
  return tmp;
}

std::string ReplyMessage::rplAdminMail(const std::string &arg) {
  std::string tmp = "259 RPL_ADMINEMAIL :" + arg + "\r\n";
  return tmp;
}
