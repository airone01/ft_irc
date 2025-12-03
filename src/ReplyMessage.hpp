#ifndef REPLYMESSAGE_HPP
#define REPLYMESSAGE_HPP

#include <iostream>

class ReplyMessage {
public:
	static void errNoSuchNick(const std::string& arg); // "<nickname> :No such nick/channel" 401
	static void errNoSuchServer(const std::string& arg); // "<server name> :No such server" 402
	static void errNoSuckChannel(const std::string& arg); // "<channel name> :No such channel" 403
	static void errCannotSendToChannel(const std::string& arg); // "<channel name> :Cannot send to channel" 404
	static void errTooManyChannels(const std::string& arg); // "<channel name> :You have joined too many channels" 405
	static void errWasNoSuckNick(const std::string& arg); // "<nickname> :There was no such nickname" 406
	static void errTooManyTargets(const std::string& arg); // "<target> :Duplicate recipients. No message delivered" 407
	static void errNoOrigin(); // ":No origin specified" 409
	static void errNoRecipient(const std::string& arg); // ":No recipient given (<command>)" 411
	static void errNoTextToSend(); // ":No text to send" 412
	static void errNoTopLevel(const std::string& arg); // "<mask> :No toplevel domain specified" 413
	static void errWildTopLevel(const std::string& arg); // "<mask> :Wildcard in toplevel domain" 414
	static void errUnknownCommand(const std::string& arg); // "<command> :Unknown command" 421
	static void errNoMOTD(); // ":MOTD File is missing" 422
	static void errNoAdminInfo(const std::string& arg); // "<server> :No administrative info available" 423
	static void errFileError(const std::string& arg, const std::string& arg2); // ":File error doing <file op> on <file>" 424
	static void errNoNickNameGiven(); //  ":No nickname given" 431
	static void errErroneusNuckname(const std::string& arg); //  "<nick> :Erroneus nickname" 432
	static void errNicknameInUse(const std::string& arg); //  "<nick> :Nickname is already in use" 433
	static void errNickCollision(const std::string& arg); //  "<nick> :Nickname collision KILL" 434
	static void errUserNotInChannel(const std::string& arg, const std::string& arg2); //  "<nick> <channel> :They aren't on that channel" 441
	static void errNotOnChannel(const std::string& arg); // "<channel> :You're not on that channel" 442
	static void errUserOnChannel(const std::string& arg, const std::string& arg2); // "<user> <channel> :is already on channel" 443
	static void errNoLogin(const std::string& arg); // "<user> :User not logged in" 444
	static void errSummonDisabled(); // ":SUMMON has been disabled" 445
	static void errUserDisabled(); //  ":USERS has been disabled"  446
	static void errNotRegistered(); //  ":You have not registered" 451
	static void errNeedMoreParams(const std::string& arg); //  "<command> :Not enough parameters" 461
	static void errAlreadyRegistered(); //  ":You may not reregister" 462
	static void errNoPermForHost(); //  ":Your host isn't among the privileged" 463
	static void errPasswdMismatch(); //  ":Password incorrect" 464
	static void errYoureBannedCreep(); //  ":You are banned from this server" 465
	static void errKeySet(const std::string& arg); //  "<channel> :Channel key already set" 467
	static void errChannelIsFull(const std::string& arg); //  "<channel> :Cannot join channel (+l)" 471
	static void errUnknownMode(const std::string& arg); //  "<char> :is unknown mode char to me" 472
	static void errInviteOnlyChan(const std::string& arg); //  "<channel> :Cannot join channel (+i)" 473
	static void errBannedFromChan(const std::string& arg); //  "<channel> :Cannot join channel (+b)" 474
	static void errBadChannelKey(const std::string& arg); //  "<channel> :Cannot join channel (+k)" 475
	static void errNoPrivileges(); //  ":Permission Denied- You're not an IRC operator" 481
	static void errChaNoPrivsNeeded(const std::string& arg); //  "<channel> :You're not channel operator" 482
	static void errCantKillServer(); //  ":You cant kill a server!" 483
	static void errNoOPerHost(); // ":No O-lines for your host" 491
	static void errUModeUnknownFlag(); // ":Unknown MODE flag" 501
	static void errUserDontMatch(); //  ":Cant change mode for other users" 502
	static void rplNone(const std::string& arg); // Dummy reply number. Not used. 300
	static void rplUserHost(const std::string& arg); // ":[<reply>{<space><reply>}]" 302
	static void rplIsOn(const std::string& arg); // ":[<nick> {<space><nick>}]" 303
	static void rplAway(const std::string& arg, const std::string& arg2); // "<nick> :<away message>" 301
	static void rplUnAway(); // ":You are no longer marked as being away" 305
	static void rplNoAway(); // ":You have been marked as being away" 306
	static void rplWhoIsUser(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "<nick> <user> <host> * :<real name>" 311
	static void rplWhoIsServer(const std::string& arg, const std::string& arg2, const std::string& arg3); // "<nick> <server> :<server info>" 312
	static void rplWhoIsOperator(const std::string& arg); // "<nick> :is an IRC operator" 313
	static void rplWhoIsIdle(const std::string& arg, const std::string& arg2); // "<nick> <integer> :seconds idle" 317
	static void rplEndOfWhoIs(const std::string& arg); // "<nick> :End of /WHOIS list" 318
	static void rplWhoIsChannels(const std::string& arg, const std::string& arg2); // "<nick> :{[@|+]<channel><space>}" 319
	static void rplWhoAsUser(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "<nick> <user> <host> * :<real name>" 314
	static void rplEndOfWhoWas(const std::string& arg); // "<nick> :End of WHOWAS" 369
	static void rplListStart(); // "Channel :Users  Name" 321
	static void rplList(const std::string& arg, const std::string& arg2, const std::string& arg3); // "<channel> <# visible> :<topic>" 322
	static void rplListEnd(const std::string& arg); // ":End of /LIST" 323
	static void rplChannelModeIs(const std::string& arg, const std::string& arg2, const std::string& arg3); // "<channel> <mode> <mode params>" 324
	static void rplNoTopic(const std::string& arg); // "<channel> :No topic is set" 331
	static void rplTopic(const std::string& arg, const std::string& arg2); // "<channel> :<topic>" 332
	static void rplInviting(const std::string& arg, const std::string& arg2); // "<channel> <nick>" 341
	static void rplSummoning(const std::string& arg); // "<user> :Summoning user to IRC" 342
	static void rplVersion(const std::string& arg, const std::string& arg2, const std::string& arg3); // "<version>.<debuglevel> <server> :<comments>" 351
	static void rplWhoReply(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5, const std::string& arg6, const std::string& arg7, const std::string& arg8); // "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>" 352
	static void rplEndOfWho(const std::string& arg); // "<name> :End of /WHO list" 315
	static void rplNamReply(const std::string& arg, const std::string& arg2); // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]" 353
	static void rplEndOfNames(const std::string& arg); // "<channel> :End of /NAMES list" 366
	static void rplLinks(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "<mask> <server> :<hopcount> <server info>" 364
	static void rplEndOfLinks(const std::string& arg); // "<mask> :End of /LINKS list" 365
	static void rplBanList(const std::string& arg, const std::string& arg2); // "<channel> <banid>" 367
	static void rplEndOfBanList(const std::string& arg); // "<channel> :End of channel ban list" 368
	static void rplInfo(const std::string& arg); // ":<string>" 371
	static void rplEndOfInfo(const std::string& arg); // ":End of /INFO list" 374
	static void rplMOTDStart(const std::string& arg); // ":- <server> Message of the day - " 375
	static void rplMOTD(const std::string& arg); // ":- <text>" 372
	static void rplEndOfMOTD(const std::string& arg); // ":End of /MOTD command" 376
	static void rplYoureOper(const std::string& arg); // ":You are now an IRC operator" 381
	static void rplRehashing(const std::string& arg); // "<config file> :Rehashing" 382
	static void rplTime(const std::string& arg, const std::string& arg2); // "<server> :<string showing server's local time>" 391
	static void rplUpersStart(const std::string& arg); // ":UserID   Terminal  Host" 392
	static void rplUsers(const std::string& arg); // ":%-8s %-9s %-8s" 393
	static void rplEndOfUsers(const std::string& arg); // ":End of users" 394
	static void rplNoUsers(const std::string& arg); // ":Nobody logged in" 395
	static void rplTraceLink(const std::string& arg, const std::string& arg2, const std::string& arg3); // "Link <version & debug level> <destination> <next server>" 200
	static void rplTraceConnecting(const std::string& arg, const std::string& arg2); // "Try. <class> <server>" 201
	static void rplTraceHandShake(const std::string& arg, const std::string& arg2); // "H.S. <class> <server>" 202
	static void rplTraceUnknown(const std::string& arg, const std::string& arg2); // "???? <class> [<client IP address in dot form>]" 203
	static void rplTraceOperator(const std::string& arg, const std::string& arg2); // "Oper <class> <nick>" 204
	static void rplTraceUser(const std::string& arg, const std::string& arg2); // "User <class> <nick>" 205
	static void rplTraceServer(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5); // "Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server>" 206
	static void rplTraceNewType(const std::string& arg, const std::string& arg2); // "<newtype> 0 <client name>" 208
	static void rplTraceLog(const std::string& arg, const std::string& arg2); // "File <logfile> <debug level>" 261
	static void rplStatsLinkInfo(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4, const std::string& arg5, const std::string& arg6, const std::string& arg7); // "<linkname> <sendq> <sent messages> <sent bytes> <received messages> <received bytes> <time open>" 211
	static void rplStatsCommands(const std::string& arg, const std::string& arg2); // "<command> <count>" 212
	static void rplStatsCLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "C <host> * <name> <port> <class>" 213
	static void rplStatsNLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "N <host> * <name> <port> <class>" 214
	static void rplStatsILine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "I <host> * <host> <port> <class>" 215
	static void rplStatsKLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "K <host> * <username> <port> <class>" 216
	static void rplStatsYLine(const std::string& arg, const std::string& arg2, const std::string& arg3, const std::string& arg4); // "Y <class> <ping frequency> <connect frequency> <max sendq>" 218
	static void rplEndOfStats(const std::string& arg); // "<stats letter> :End of /STATS report" 219
	static void rplStatsLLine(const std::string& arg, const std::string& arg2, const std::string& arg3); // "L <hostmask> * <servername> <maxdepth>" 241
	static void rplStatsUpTime(const std::string& arg); // ":Server Up %d days %d:%02d:%02d" 242
	static void rplStatsOLine(const std::string& arg, const std::string& arg2); // "O <hostmask> * <name>" 243
	static void rplStatsHLine(const std::string& arg, const std::string& arg2); // "H <hostmask> * <servername>" 244
	static void rplUModeIs(const std::string& arg); // "<user mode string>" 221
	static void rplLUserClient(const std::string& arg, const std::string& arg2, const std::string& arg3); // ":There are <integer> users and <integer> invisible on <integer> servers" 251
	static void rplLUserOp(const std::string& arg); // "<integer> :operator(s) online" 252
	static void rplLUserUnknown(const std::string& arg); // "<integer> :unknown connection(s)" 253
	static void rplLUserChannels(const std::string& arg); // "<integer> :channels formed" 254
	static void rplLUserMe(const std::string& arg, const std::string& arg2); // ":I have <integer> clients and <integer> servers" 255
	static void rplAdminMe(const std::string& arg); // "<server> :Administrative info" 256
	static void rplAdminLoc1(const std::string& arg); // ":<admin info>" 257
	static void rplAdminLoc2(const std::string& arg); // ":<admin info>" 258
	static void rplAdminMail(const std::string& arg); // ":<admin info>" 259
};

#endif
