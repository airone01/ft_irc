#include "IRCMessage.hpp"

static std::string trimCRLF(const std::string& line) {
	if (!line.empty() && line[line.size() - 1] == '\n')
		line.erase(line.size() - 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (line);
}

static std::string extractPrefix(const std::string& line, size_t& pos) {
	std::string newLine = line;
	if (line[0] == ':') {
		pos = line.find(" ");
		if (pos == std::string::npos)
			return (NULL);
		std::string prefix = line.substr(1, pos - 1);
		newLine = line.substr(pos + 1);
	}
	return (newLine);
}

static std::string extractCommand(const std::string& line, size_t& pos) {
	std::string newLine = line;
	pos = newLine.find(" ");
	if (pos == std::string::npos)
		return (NULL);
	newLine = line.substr(0, pos - 1);
	return (newLine);
}

int main() {
	size_t pos = 0;
	std::string line = ":Angel PRIVMSG Wiz :Hello are you receiving this message ?\r\n";
	std::string cleanLine = trimCRLF(line);
	if (cleanLine.empty())
		return 1;
	std::string prefix = extractPrefix(cleanLine, pos);
	std::string command = extractCommand(cleanLine, pos);
}

IRCMessage::IRCMessage(const std::string& line) {
	std::string cleanLine = trimCRLF(line);
	if (cleanLine.empty())
		throw MsgEmptyException();
	size_t pos = 0;
	_prefix = extractPrefix(cleanLine, pos);
	_trailing = extractCommand(cleanLine, pos);

	size_t trailingPos = cleanLine.find(" :");
	std::string beforeTrailing;
	if (trailingPos != std::string::npos) {
		beforeTrailing = cleanLine.substr(0, trailingPos);
		_trailing = cleanLine.substr(trailingPos + 2);
	} else
		beforeTrailing = cleanLine;

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

