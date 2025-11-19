#include <string>
#include <iostream>
#include <sstream>
#include <vector>

std::string trimCRLF(std::string& line) {
	if (!line.empty() && line[line.size() - 1] == '\n')
		line.erase(line.size() - 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (line);
}

std::string extractPrefix(std::string& line, size_t& pos) {
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

std::string extractCommand(std::string& line, size_t& pos) {
	std::string newLine = line;
	pos = newLine.find(" ");
	if (pos == std::string::npos)
		return ("");
	newLine = line.substr(0, pos);
	line = line.substr(pos + 1);
	return (newLine);
}

std::string extractTrailing(std::string& line, size_t& pos) {
	std::string newLine = line;
	pos = newLine.find(" :");
	if (pos == std::string::npos)
		return ("");
	newLine = line.substr(pos + 2);
	line = line.substr(0, pos);
	return (newLine);
}

std::vector<std::string> extractParams(const std::string& line) {
	std::vector<std::string> params;
	std::string word;
	for (size_t i = 0; i < line.length(); i++) {
		if (line[i] == ' ') {
			if (!word.empty()) {
				params.push_back(word);
				word.clear();
			}
		} else
			word += line[i];
	}
	if (!word.empty()) {
		params.push_back(word);
	}
	return params;
}

int main() {
	size_t pos = 0;
	std::string line = ":qqqq PRIVMSG Wiz,Biz,Fiz,Giz hello :Hello are you receiving this message ?\r\n";
	std::string cleanLine = trimCRLF(line);
	if (cleanLine.empty())
		return 1;
	std::cout << "Ligne de base: "<<cleanLine << std::endl;
	std::string prefix = extractPrefix(cleanLine, pos);
	std::string command = extractCommand(cleanLine, pos);
	std::string trailing = extractTrailing(cleanLine, pos);
	std::vector<std::string> params = extractParams(cleanLine);
	std::cout << "Prefix: " << prefix << "\nCommand: " << command << "\nTrailing: " << trailing << "\nPos: " << pos << std::endl;
	for (auto i : params) {
		std::cout << "Params: " << i << std::endl;
	}
	std::cout << line << std::endl;
}