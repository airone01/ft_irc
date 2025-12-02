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
	static void rplAway(const std::string& arg); // "<nick> :<away message>" 301
	static void rplUnAway(const std::string& arg); // ":You are no longer marked as being away" 305
	static void rplNoAway(const std::string& arg); // ":You have been marked as being away" 306
	static void rplWhoIsUser(const std::string& arg); // "<nick> <user> <host> * :<real name>" 311
	static void rplWhoIsServer(const std::string& arg); // "<nick> <server> :<server info>" 312
	static void rplWhoIsOperator(const std::string& arg); // "<nick> :is an IRC operator" 313
	static void rplWhoIsIdle(const std::string& arg); // "<nick> <integer> :seconds idle" 317
	static void rplEndOfWhoIs(const std::string& arg); // "<nick> :End of /WHOIS list" 318
	static void rplWhoIsChannels(const std::string& arg); // "<nick> :{[@|+]<channel><space>}" 319
	static void rplWhoAsUser(const std::string& arg); // "<nick> <user> <host> * :<real name>" 314
	static void rplEndOfWhoWas(const std::string& arg); // "<nick> :End of WHOWAS" 369
	static void rplListStart(const std::string& arg); // "Channel :Users  Name" 321
	static void rplList(const std::string& arg); // "<channel> <# visible> :<topic>" 322
	static void rplListEnd(const std::string& arg); // ":End of /LIST" 323
	static void rplChannelModeIs(const std::string& arg); // "<channel> <mode> <mode params>" 324
	static void rplNoTopic(const std::string& arg); // "<channel> :No topic is set" 331
	static void rplTopic(const std::string& arg); // "<channel> :<topic>" 332
	static void rplInviting(const std::string& arg); // "<channel> <nick>" 341
	static void rplSummoning(const std::string& arg); // "<user> :Summoning user to IRC" 342
	static void rplVersion(const std::string& arg); // "<version>.<debuglevel> <server> :<comments>" 351
	/TODO
	static void rpl(const std::string& arg); //
	//        352     RPL_WHOREPLY
	//                        "<channel> <user> <host> <server> <nick> \
	//                         <H|G>[*][@|+] :<hopcount> <real name>"
	static void rpl(const std::string& arg); //
	//        315     RPL_ENDOFWHO
	//                        "<name> :End of /WHO list"
	static void rpl(const std::string& arg); //
	//        353     RPL_NAMREPLY
	//                        "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	static void rpl(const std::string& arg); //
	//        366     RPL_ENDOFNAMES
	//                        "<channel> :End of /NAMES list"
	static void rpl(const std::string& arg); //
	//        364     RPL_LINKS
	//                        "<mask> <server> :<hopcount> <server info>"
	static void rpl(const std::string& arg); //
	//        365     RPL_ENDOFLINKS
	//                        "<mask> :End of /LINKS list"
	static void rpl(const std::string& arg); //
	//        367     RPL_BANLIST
	//                        "<channel> <banid>"
	static void rpl(const std::string& arg); //
	//        368     RPL_ENDOFBANLIST
	//                        "<channel> :End of channel ban list"
	static void rpl(const std::string& arg); //
	//        371     RPL_INFO
	//                        ":<string>"
	static void rpl(const std::string& arg); //
	//        374     RPL_ENDOFINFO
	//                        ":End of /INFO list"
	static void rpl(const std::string& arg); //
	//        375     RPL_MOTDSTART
	//                        ":- <server> Message of the day - "
	static void rpl(const std::string& arg); //
	//        372     RPL_MOTD
	//                        ":- <text>"
	static void rpl(const std::string& arg); //
	//        376     RPL_ENDOFMOTD
	//                        ":End of /MOTD command"
	static void rpl(const std::string& arg); //
	//        381     RPL_YOUREOPER
	//                        ":You are now an IRC operator"
	static void rpl(const std::string& arg); //
	//        382     RPL_REHASHING
	//                        "<config file> :Rehashing"
	static void rpl(const std::string& arg); //
	//        391     RPL_TIME
	//                        "<server> :<string showing server's local time>"
	static void rpl(const std::string& arg); //
	//        392     RPL_USERSSTART
	//                        ":UserID   Terminal  Host"
	static void rpl(const std::string& arg); //
	//        393     RPL_USERS
	//                        ":%-8s %-9s %-8s"
	static void rpl(const std::string& arg); //
	//        394     RPL_ENDOFUSERS
	//                        ":End of users"
	static void rpl(const std::string& arg); //
	//        395     RPL_NOUSERS
	//                        ":Nobody logged in"
	static void rpl(const std::string& arg); //
	//        200     RPL_TRACELINK
	//                        "Link <version & debug level> <destination> \
	//                         <next server>"
	static void rpl(const std::string& arg); //
	//        201     RPL_TRACECONNECTING
	//                        "Try. <class> <server>"
	static void rpl(const std::string& arg); //
	//        202     RPL_TRACEHANDSHAKE
	//                        "H.S. <class> <server>"
	static void rpl(const std::string& arg); //
	//        203     RPL_TRACEUNKNOWN
	//                        "???? <class> [<client IP address in dot form>]"
	static void rpl(const std::string& arg); //
	//        204     RPL_TRACEOPERATOR
	//                        "Oper <class> <nick>"
	static void rpl(const std::string& arg); //
	//        205     RPL_TRACEUSER
	//                        "User <class> <nick>"
	static void rpl(const std::string& arg); //
	//        206     RPL_TRACESERVER
	//                        "Serv <class> <int>S <int>C <server> \
	//                         <nick!user|*!*>@<host|server>"
	static void rpl(const std::string& arg); //
	//        208     RPL_TRACENEWTYPE
	//                        "<newtype> 0 <client name>"
	static void rpl(const std::string& arg); //
	//        261     RPL_TRACELOG
	//                        "File <logfile> <debug level>"
	static void rpl(const std::string& arg); //
	//        211     RPL_STATSLINKINFO
	//                        "<linkname> <sendq> <sent messages> \
	//                         <sent bytes> <received messages> \
	//                         <received bytes> <time open>"
	static void rpl(const std::string& arg); //
	//        212     RPL_STATSCOMMANDS
	//                        "<command> <count>"
	static void rpl(const std::string& arg); //
	//        213     RPL_STATSCLINE
	//                        "C <host> * <name> <port> <class>"
	static void rpl(const std::string& arg); //
	//        214     RPL_STATSNLINE
	//                        "N <host> * <name> <port> <class>"
	static void rpl(const std::string& arg); //
	//        215     RPL_STATSILINE
	//                        "I <host> * <host> <port> <class>"
	static void rpl(const std::string& arg); //
	//        216     RPL_STATSKLINE
	//                        "K <host> * <username> <port> <class>"
	static void rpl(const std::string& arg); //
	//        218     RPL_STATSYLINE
	//                        "Y <class> <ping frequency> <connect \
	//                         frequency> <max sendq>"
	static void rpl(const std::string& arg); //
	//        219     RPL_ENDOFSTATS
	//                        "<stats letter> :End of /STATS report"
	static void rpl(const std::string& arg); //
	//        241     RPL_STATSLLINE
	//                        "L <hostmask> * <servername> <maxdepth>"
	static void rpl(const std::string& arg); //
	//        242     RPL_STATSUPTIME
	//                        ":Server Up %d days %d:%02d:%02d"
	static void rpl(const std::string& arg); //
	//        243     RPL_STATSOLINE
	//                        "O <hostmask> * <name>"
	static void rpl(const std::string& arg); //
	//        244     RPL_STATSHLINE
	//                        "H <hostmask> * <servername>"
	static void rpl(const std::string& arg); //
	//        221     RPL_UMODEIS
	//                        "<user mode string>"
	static void rpl(const std::string& arg); //
	//        251     RPL_LUSERCLIENT
	//                        ":There are <integer> users and <integer> \
	//                         invisible on <integer> servers"
	static void rpl(const std::string& arg); //
	//        252     RPL_LUSEROP
	//                        "<integer> :operator(s) online"
	static void rpl(const std::string& arg); //
	//        253     RPL_LUSERUNKNOWN
	//                        "<integer> :unknown connection(s)"
	static void rpl(const std::string& arg); //
	//        254     RPL_LUSERCHANNELS
	//                        "<integer> :channels formed"
	static void rpl(const std::string& arg); //
	//        255     RPL_LUSERME
	//                        ":I have <integer> clients and <integer> \
	//                          servers"
	static void rpl(const std::string& arg); //
	//        256     RPL_ADMINME
	//                        "<server> :Administrative info"
	static void rpl(const std::string& arg); //
	//        257     RPL_ADMINLOC1
	//                        ":<admin info>"
	static void rpl(const std::string& arg); //
	//        258     RPL_ADMINLOC2
	//                        ":<admin info>"
	static void rpl(const std::string& arg); //
	//        259     RPL_ADMINEMAIL
	//                        ":<admin info>"
	static void rpl(const std::string& arg); //
};

#endif
