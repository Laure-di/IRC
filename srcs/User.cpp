//TODO define mode and status and add to constructor
//TODO change mode and status in consructor
#include "../includes/Class/User.hpp"

Client::Client(const int fd, std::string hostname): _nickname(""), _username(""), _fullName(""), _fd(fd), _hostname(hostname), _mode(0), _status(0), _pwd(false)
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
