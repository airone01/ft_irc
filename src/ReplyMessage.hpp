#ifndef REPLYMESSAGE_HPP
#define REPLYMESSAGE_HPP

#include <iostream>

class ReplyMessage {
public:
	static void errNoSuchNick(const std::string& arg); // "<nickname> :No such nick/channel" 401
	static void errNoSuchServer(const std::string& arg); // "<server name> :No such server" 402
	static void errNoSuckChannel(const std::string& arg); // "<channel name> :No such channel" 403
	static void errCannotSendToChannel(const std::string& arg); // "<channel name> :Cannot send to channel" 404
	static void err_TooManyChannels(const std::string& arg); // "<channel name> :You have joined too many channels" 405
	static void err_WasNoSuckNick(const std::string& arg); // "<nickname> :There was no such nickname" 406
	static void err_TooManyTargets(const std::string& arg); // "<target> :Duplicate recipients. No message delivered" 407
	static void err_NoOrigin(); // ":No origin specified" 409
	static void err_NoRecipient(const std::string& arg); // ":No recipient given (<command>)" 411
	static void err_NoTextToSend(); // ":No text to send" 412
	static void err_NoTopLevel(const std::string& arg); // "<mask> :No toplevel domain specified" 413
	static void err_WildTopLevel(const std::string& arg); // "<mask> :Wildcard in toplevel domain" 414
	static void err_UnknownCommand(const std::string& arg); // "<command> :Unknown command" 421
	static void err_NoMOTD(); // ":MOTD File is missing" 422
	static void err_NoAdminInfo(const std::string& arg); // "<server> :No administrative info available" 423
	static void err_FileError(const std::string& arg, const std::string& arg2); // ":File error doing <file op> on <file>" 424
	static void err_NoNickNameGiven(); //  ":No nickname given" 431
	static void err_ErroneusNuckname(const std::string& arg); //  "<nick> :Erroneus nickname" 432
	static void err_NicknameInUse(const std::string& arg); //  "<nick> :Nickname is already in use" 433
	static void err_NickCollision(const std::string& arg); //  "<nick> :Nickname collision KILL" 434
	static void err_UserNotInChannel(const std::string& arg, const std::string& arg2); //  "<nick> <channel> :They aren't on that channel" 441
	static void err_NotOnChannel(const std::string& arg); //  ERR_NOTONCHANNEL"<channel> :You're not on that channel" 442
	static void err_UserOnChannel(const std::string& arg, const std::string& arg2); //  "<user> <channel> :is already on channel" 443
	static void err_NoLogin(const std::string& arg); //   "<user> :User not logged in" 444
	static void err_SummonDisabled(); //  ":SUMMON has been disabled" 445
	static void err_UserDisabled(); //  ":USERS has been disabled"  446
	static void err_NotRegistered(); //  ":You have not registered" 451
	static void err_NeedMoreParams(const std::string& arg); //  "<command> :Not enough parameters" 461
	static void err_AlreadyRegistered(); //  ":You may not reregister" 462
	static void err_NoPermForHost(); //  ":Your host isn't among the privileged" 463
	static void err_PasswdMismatch(); //  ":Password incorrect" 464
	static void err_YoureBannedCreep(); //  ":You are banned from this server" 465
	static void err_KeySet(const std::string& arg); //  "<channel> :Channel key already set" 467
	static void err_ChannelIsFull(const std::string& arg); //  "<channel> :Cannot join channel (+l)" 471
	static void err_UnknownMode(const std::string& arg); //  "<char> :is unknown mode char to me" 472
	static void err_InviteOnlyChan(const std::string& arg); //  "<channel> :Cannot join channel (+i)" 473
	static void err_BannedFromChan(const std::string& arg); //  "<channel> :Cannot join channel (+b)" 474
	static void err_BadChannelKey(const std::string& arg); //  "<channel> :Cannot join channel (+k)" 475
	static void err_NoPrivileges(); //  ":Permission Denied- You're not an IRC operator" 481
	static void err_ChaNoPrivsNeeded(const std::string& arg); //  "<channel> :You're not channel operator" 482
	static void err_CantKillServer(); //  ":You cant kill a server!" 483
	static void err_NoOPerHost(); // ":No O-lines for your host" 491
	static void err_UModeUnknownFlag(); // ":Unknown MODE flag" 501
	static void err_UserDontMatch(); //  ":Cant change mode for other users" 502

	//        300     RPL_NONE
	//                        Dummy reply number. Not used.
	//        302     RPL_USERHOST
	//                        ":[<reply>{<space><reply>}]"
	//        303     RPL_ISON
	//                        ":[<nick> {<space><nick>}]"
	//        301     RPL_AWAY
	//                        "<nick> :<away message>"
	//        305     RPL_UNAWAY
	//                        ":You are no longer marked as being away"
	//        306     RPL_NOWAWAY
	//                        ":You have been marked as being away"
	//        311     RPL_WHOISUSER
	//                        "<nick> <user> <host> * :<real name>"
	//        312     RPL_WHOISSERVER
	//                        "<nick> <server> :<server info>"
	//        313     RPL_WHOISOPERATOR
	//                        "<nick> :is an IRC operator"
	//        317     RPL_WHOISIDLE
	//                        "<nick> <integer> :seconds idle"
	//        318     RPL_ENDOFWHOIS
	//                        "<nick> :End of /WHOIS list"
	//        319     RPL_WHOISCHANNELS
	//                        "<nick> :{[@|+]<channel><space>}"
	//        314     RPL_WHOWASUSER
	//                        "<nick> <user> <host> * :<real name>"
	//        369     RPL_ENDOFWHOWAS
	//                        "<nick> :End of WHOWAS"
	//        321     RPL_LISTSTART
	//                        "Channel :Users  Name"
	//        322     RPL_LIST
	//                        "<channel> <# visible> :<topic>"
	//        323     RPL_LISTEND
	//                        ":End of /LIST"
	//        324     RPL_CHANNELMODEIS
	//                        "<channel> <mode> <mode params>"
	//        331     RPL_NOTOPIC
	//                        "<channel> :No topic is set"
	//        332     RPL_TOPIC
	//                        "<channel> :<topic>"
	//        341     RPL_INVITING
	//                        "<channel> <nick>"
	//        342     RPL_SUMMONING
	//                        "<user> :Summoning user to IRC"
	//        351     RPL_VERSION
	//                        "<version>.<debuglevel> <server> :<comments>"
	//        352     RPL_WHOREPLY
	//                        "<channel> <user> <host> <server> <nick> \
	//                         <H|G>[*][@|+] :<hopcount> <real name>"
	//        315     RPL_ENDOFWHO
	//                        "<name> :End of /WHO list"
	//        353     RPL_NAMREPLY
	//                        "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	//        366     RPL_ENDOFNAMES
	//                        "<channel> :End of /NAMES list"
	//        364     RPL_LINKS
	//                        "<mask> <server> :<hopcount> <server info>"
	//        365     RPL_ENDOFLINKS
	//                        "<mask> :End of /LINKS list"
	//        367     RPL_BANLIST
	//                        "<channel> <banid>"
	//        368     RPL_ENDOFBANLIST
	//                        "<channel> :End of channel ban list"
	//        371     RPL_INFO
	//                        ":<string>"
	//        374     RPL_ENDOFINFO
	//                        ":End of /INFO list"
	//        375     RPL_MOTDSTART
	//                        ":- <server> Message of the day - "
	//        372     RPL_MOTD
	//                        ":- <text>"
	//        376     RPL_ENDOFMOTD
	//                        ":End of /MOTD command"
	//        381     RPL_YOUREOPER
	//                        ":You are now an IRC operator"
	//        382     RPL_REHASHING
	//                        "<config file> :Rehashing"
	//        391     RPL_TIME
	//                        "<server> :<string showing server's local time>"
	//        392     RPL_USERSSTART
	//                        ":UserID   Terminal  Host"
	//        393     RPL_USERS
	//                        ":%-8s %-9s %-8s"
	//        394     RPL_ENDOFUSERS
	//                        ":End of users"
	//        395     RPL_NOUSERS
	//                        ":Nobody logged in"
	//        200     RPL_TRACELINK
	//                        "Link <version & debug level> <destination> \
	//                         <next server>"
	//        201     RPL_TRACECONNECTING
	//                        "Try. <class> <server>"
	//        202     RPL_TRACEHANDSHAKE
	//                        "H.S. <class> <server>"
	//        203     RPL_TRACEUNKNOWN
	//                        "???? <class> [<client IP address in dot form>]"
	//        204     RPL_TRACEOPERATOR
	//                        "Oper <class> <nick>"
	//        205     RPL_TRACEUSER
	//                        "User <class> <nick>"
	//        206     RPL_TRACESERVER
	//                        "Serv <class> <int>S <int>C <server> \
	//                         <nick!user|*!*>@<host|server>"
	//        208     RPL_TRACENEWTYPE
	//                        "<newtype> 0 <client name>"
	//        261     RPL_TRACELOG
	//                        "File <logfile> <debug level>"
	//        211     RPL_STATSLINKINFO
	//                        "<linkname> <sendq> <sent messages> \
	//                         <sent bytes> <received messages> \
	//                         <received bytes> <time open>"
	//        212     RPL_STATSCOMMANDS
	//                        "<command> <count>"
	//        213     RPL_STATSCLINE
	//                        "C <host> * <name> <port> <class>"
	//        214     RPL_STATSNLINE
	//                        "N <host> * <name> <port> <class>"
	//        215     RPL_STATSILINE
	//                        "I <host> * <host> <port> <class>"
	//        216     RPL_STATSKLINE
	//                        "K <host> * <username> <port> <class>"
	//        218     RPL_STATSYLINE
	//                        "Y <class> <ping frequency> <connect \
	//                         frequency> <max sendq>"
	//        219     RPL_ENDOFSTATS
	//                        "<stats letter> :End of /STATS report"
	//        241     RPL_STATSLLINE
	//                        "L <hostmask> * <servername> <maxdepth>"
	//        242     RPL_STATSUPTIME
	//                        ":Server Up %d days %d:%02d:%02d"
	//        243     RPL_STATSOLINE
	//                        "O <hostmask> * <name>"
	//        244     RPL_STATSHLINE
	//                        "H <hostmask> * <servername>"
	//        221     RPL_UMODEIS
	//                        "<user mode string>"
	//        251     RPL_LUSERCLIENT
	//                        ":There are <integer> users and <integer> \
	//                         invisible on <integer> servers"
	//        252     RPL_LUSEROP
	//                        "<integer> :operator(s) online"
	//        253     RPL_LUSERUNKNOWN
	//                        "<integer> :unknown connection(s)"
	//        254     RPL_LUSERCHANNELS
	//                        "<integer> :channels formed"
	//        255     RPL_LUSERME
	//                        ":I have <integer> clients and <integer> \
	//                          servers"
	//        256     RPL_ADMINME
	//                        "<server> :Administrative info"
	//        257     RPL_ADMINLOC1
	//                        ":<admin info>"
	//        258     RPL_ADMINLOC2
	//                        ":<admin info>"
	//        259     RPL_ADMINEMAIL
	//                        ":<admin info>"
};

#endif
