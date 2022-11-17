/*Constructeur && Destructeur*/

//TODO add client message

#include "../includes/include.hpp"

static bool	is_running=true;


Server::Server(int port, std::string password):_port(port), _hostname(HOSTNAME), _passwordHash(hasher(password.c_str()))
{
	int			optval = 1;

	memset(&this->_ev, 0, sizeof(epoll_event));
	memset(&this->_ep_event, 0, sizeof(epoll_event) * MAX_EVENTS);
	memset(&this->_addr, 0, sizeof(sockaddr_in));
	this->createCmdDict();
	//INFO Creation of the master socket
	if ((this->_listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	//INFO Set socket to allow multi connections && reuse of socket
	if (setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		throw serverError("setsockopt", strerror(errno));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	//INFO binding the socket to the port
	if (bind(this->_listenSocket,(const struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1)
	{
		if (close(this->_listenSocket) == -1)
			throw serverError("close fd", strerror(errno));
		std::cerr << std::strerror(errno) << std::endl;
	}
	if (listen(this->_listenSocket, BACKLOG) == -1) //TODO move to start?
		throw serverError("listen", strerror(errno));
}

void	Server::_createPoll(void)
{
	if ((this->_pollfd = epoll_create1(0)) == -1)
		throw serverError("epoll_create1", strerror(errno));
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_listenSocket;
	if (epoll_ctl(this->_pollfd, EPOLL_CTL_ADD, this->_listenSocket, &this->_ev) == -1)
	{
		close(this->_pollfd);
		close(this->_listenSocket);
		throw serverError("epoll_ctl", strerror(errno));
	}
}

void	Server::_acceptNewClient(int listenSocket, int pollfd)
{
	socklen_t				addrlen;
	struct sockaddr_storage client_addr;
	int						client_fd;

	addrlen = sizeof(struct sockaddr_storage);
	memset(&client_addr, 0, sizeof(sockaddr_storage));
	if ((client_fd = accept(listenSocket, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen)) == - 1)
		std::cerr << std::strerror(errno) << std::endl; //QUID throw an exception or just a error?!
	this->_clientsOnServer[client_fd] = new Client(client_fd, this->getHostname()); //TODO check hostname
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = client_fd;
	if (epoll_ctl(pollfd, EPOLL_CTL_ADD, client_fd, &this->_ev) == -1)
		throw serverError("epoll_ctl", strerror(errno));
	//this->printAllUsersFd();
	std::cout << "nouvelle connexion" << std::endl;
}

void	Server::_handleMessage(int i)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	numbytes;

	memset(buffer, 0, BUFFER_SIZE);
	//est ce que je dois utiliser le fd de la struct event ou user??
	numbytes = recv(this->_ep_event[i].data.fd, buffer, BUFFER_SIZE, 0);
	if (numbytes == -1)
		std::cerr << "recv error" << std::endl;
	else if (numbytes == 0) //INFO client close connection
	{
		Client* userToDel = this->getClientByFd(this->_ep_event[i].data.fd);
		std::cerr << "Socket closed by client" << std::endl; //TODO delete before set as finish
		if (epoll_ctl(this->_pollfd, EPOLL_CTL_DEL, this->_ep_event[i].data.fd, &this->_ev) == -1)
			throw serverError("epoll_ctl", strerror(errno));
		if (close(this->_ep_event[i].data.fd) == -1)
			throw serverError("close", strerror(errno));
		this->deleteClient(userToDel);
	}
	else
	{
		Client* currentClient = this->getClientByFd(this->_ep_event[i].data.fd);
		this->executeCommands(buffer, currentClient);
	}

}

void	stopServer(int signal)
{
	(void)signal;
	std::cout << "\r";
	is_running = false;
}

void	Server::executeCommands(char *buffer, Client *client)
{
	std::string							toSplit(buffer);
	std::vector<std::string>			listOfCommands = splitCmd(toSplit, "\r\n");
	std::vector<Commands>				commandsList = manageMultipleCommands(listOfCommands);
	std::vector<Commands>::iterator		it = commandsList.begin();
	std::string							cmdFail;

	transformCmdsToUpper(&commandsList);
	if (!checkCmdLength(listOfCommands))
		return ;
	for (size_t i = 0; i < commandsList.size(); i++)
	{
		if (!isClientFullyRegister(client) && !isRegistrationCmd(it->command))
			return this->sendMsg(ERR_NOTREGISTERED, client->getFd()); //451
		this->_cmdDict[it->command](this, client->getFd(), *it);
		if (i == commandsList.size())
			break;
		it++;
	};
	//TODO
	//Check length of command
	//Check nb of arg ??!!
	//Check registration cmd
	//check is fully register
#ifdef DEBUG
	printAllCmds(commandsList);
#endif
}

void	Server::execute(void)
{
	int	nfds = 0;
	this->_createPoll();
	is_running = true;
	signal(SIGINT, stopServer);
	while (is_running)
	{
		if ((nfds = epoll_wait(this->_pollfd, this->_ep_event, MAX_EVENTS, -1)) == -1) //TODO define last arg as TIME OUT //INFO with a value of -1 it's going to wait indefinitly
			std::cerr << "QUID MESSAGE OU NON" << std::endl;//this->clearServer ??
		for (int i = 0; i < nfds; i++)
		{
			if ((this->_ep_event[i].events & EPOLLIN) == EPOLLIN)
			{
				if (this->_ep_event[i].data.fd == this->_listenSocket)
					this->_acceptNewClient(this->_listenSocket, this->_pollfd);
				else
					this->_handleMessage(i);
			}
		}
	}
	signal(SIGINT, SIG_DFL);
}


void	Server::clearServer(void) //TODO link with signal??!!
{
	//TODO delete channels before user bc channels are links to users
	//Close connection and fd and delete users
	if (!this->_clientsOnServer.empty())
	{
		std::map<int, Client*>::const_iterator it_end = this->_clientsOnServer.end();
		for (std::map<int, Client*>::const_iterator it_begin = this->_clientsOnServer.begin(); it_begin != it_end; it_begin++)
		{
			try
			{
				if (epoll_ctl(this->_pollfd, EPOLL_CTL_DEL, it_begin->second->getFd(), &this->_ev) == -1)
					throw serverError("epoll_ctl_del", strerror(errno));
				if (it_begin->second)
				{
					if (close(it_begin->second->getFd()) == -1)
						throw serverError("close", strerror(errno));
				}
				delete it_begin->second;
			}
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	if (close(this->_listenSocket) == -1)
		std::cerr << "close issue" << std::endl;
	if (close(this->_pollfd) == -1)
		std::cerr << "close issue" << std::endl;
}


const std::string&		Server::getHostname(void)const
{
	return (this->_hostname);
}

const int&				Server::getListenSocket(void)const
{
	return (this->_listenSocket);
}


const std::map<std::string, time_t>&	Server::getNicknameUnavailable(void)const
{
	return (this->_nicknameUnavailable);
}

void				Server::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

void				Server::addNicknameUnavailable(std::string nick, time_t time)
{
	this->_nicknameUnavailable[nick] = time;
}

std::vector<Client*>	Server::getAllClients(void)const
{
	std::vector<Client*> allClients;

	for (std::map<int, Client*>::const_iterator it = this->_clientsOnServer.begin(); it!= this->_clientsOnServer.end(); it++)
	{
		allClients.push_back(it->second);
	}

	return (allClients);
}

std::vector<Client*>	Server::getAllClientsMatching(std::string pattern) const
{
	std::vector<Client*> allClients;

	for (std::map<int, Client*>::const_iterator it = this->_clientsOnServer.begin(); it!= this->_clientsOnServer.end(); it++)
	{
		if (strmatch(it->second->getNickname(), pattern)
			|| strmatch(it->second->getHostname(), pattern)
			|| strmatch(it->second->getUsername(), pattern))
			allClients.push_back(it->second);
	}

	return (allClients);
}

void	Server::printAllUsersFd(void)
{
	if (!this->_clientsOnServer.empty())
	{
		std::map<int, Client*>::const_iterator it;
		std::map<int, Client*>::const_iterator ite = this->_clientsOnServer.end();
		for (it = this->_clientsOnServer.begin(); it != ite; it++)
		{
			std::cout << (it->second)->getHostname() << std::endl;
		}
	}

}

void	Server::deleteClient(Client* user)
{
	std::map<int, Client*>::iterator it;
	std::map<int, Client*>::iterator ite = this->_clientsOnServer.end();
	for (it = this->_clientsOnServer.begin(); it != ite;)
	{
		if (user == it->second)
		{
			delete it->second;
			//close (it->second->getFd());
			this->_clientsOnServer.erase(it++);
		}
		else
			it++;
	}
}

void	Server::sendMsg(const std::string msg, const int fd)
{
#ifdef DEBUG
	std::cout << msg << std::endl;
#endif
	send(fd, msg.c_str(), msg.length(), MSG_DONTWAIT);
}

void	Server::sendMsg(const std::string msg, Client *client)
{
	if (client)
		sendMsg(msg, client->getFd());
}

void	Server::createCmdDict(void) {
	_cmdDict["PASS"] = &pass;
	_cmdDict["NICK"] = &nick;
	_cmdDict["USER"] = &user;
	_cmdDict["CAP"] = &cap;
	// _cmdDict["OPER"] = &oper;
	// _cmdDict["MODE"] = &mode;
	// _cmdDict["SERVICE"] = &service;
	// _cmdDict["QUIT"] = &quit;
	// _cmdDict["SQUIT"] = &squit;
	_cmdDict["JOIN"] = &join;
	// _cmdDict["PART"] = &part;
	// _cmdDict["TOPIC"] = &topic;
	// _cmdDict["NAMES"] = &names;
	// _cmdDict["LIST"] = &list;
	// _cmdDict["INVITE"] = &invite;
	// _cmdDict["KICK"] = &kick;
	// _cmdDict["PRIVMSG"] = &privmsg;
	_cmdDict["NOTICE"] = &notice;
	// _cmdDict["MOTD"] = &motd;
	// _cmdDict["LUSERS"] = &lusers;
	// _cmdDict["VERSION"] = &version;
	// _cmdDict["STATS"] = &stats;
	// _cmdDict["LINKS"] = &links;
	 _cmdDict["TIME"] = &time;
	// _cmdDict["CONNECT"] = &connect;
	// _cmdDict["TRACE"] = &trace;
	// _cmdDict["ADMIN"] = &admin;
	// _cmdDict["INFO"] = &info;
	// _cmdDict["SERVLIST"] = &servlist;
	// _cmdDict["SQUERY"] = &squery;
	// _cmdDict["WHO"] = &who;
	// _cmdDict["WHOIS"] = &whois;
	// _cmdDict["WHOWAS"] = &whowas;
	// _cmdDict["KILL"] = &kill;
	// _cmdDict["PING"] = &ping;
	// _cmdDict["PONG"] = &pong;
	// _cmdDict["ERROR"] = &error;
	// _cmdDict["AWAY"] = &away;
	// _cmdDict["REHASH"] = &rehash;
	// _cmdDict["DIE"] = &die;
	// _cmdDict["RESTART"] = &restart;
	// _cmdDict["SUMMON"] = &summon;
	// _cmdDict["USERS"] = &users;
	// _cmdDict["WALLOPS"] = &wallops;
	// _cmdDict["USERHOST"] = &userhost;
	// _cmdDict["ISON"] = &ison;
}

Client* Server::getClientByNickname(const std::string nickname) const
{
	std::map<const int, Client*>::const_iterator currentUser;
	for (currentUser = _clientsOnServer.begin(); currentUser != _clientsOnServer.end(); currentUser++)
	{
		if (currentUser->second->getNickname() == nickname)
			return currentUser->second;
	}
	return NULL;
}

Client* Server::getClientByFd(size_t fd)
{
	return _clientsOnServer[fd];
}

void	Server::sendMsg(NumericReplies reply, const int fd)
{
	std::string msg = ":" + getHostname() + " " + toString(reply.num) + " " + getClientByFd(fd)->getNickname() + " " + reply.msg;
#ifdef DEBUG
	std::cout << "value of end split " << std::endl;
#endif
	sendMsg(msg, fd);
}

bool	Server::checkPassword(const std::string password) const
{
	return hasher(password.c_str()) == _passwordHash;
}

Channel* Server::getChannelByName(const std::string name)
{
	return _channels[name];
}

void Server::addChannel(std::string name, Client* creator)
{
	Channel *newChannel = new Channel(this, name, creator);
	_channels[name] = newChannel;
}

std::string		Server::getMessageOfTheDay(void)
{
	return _messageOfTheDay;
}

std::map<std::string, Channel*> Server::getChannels(void)
{
	return _channels;
}

void Server::changeNicknameAsKeysInChannels(std::string oldNickname, std::string newNickname)
{
	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.end(); it != _channels.begin(); it++)
	{
		Channel* channel = it->second;
		channel->changeNickname(oldNickname, newNickname);
	}
}

/**
 * @brief Helper function to check wether the channel name is correct and the
 * client can create/join it
 */
void Server::checkAndJoinChannel(int socket, std::string channelName, std::string key)
{
	if (!checkChannelName(channelName))
		return sendMsg(ERR_BADCHANMASK(channelName), socket);
	Channel *channel = getChannelByName(channelName);
	Client *client = getClientByFd(socket);
	if (!channel)
		return addChannel(channelName, client);
	channel->addClient(socket);
	channel->sendJoin(client->getHostname());
	if (!channel->getTopic().empty())
		channel->sendTopic(socket);
	channel->sendListOfNames(socket);
}

/**
 * @brief Helper function to check wether the channel name is correct and the
 * client can leave it
 */
void	Server::checkAndLeaveChannel(int socket, std::string channelName)
{
	Channel *channel = getChannelByName(channelName);
	if (!channel)
		return sendMsg(ERR_NOSUCHCHANNEL(channelName), socket);
	Client *client = getClientByFd(socket);
	if (!channel->findClientByNickname(client->getNickname()))
		return sendMsg(ERR_NOTONCHANNEL(channelName), socket);
	channel->deleteClient(client->getNickname());
}

/*
** @Brief display the localtime of the server
**		  https://cplusplus.com/reference/ctime/localtime/
*/

void	Server::printCurrentLocaltime(int socket)
{
	time_t		rawtime;
	struct tm*	timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);
	std::string localtime = (std::string)"Current localtime of the current server " + (std::string)asctime(timeinfo);
	this->sendMsg(localtime , socket);
}
