#include "IRCMessage.hpp"

static std::string trimCRLF(std::string& line) {
	if (!line.empty() && line[line.size() - 1] == '\n')
		line.erase(line.size() - 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (line);
}

static std::string extractPrefix(std::string& line, size_t& pos) {
	if (line[0] == ':') {
		size_t spacePos = line.find(' ');
		if (spacePos == std::string::npos)
			return ("");
		std::string prefix = line.substr(1, spacePos - 1);
		line = line.substr(spacePos + 1);
		pos += spacePos;
		return (prefix);
	}
	pos = 0;
	return ("");
}

static std::string extractCommand(std::string& line, size_t& pos) {
	std::string newLine = line;
	pos = newLine.find(" ");
	if (pos == std::string::npos)
		return ("");
	newLine = line.substr(0, pos);
	return (newLine);
}

static std::string extractTrailing(const std::string& line, size_t& pos) {
	std::string newLine = line;
	pos = newLine.find(" :");
	if (pos == std::string::npos)
		return ("");
	newLine = line.substr(pos + 2);
	return (newLine);
}

static std::string extractParams(const std::string& line, size_t& pos) {
	std::string newLine = line;

}

IRCMessage::IRCMessage(const std::string& line) {
	std::string cleanLine = trimCRLF(line);
	if (cleanLine.empty())
		throw MsgEmptyException();
	size_t pos = 0;
	setPrefix(extractPrefix(cleanLine, pos));
	setCommand(extractCommand(cleanLine, pos));
	setTrailing(extractTrailing(cleanLine, pos));
	addParams(extractParams(cleanLine, pos));
}

IRCMessage::~IRCMessage() {
}

const char* IRCMessage::MsgEmptyException::what() const throw() {
	return ("The string cannot be empty");
}

const std::string&	IRCMessage::getPrefix() const {
	return (this->_prefix);
}

const std::string& IRCMessage::getCommand() const {
	return (this->_command);
}

const std::vector<std::string>& IRCMessage::getParams() const {
	return (this->_params);
}

const std::string&	IRCMessage::getTrailing() const {
	return (this->_trailing);
}

void	IRCMessage::setPrefix(const std::string& prefix) {
	this->_prefix = prefix;
}

void	IRCMessage::setCommand(const std::string& command) {
	this->_command = command;
}

void	IRCMessage::addParams(const std::string& params) {
	this->_params.push_back(params);
}

void	IRCMessage::setTrailing(const std::string& trailing) {
	this->_trailing = trailing;
}

size_t	IRCMessage::getCountParams() const {
	return (this->_params.size());
}

