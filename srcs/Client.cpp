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

std::string	Client::getFullName(void)const
{
	return _fullName;
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

std::string		Client::getBuffer(void)const
{
	return _buffer;
}

std::string		Client::getAwayMessage(void)const
{
	return _awayMessage;
}

bool Client::isInSameChannel(Client *client) const
{
	std::string nickname = client->getNickname();
	std::map<std::string, Channel*>::const_iterator cit;
	for (cit = _channels.begin(); cit != _channels.end(); cit++)
	{
		if (cit->second->getClientByNickname(nickname))
			return true;
	}
	return false;
}

std::string			Client::getPresence(void) const
{
	if (_mode & AWAY)
		return "G";
	return "H";
}

NumericReplies			Client::getWhoMessage(void) const
{
	std::string channel;
	if (_channels.empty())
		channel = "*";
	else
		channel = _channels.begin()->second->getName();
	return RPL_WHOREPLY(channel, getUsername(), getHostname(), HOSTNAME, getNickname(), getPresence(), getFullName());
}

NumericReplies			Client::getWhoIsMessage(void) const
{
	return RPL_WHOISUSER(getNickname(), getUsername(), HOSTNAME, getFullName());
}

void	Client::setNickname(std::string nickname)
{
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

void	Client::setAwayMessage(std::string awayMessage)
{
	_awayMessage = awayMessage;
}

void	Client::addChannel(Channel* channel, std::string channelName)
{
	_channels.insert(std::make_pair(channelName, channel));
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
	if (_mode & WALLOPS)
		res += "w";
	return res;
}

void	Client::removeChannel(std::string channelName)
{
	if (_channels.find(channelName) != _channels.end())
		_channels.erase(channelName);
}

void	Client::removeFromAllChannels(void)
{
	std::map<std::string, Channel*>::const_iterator	it = _channels.begin();
	for(; it != _channels.end(); it++)
	{
		std::map<std::string, Client*>				clientsOnChannel = it->second->getClients();
		std::map<std::string, Client*>::iterator	ite = clientsOnChannel.end();
		if ((it->second->getClients()).find(_nickname) != ite)
			(it->second)->remClient(_nickname);
	}
}

