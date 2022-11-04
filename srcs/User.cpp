//TODO define mode and status and add to constructor

User::User(int fd, std::string hostname): _nickname(""), _username(""), _fd(fd), _hostname(hostname), _mode(), _status()
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

const int	User::getFd(void)const
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

void	User::setUsername(void)
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
