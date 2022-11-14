//TODO define mode and status and add to constructor
//TODO change mode and status in consructor
#include "../includes/include.hpp"

Client::Client(const int fd, std::string hostname): _nickname(""), _username(""), _fullName(""), _fd(fd), _hostname(hostname), _mode(None), _status(None), _pwd(false)
{
}

Client::~Client()
{}

std::string	Client::getNickname(void)const
{
	return (this->_nickname);
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

Channel*	Client::getActiveChannel(void)const
{
	return (this->_activeChannel);
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

void	Client::setMode(int mode)
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

void	Client::addMode(int mask)
{
	_mode |= mask;
}

void	Client::remMode(int mask)
{
	_mode &= ~mask;
}

void	Client::modMode(int mask, bool add)
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