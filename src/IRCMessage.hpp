#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <exception>


/**
 *	@Brief This is the parsing of the messages sent.
 *	Format			:	[ ':' <prefix> <SPACE> ] <command> <SPACE> <params>[ ',' <params> ] <SPACE> ':' <trailing>
 *	Numeric Replies	:
 *						ERR_NORECIPIENT			ERR_NOTEXTTOSEND
 *						ERR_CANNOTSENDTOCHAN	ERR_NOTOPLEVEL
 *						ERR_WILDTOPLEVEL		ERR_TOOMANYTARGETS
 *						ERR_NOSUCHNICK			RPL_AWAY
 *	Exemple			:	:Angel PRIVMSG Wiz :Hello are you receiving this message ?;
 *						PRIVMSG Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br;
 *						PRIVMSG jtotolsun.oulu.fi :Hello !;
 *						PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.; Message to everyone on a server which has a name matching *.fi.
 *						PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions; Message to all users who come from a host which has a name matching *.edu.
 */
class IRCMessage {
private:
	std::string	_prefix;
	std::string	_command;
	std::vector<std::string> _params;
	std::string	_trailing;
	// setter
	void	setPrefix(const std::string& prefix);
	void	setCommand(const std::string& command);
	void	setParams(const std::vector<std::string>& params);
	void	setTrailing(const std::string& trailing);
	IRCMessage();
	IRCMessage(const IRCMessage& copy);
	IRCMessage& operator=(const IRCMessage& other);
public:
	IRCMessage(std::string& line);
	~IRCMessage();
	// exception
	class MsgEmptyException: public std::exception {
		virtual const char* what() const throw();
	};
	// getter
	const std::string&				getPrefix() const;
	const std::string&				getCommand() const;
	const std::vector<std::string>&	getParams() const;
	const std::string&				getTrailing() const;
	// methods
	size_t	getCountParams() const;
};

#endif
