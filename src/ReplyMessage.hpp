#ifndef REPLYMESSAGE_HPP
#define REPLYMESSAGE_HPP

#include <string>

class ReplyMessage {
public:
  static std::string errNoSuchNick(
      const std::string &arg); // "<nickname> :No such nick/channel" 401
  static std::string errNoSuchServer(
      const std::string &arg); // "<server name> :No such server" 402
  static std::string errNoSuchChannel(
      const std::string &arg); // "<channel name> :No such channel" 403
  static std::string errCannotSendToChannel(
      const std::string &arg); // "<channel name> :Cannot send to channel" 404
  static std::string errTooManyChannels(
      const std::string
          &arg); // "<channel name> :You have joined too many channels" 405
  static std::string errWasNoSuchNick(
      const std::string &arg); // "<nickname> :There was no such nickname" 406
  static std::string errTooManyTargets(
      const std::string
          &arg); // "<target> :Duplicate recipients. No message delivered" 407
  static std::string errNoOrigin(); // ":No origin specified" 409
  static std::string errNoRecipient(
      const std::string &arg);          // ":No recipient given (<command>)" 411
  static std::string errNoTextToSend(); // ":No text to send" 412
  static std::string errNoTopLevel(
      const std::string &arg); // "<mask> :No toplevel domain specified" 413
  static std::string errWildTopLevel(
      const std::string &arg); // "<mask> :Wildcard in toplevel domain" 414
  static std::string
  errUnknownCommand(const std::string &arg); // "<command> :Unknown command" 421
  static std::string errNoMOTD();            // ":MOTD File is missing" 422
  static std::string
  errNoAdminInfo(const std::string
                     &arg); // "<server> :No administrative info available" 423
  static std::string errFileError(
      const std::string &arg,
      const std::string &arg2); // ":File error doing <file op> on <file>" 424
  static std::string errNoNickNameGiven(); //  ":No nickname given" 431
  static std::string errErroneusNuckname(
      const std::string &arg); //  "<nick> :Erroneus nickname" 432
  static std::string errNicknameInUse(
      const std::string &arg); //  "<nick> :Nickname is already in use" 433
  static std::string errNickCollision(
      const std::string &arg); //  "<nick> :Nickname collision KILL" 434
  static std::string errUserNotInChannel(
      const std::string &arg,
      const std::string
          &arg2); //  "<nick> <channel> :They aren't on that channel" 441
  static std::string errNotOnChannel(
      const std::string &arg); // "<channel> :You're not on that channel" 442
  static std::string errUserOnChannel(
      const std::string &arg,
      const std::string &arg2); // "<user> <channel> :is already on channel" 443
  static std::string
  errNoLogin(const std::string &arg);     // "<user> :User not logged in" 444
  static std::string errSummonDisabled(); // ":SUMMON has been disabled" 445
  static std::string errUserDisabled();   //  ":USERS has been disabled"  446
  static std::string errNotRegistered();  //  ":You have not registered" 451
  static std::string errNeedMoreParams(
      const std::string &arg); //  "<command> :Not enough parameters" 461
  static std::string errAlreadyRegistered(); //  ":You may not reregister" 462
  static std::string
  errNoPermForHost(); //  ":Your host isn't among the privileged" 463
  static std::string errPasswdMismatch(); //  ":Password incorrect" 464
  static std::string
  errYoureBannedCreep(); //  ":You are banned from this server" 465
  static std::string errKeySet(
      const std::string &arg); //  "<channel> :Channel key already set" 467
  static std::string errChannelIsFull(
      const std::string &arg); //  "<channel> :Cannot join channel (+l)" 471
  static std::string errUnknownMode(
      const std::string &arg); //  "<char> :is unknown mode char to me" 472
  static std::string errInviteOnlyChan(
      const std::string &arg); //  "<channel> :Cannot join channel (+i)" 473
  static std::string errBannedFromChan(
      const std::string &arg); //  "<channel> :Cannot join channel (+b)" 474
  static std::string errBadChannelKey(
      const std::string &arg); //  "<channel> :Cannot join channel (+k)" 475
  static std::string
  errNoPrivileges(); //  ":Permission Denied- You're not an IRC operator" 481
  static std::string errChaNoPrivsNeeded(
      const std::string &arg); //  "<channel> :You're not channel operator" 482
  static std::string errCantKillServer();   //  ":You cant kill a server!" 483
  static std::string errNoOPerHost();       // ":No O-lines for your host" 491
  static std::string errUModeUnknownFlag(); // ":Unknown MODE flag" 501
  static std::string
  errUsersDontMatch(); //  ":Cant change mode for other users" 502
  static std::string
  rplNone(const std::string &arg); // Dummy reply number. Not used. 300
  static std::string
  rplUserHost(const std::string &arg); // ":[<reply>{<space><reply>}]" 302
  static std::string
  rplIsOn(const std::string &arg); // ":[<nick> {<space><nick>}]" 303
  static std::string
  rplAway(const std::string &arg,
          const std::string &arg2); // "<nick> :<away message>" 301
  static std::string
  rplUnAway(); // ":You are no longer marked as being away" 305
  static std::string rplNoAway(); // ":You have been marked as being away" 306
  static std::string rplWhoIsUser(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "<nick> <user> <host> * :<real name>" 311
  static std::string rplWhoIsServer(
      const std::string &arg, const std::string &arg2,
      const std::string &arg3); // "<nick> <server> :<server info>" 312
  static std::string
  rplWhoIsOperator(const std::string &arg); // "<nick> :is an IRC operator" 313
  static std::string
  rplWhoIsIdle(const std::string &arg,
               const std::string &arg2); // "<nick> <integer> :seconds idle" 317
  static std::string
  rplEndOfWhoIs(const std::string &arg); // "<nick> :End of /WHOIS list" 318
  static std::string rplWhoIsChannels(
      const std::string &arg,
      const std::string &arg2); // "<nick> :{[@|+]<channel><space>}" 319
  static std::string rplWhoAsUser(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "<nick> <user> <host> * :<real name>" 314
  static std::string
  rplEndOfWhoWas(const std::string &arg); // "<nick> :End of WHOWAS" 369
  static std::string rplListStart();      // "Channel :Users  Name" 321
  static std::string
  rplList(const std::string &arg, const std::string &arg2,
          const std::string &arg3); // "<channel> <# visible> :<topic>" 322
  static std::string rplListEnd(const std::string &arg); // ":End of /LIST" 323
  static std::string rplChannelModeIs(
      const std::string &arg, const std::string &arg2,
      const std::string &arg3); // "<channel> <mode> <mode params>" 324
  static std::string
  rplNoTopic(const std::string &arg); // "<channel> :No topic is set" 331
  static std::string
  rplTopic(const std::string &arg,
           const std::string &arg2); // "<channel> :<topic>" 332
  static std::string
  rplInviting(const std::string &arg,
              const std::string &arg2); // "<channel> <nick>" 341
  static std::string
  rplSummoning(const std::string &arg); // "<user> :Summoning user to IRC" 342
  static std::string
  rplVersion(const std::string &arg, const std::string &arg2,
             const std::string
                 &arg3); // "<version>.<debuglevel> <server> :<comments>" 351
  static std::string rplWhoReply(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4, const std::string &arg5, const std::string &arg6,
      const std::string &arg7,
      const std::string &arg8); // "<channel> <user> <host> <server> <nick>
                                // <H|G>[*][@|+] :<hopcount> <real name>" 352
  static std::string
  rplEndOfWho(const std::string &arg); // "<name> :End of /WHO list" 315
  static std::string
  rplNamReply(const std::string &arg,
              const std::string
                  &arg2); // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]" 353
  static std::string
  rplEndOfNames(const std::string &arg); // "<channel> :End of /NAMES list" 366
  static std::string
  rplLinks(const std::string &arg, const std::string &arg2,
           const std::string &arg3,
           const std::string
               &arg4); // "<mask> <server> :<hopcount> <server info>" 364
  static std::string
  rplEndOfLinks(const std::string &arg); // "<mask> :End of /LINKS list" 365
  static std::string
  rplBanList(const std::string &arg,
             const std::string &arg2); // "<channel> <banid>" 367
  static std::string rplEndOfBanList(
      const std::string &arg); // "<channel> :End of channel ban list" 368
  static std::string rplInfo(const std::string &arg); // ":<string>" 371
  static std::string
  rplEndOfInfo(const std::string &arg); // ":End of /INFO list" 374
  static std::string rplMOTDStart(
      const std::string &arg); // ":- <server> Message of the day - " 375
  static std::string rplMOTD(const std::string &arg); // ":- <text>" 372
  static std::string
  rplEndOfMOTD(const std::string &arg); // ":End of /MOTD command" 376
  static std::string
  rplYoureOper(const std::string &arg); // ":You are now an IRC operator" 381
  static std::string
  rplRehashing(const std::string &arg); // "<config file> :Rehashing" 382
  static std::string
  rplTime(const std::string &arg,
          const std::string
              &arg2); // "<server> :<string showing server's local time>" 391
  static std::string
  rplUpersStart(const std::string &arg); // ":UserID   Terminal  Host" 392
  static std::string rplUsers(const std::string &arg); // ":%-8s %-9s %-8s" 393
  static std::string
  rplEndOfUsers(const std::string &arg); // ":End of users" 394
  static std::string
  rplNoUsers(const std::string &arg); // ":Nobody logged in" 395
  static std::string
  rplTraceLink(const std::string &arg, const std::string &arg2,
               const std::string &arg3); // "Link <version & debug level>
                                         // <destination> <next server>" 200
  static std::string
  rplTraceConnecting(const std::string &arg,
                     const std::string &arg2); // "Try. <class> <server>" 201
  static std::string
  rplTraceHandShake(const std::string &arg,
                    const std::string &arg2); // "H.S. <class> <server>" 202
  static std::string rplTraceUnknown(
      const std::string &arg,
      const std::string
          &arg2); // "???? <class> [<client IP address in dot form>]" 203
  static std::string
  rplTraceOperator(const std::string &arg,
                   const std::string &arg2); // "Oper <class> <nick>" 204
  static std::string
  rplTraceUser(const std::string &arg,
               const std::string &arg2); // "User <class> <nick>" 205
  static std::string rplTraceServer(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4,
      const std::string &arg5); // "Serv <class> <int>S <int>C <server>
                                // <nick!user|*!*>@<host|server>" 206
  static std::string
  rplTraceNewType(const std::string &arg,
                  const std::string &arg2); // "<newtype> 0 <client name>" 208
  static std::string
  rplTraceLog(const std::string &arg,
              const std::string &arg2); // "File <logfile> <debug level>" 261
  static std::string rplStatsLinkInfo(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4, const std::string &arg5, const std::string &arg6,
      const std::string
          &arg7); // "<linkname> <sendq> <sent messages> <sent bytes> <received
                  // messages> <received bytes> <time open>" 211
  static std::string
  rplStatsCommands(const std::string &arg,
                   const std::string &arg2); // "<command> <count>" 212
  static std::string rplStatsCLine(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "C <host> * <name> <port> <class>" 213
  static std::string rplStatsNLine(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "N <host> * <name> <port> <class>" 214
  static std::string rplStatsILine(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "I <host> * <host> <port> <class>" 215
  static std::string rplStatsKLine(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "K <host> * <username> <port> <class>" 216
  static std::string rplStatsYLine(
      const std::string &arg, const std::string &arg2, const std::string &arg3,
      const std::string &arg4); // "Y <class> <ping frequency> <connect
                                // frequency> <max sendq>" 218
  static std::string rplEndOfStats(
      const std::string &arg); // "<stats letter> :End of /STATS report" 219
  static std::string rplStatsLLine(
      const std::string &arg, const std::string &arg2,
      const std::string &arg3); // "L <hostmask> * <servername> <maxdepth>" 241
  static std::string rplStatsUpTime(
      const std::string &arg); // ":Server Up %d days %d:%02d:%02d" 242
  static std::string
  rplStatsOLine(const std::string &arg,
                const std::string &arg2); // "O <hostmask> * <name>" 243
  static std::string
  rplStatsHLine(const std::string &arg,
                const std::string &arg2); // "H <hostmask> * <servername>" 244
  static std::string
  rplUModeIs(const std::string &arg); // "<user mode string>" 221
  static std::string rplLUserClient(
      const std::string &arg, const std::string &arg2,
      const std::string &arg3); // ":There are <integer> users and <integer>
                                // invisible on <integer> servers" 251
  static std::string
  rplLUserOp(const std::string &arg); // "<integer> :operator(s) online" 252
  static std::string rplLUserUnknown(
      const std::string &arg); // "<integer> :unknown connection(s)" 253
  static std::string
  rplLUserChannels(const std::string &arg); // "<integer> :channels formed" 254
  static std::string
  rplLUserMe(const std::string &arg,
             const std::string &
                 arg2); // ":I have <integer> clients and <integer> servers" 255
  static std::string
  rplAdminMe(const std::string &arg); // "<server> :Administrative info" 256
  static std::string
  rplAdminLoc1(const std::string &arg); // ":<admin info>" 257
  static std::string
  rplAdminLoc2(const std::string &arg); // ":<admin info>" 258
  static std::string
  rplAdminMail(const std::string &arg); // ":<admin info>" 259
};

#endif
