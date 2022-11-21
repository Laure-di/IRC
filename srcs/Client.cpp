//TODO define mode and status and add to constructor
//TODO change mode and status in consructor
#include "../includes/include.hpp"

Client::Client(const int fd, std::string hostname): _nickname(""), _username(""), _fullName(""), _fd(fd), _hostname(hostname), _mode(NONE), _status(NONE), _pwd(false)
{
}

Client::~Client()
{}

std::string	Client::getNickname(void)const
{
	return (this->_nickname);
}

std::string	Client::getPrefix(void)const
{
	if (getMode() & CREATOR)
		return "~";
	if (getMode() & OPERATOR)
		return "@";
	if (getMode() & VOICE)
		return "+";
	return ("");
}

std::string	Client::getNicknameWithPrefix(Channel *channel)const
{

	return channel->getClientPrefix(this) + getNickname();
}

std::string	Client::getUsername(void)const
{
	return (this->_username);
}

int	Client::getFd(void)const
{
	return (this->_fd);
}

std::string	Client::getHostname(void)const
{
	return (this->_hostname);
}

std::string Client::getFullIdentifier(void) const
{
	return (getNickname() + "!" + getUsername() + "@" + getHostname());
}

int		Client::getStatus(void)const
{
	return (this->_status);
}

int		Client::getMode(void)const
{
	return (this->_mode);
}

bool Client::getPwd(void)const
{
	return (this->_pwd);
}


Client*	Client::getLastExpediteur(void)const
{
	return (this->_lastExpediteur);
}

Client*	Client::getLastRecipient(void)const
{
	return (this->_lastRecipient);
}

std::vector<Channel*>	Client::getAllChannels(void)const
{
	std::vector<Channel*>							list;
	std::map<std::string, Channel*>::const_iterator	it;
	for (it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		list.push_back(it->second);
	}
	return (list);
}

Channel*	Client::getActiveChannel(void)const
{
	return (this->_activeChannel);
}

std::string		Client::getBuffer(void)const
{
	return _buffer;
}

void	Client::setNickname(std::string nickname)
{
	//TODO faire les check nickname
	this->_nickname = nickname;
}

void	Client::setUsername(std::string username)
{
	this->_username = username;
}

void	Client::setMode(unsigned mode)
{
	this->_mode = mode;
}

void	Client::setStatus(int status)
{
	this->_status = status;
}

void	Client::setFullName(std::string fullName)
{
	this->_fullName = fullName;
}

void	Client::setPwd(bool pwd)
{
	this->_pwd = pwd;
}

void	Client::addMode(unsigned mask)
{
	_mode |= mask;
}

void	Client::remMode(unsigned mask)
{
	_mode &= ~mask;
}

void	Client::modMode(unsigned mask, bool add)
{
	if (add)
		return addMode(mask);
	remMode(mask);
}

void	Client::setLastExpediteur(Client* expediteur)
{
	this->_lastExpediteur = expediteur;
}

void	Client::setLastRecipient(Client* recipient)
{
	this->_lastRecipient = recipient;
}

void	Client::setActiveChannel(Channel* active)
{
	this->_activeChannel = active;
}

void	Client::append(std::string buffer)
{
	_buffer += buffer;
}

void	Client::clearBuffer(void)
{
	_buffer.clear();
}

std::string		Client::getModeStr(void) const
{
	std::string res = "+";
	if (_mode & AWAY)
		res += "a";
	if (_mode & INVISIBLE)
		res += "i";
	if (_mode & RESTRICTED)
		res += "r";
	if (_mode & SERVEROPERATOR)
		res += "o";
	return res;
}
