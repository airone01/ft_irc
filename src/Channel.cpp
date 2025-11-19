#include "Channel.hpp"


Channel::Channel( void ){}
Channel::Channel(const Channel &tmp ){}
Channel &Channel::operator=(const Channel &tmp ) {return *this;}
Channel::~Channel( void ){}

Channel::Channel( Client &tmp)
{
	this->_users.insert(std::pair<int, Client*>(tmp.getSocket(), &tmp));
	this->_maxCapacity = -1;
}

Channel::Channel( Client &tmp, int capacity ) :
	_maxCapacity(capacity)
{
	this->_users.insert(std::pair<int, Client*>(tmp.getSocket(), &tmp));
	this->_maxCapacity = capacity;
}

std::string Channel::getTopic() const{
    return this->_topic;
}

std::string Channel::getPswrd() const{
    return this->_pswrd;
}

std::string Channel::getName() const{
    return this->_name;
}

int Channel::getCapacity() const{
    return this->_maxCapacity;
}

bool Channel::getModeSet() const{
    return this->_modeSet;
}

std::set<char> Channel::getMode() const{
	return this->_mode;
}

std::set<int> Channel::getKickedUsers() const{
	return this->_kickedUsers;
}


std::map<int, Client*> Channel::getInvitedUsers() { return this->_invitedUsers; }

void    Channel::setTopic( std::string newTopic ){
    this->_topic = newTopic;
}

void	validChannelName(std::string tmp){
	size_t space = tmp.find(' ');
	size_t comma = tmp.find(',');
	size_t ctrlG = tmp.find(7);
	size_t size = tmp.size();
	if (size > 200 || space != size || comma != size || ctrlG != size)
			throw Channel::invalidChannelName();
	if (tmp[0] == '#' || tmp[0] == '&')
		;
	else
		throw Channel::invalidChannelName();
}	

void    Channel::setName( std::string newName ){
	try
	{
		validChannelName(newName);
		this->_name = newName;
	}
	catch (const invalidChannelName e)
	{
		std::cerr << &e << std::endl;
	}
}

void    Channel::setPswrd( std::string newPswrd ){
    this->_pswrd = newPswrd;
}

void    Channel::setCapacity( int newCapacity ){
    this->_maxCapacity = newCapacity;
}

void    Channel::setModeSet( bool changeMode ){
    this->_modeSet = changeMode;
}

void    Channel::setMode(const char c){
	this->_mode.insert(c);
}

void    Channel::setKickedUsers(const int socket){
	this->_kickedUsers.insert(socket);
}


void Channel::newUser( Client &tmp){
	_users.insert(std::pair<int, Client*>(tmp.getSocket(), &tmp));
}

void Channel::updatePriv(const Client &admin, Client &user){
	if (_admins.find(admin.getSocket()) == _admins.end())
		throw insufficientPrivilege();
	_admins.insert(std::pair<int, Client*>(user.getSocket(), &user));
}

void Channel::tryJoin( const Client &tmp, std::string pswrd){
	if (_modeSet ){
		if (_mode.find('i') != _mode.end()){
			if (_invitedUsers.find(tmp.getSocket()) == _invitedUsers.end())
				throw errorMode("ERR_INVITEONLYCHAN");
		}
		if (_mode.find('l') != _mode.end())
			if ((_users.size() == _maxCapacity))
				throw errorMode("ERR_CHANNELISFULL");
		if (_mode.find('k') != _mode.end()){
			if (pswrd.empty())
				throw errorMode("ERR_NEEDMOREPARAMS");
			else if ((pswrd != _pswrd))
				throw errorMode("ERR_BADCHANNELKEY");
		}
	}
}

const char *Channel::maxCapacityReached::what() const throw(){
	return "error: max capacity for this channel already reached.";
}

const char *Channel::invitationNeeded::what() const throw(){
	return "error: you need a invitation to join this channel.";
}

const char *Channel::insufficientPrivilege::what() const throw(){
	return "error: unsufficient privilege to do this action.";
}

const char *Channel::invalidChannelName::what() const throw(){
	return "error: Channel name must begin with '&' or '#'.";
}

const char *Channel::errorMode::what() const throw(){
	return _errMsg.c_str();
}