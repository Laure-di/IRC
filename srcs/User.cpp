//TODO define mode and status and add to constructor
//TODO change mode and status in consructor
#include "../includes/Class/User.hpp"

User::User(const int fd, std::string hostname): _nickname(""), _username(""), _fullName(""), _fd(fd), _hostname(hostname), _mode(0), _status(0)
{
}

User::~User()
{}

std::string	User::getNickname(void)const
{
	return (this->_nickname);
}

std::string	User::getUsername(void)const
{
	return (this->_username);
}

int	User::getFd(void)const
{
	return (this->_fd);
}

std::string	User::getHostname(void)const
{
	return (this->_hostname);
}

int		User::getStatus(void)const
{
	return (this->_status);
}

int		User::getMode(void)const
{
	return (this->_mode);
}

void	User::setNickname(std::string nickname)
{
	//TODO faire les check nickname
	this->_nickname = nickname;
}

void	User::setUsername(std::string username)
{
	this->_username = username;
}

void	User::setMode(int mode)
{
	this->_mode = mode;
}

void	User::setStatus(int status)
{
	this->_status = status;
}

void	User::setFullName(std::string fullName)
{
	this->_fullName = fullName;
}
