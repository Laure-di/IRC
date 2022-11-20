#include "../includes/include.hpp"

#define DEBUG

static bool is_running=true;

/****************************************************************************/
/***					Constructor && Destructor						 ***/
/***																	***/
/*************************************************************************/

/**
 ** @Brief : Socket creation with a domain IPV4 (AF_IFNET), a communication type TCP (SOCK_STREAM), and protocol IP (0).
 **			Set socket option to allow multi-connection and the reuse of socket.
 **			Bind the socket to the address and port number (with info from getaddrinfo)
 **/

Server::Server(int port, std::string password): _port(port), _hostname(HOSTNAME), _version(VERSION), _passwordHash(hasher(password.c_str()))
{
	_port = port;
	this->createAndBind();
	memset(&_ep_event, 0, sizeof(epoll_event) * MAX_EVENTS);
	this->createCmdDict();
	if (listen(_listenSocket, BACKLOG) == -1)
		throw serverError("listen", strerror(errno));
	time_t      rawtime = time(NULL);
	struct tm   *info;
	info = localtime(&rawtime);
	_launchingDate = std::string(asctime(info));
}



/****************************************************************************/
/***					Getter && Setter								 ***/
/***																	***/
/*************************************************************************/

std::string								Server::getHostname(void)const
{
	return _hostname;
}

std::string								Server::getLaunchingDate(void)const
{
	return _launchingDate;
}

std::string								Server::getVersion(void)const
{
	return _version;
}


const std::map<std::string, time_t>&	Server::getNicknameUnavailable(void)const
{
	return (this->_nicknameUnavailable);
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

struct epoll_event		Server::getEventFd(Client *client)
{
	int	i = 0;
	struct epoll_event ep_event;
	while (i < MAX_EVENTS)
	{
		if (_ep_event[i].data.fd == client->getFd())
			return (ep_event = _ep_event[i]);
		i++;
	}
	return (ep_event);
}

Channel* Server::getChannelByName(const std::string name)
{
	return _channels[name];
}

std::map<std::string, Channel*> Server::getAllChannels(void)
{
	return _channels;
}

std::string		Server::getMessageOfTheDay(void)
{
	return _messageOftheDay;
}

/* SETTER */

Channel* Server::addChannel(std::string name, Client* user)
{
	Channel *newChannel = new Channel(this, name, user);
	_channels[name] = newChannel;
	return newChannel;
}

void Server::createNewChannel(int creator, std::string name)
{
	//Add creation of channel
	(void)creator;
	(void)name;
}


/****************************************************************************/
/***					Private Methods									 ***/
/***																	***/
/*************************************************************************/

/**
 ** Utils method
 **/

void	Server::sendMsg(NumericReplies reply, const int fd)
{
	std::string msg = ":" + _hostname + " " + toString(reply.num) + " " + getClientByFd(fd)->getNickname() + " " + reply.msg;
	sendMsg(msg, fd);
}

void	Server::sendMsg(const std::string msg, const int fd)
{
	send(fd, msg.c_str(), msg.length(), MSG_DONTWAIT);
}

void	Server::sendMsg(const std::string msg, Client *client)
{
	if (client)
		sendMsg(msg, client->getFd());
}

void	Server::sendMsg(const std::string msg, std::vector<Channel*> channels)
{
	std::vector<Channel*>::iterator	it;

	for (it = channels.begin(); it != channels.end(); it++)
	{
		(*it)->sendMsg(msg);
	}
}

bool	Server::checkPassword(const std::string password) const
{
	return hasher(password.c_str()) == _passwordHash;
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

void	Server::printCurrentLocaltime(int socket)
{
	time_t		rawtime;
	struct tm*	timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);
	std::string localtime = (std::string)"Current localtime of the current server " + (std::string)asctime(timeinfo);
	this->sendMsg(localtime , socket);
}


/**
 ** Execution Commands method
 **/

void	deleteAllCmds(std::vector<Commands> cmd)
{
	std::vector<Commands>::iterator	it;
	for (it = cmd.begin(); it != cmd.begin(); it++)
		delete &(*it);
}


void	Server::executeCommands(std::string buffer, Client *client)
{
	std::vector<std::string>			listOfCommands = splitCmd(buffer, "\r\n");
	std::vector<Commands>				commandsList = manageMultipleCommands(listOfCommands);
	std::vector<Commands>::iterator		it = commandsList.begin();
	std::string							cmdFail;

	transformCmdsToUpper(&commandsList);
	for (size_t i = 0; i < commandsList.size(); i++)
	{
		if (!isClientFullyRegister(client) && !isRegistrationCmd(commandsList[i].command))
			return this->sendMsg(ERR_NOTREGISTERED, client->getFd()); //451
		if (_cmdDict.find(commandsList[i].command) != _cmdDict.end())
			this->_cmdDict[commandsList[i].command](this, client->getFd(), commandsList[i]);
		else
			this->sendMsg(ERR_UNKNOWNCOMMAND(commandsList[i].command), client->getFd());
		if (i == commandsList.size())
			break;
		it++;
	};
	//TODO
	//Check nb of arg ??!!
#ifdef DEBUG
//	printAllCmds(commandsList);
#endif
}



/**
 ** Communication with server methods
 **/


/**
 ** @Brief https://support.sas.com/documentation/onlinedoc/ccompiler/doc750/html/lr2/zockname.htm
 **/

void	Server::_acceptNewClient(int listenSocket, int pollfd)
{
	socklen_t				addrlen;
	struct sockaddr_in		client_addr;
	int						client_fd;
	struct epoll_event		ev;

	addrlen = sizeof(sockaddr_in);
	memset(&client_addr, 0, sizeof(sockaddr_in));
	if ((client_fd = accept(listenSocket, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen)) == - 1)
		std::cerr << std::strerror(errno) << std::endl; //QUID throw an exception or just a error?!
	if (getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen) == -1)
		throw serverError("getsockname", strerror(errno));
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK))
		throw serverError("fctnl", strerror(errno));
	this->_clientsOnServer[client_fd] = new Client(client_fd, inet_ntoa(client_addr.sin_addr)); //TODO check hostname
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = client_fd;
	if (epoll_ctl(pollfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
		throw serverError("epoll_ctl", strerror(errno));
#ifdef DEBUG
	std::cout << "nouvelle connexion" << std::endl;
#endif
}


int		Server::_handleMessage(epoll_event ep_event)
{
	char				buffer[BUFFER_SIZE];
	ssize_t				numbytes;
	Client* currentClient = this->getClientByFd(ep_event.data.fd);

	memset(buffer, 0, BUFFER_SIZE);
	currentClient->clearBuffer();
	numbytes = recv(ep_event.data.fd, buffer, BUFFER_SIZE, 0);
	if (numbytes <= 0)
		return (-1);
	else
	{
		buffer[numbytes] = '\0';
		currentClient->append(buffer);
		this->executeCommands(currentClient->getBuffer(), currentClient);
	}
	return (1);
}


void	stopServer(int signal)
{
	(void)signal;
	std::cout << "\r";
	is_running = false;
}


void	Server::_createPoll(void)
{
	struct epoll_event	ev;

	if ((this->_pollFd = epoll_create1(0)) == -1)
		throw serverError("epoll_create1", strerror(errno));
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = this->_listenSocket;
	if (epoll_ctl(this->_pollFd, EPOLL_CTL_ADD, this->_listenSocket, &ev) == -1)
	{
		close(this->_pollFd);
		close(this->_listenSocket);
		throw serverError("epoll_ctl", strerror(errno));
	}
}

void	Server::execute(void)
{
	int	nfds = 0;
	this->_createPoll();
	is_running = true;
	signal(SIGINT, stopServer);
	while (is_running)
	{
		if ((nfds = epoll_wait(_pollFd, _ep_event, MAX_EVENTS, TIME_OUT)) == -1) 
			std::cerr << "QUID MESSAGE OU NON" << std::endl;//this->clearServer ??
		for (int i = 0; i < nfds; i++)
		{
			if ((_ep_event[i].events & EPOLLIN) == EPOLLIN)
			{
				if (_ep_event[i].data.fd == _listenSocket)
					this->_acceptNewClient(_listenSocket, _pollFd);
				else
				{
					if (this->_handleMessage(this->_ep_event[i]) == -1)
					{
						Client* userToDel = this->getClientByFd(this->_ep_event[i].data.fd);
						this->deleteClient(userToDel, this->_ep_event[i]);
						std::cerr << "Socket closed by client" << std::endl;

					}
				}
			}
		}
	}
	signal(SIGINT, SIG_DFL);
}


/**
 ** Serveur initialization method
 **/

void	Server::createCmdDict(void)
{
	_cmdDict["PASS"] = &pass;
	_cmdDict["NICK"] = &nick;
	_cmdDict["USER"] = &user;
	_cmdDict["CAP"] = &cap;
	// _cmdDict["OPER"] = &oper;
	// _cmdDict["MODE"] = &mode;
	// _cmdDict["SERVICE"] = &service;
	_cmdDict["QUIT"] = &quit;
	// _cmdDict["SQUIT"] = &squit;
	// _cmdDict["JOIN"] = &join;
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

void		Server::createAndBind(void)
{
	int		optval = 1;
	struct addrinfo hints, *result, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, "3336", &hints, &result) == -1)
		throw serverError("getaddrinfo", strerror(errno));
	for (p = result; p != NULL; p = p->ai_next)
	{
#ifdef DEBUG
		std::cout << "Method createAndBind srcs/Server.cpp" << std::endl;
		std::cout << "Value of result function getaddrinfo" << std::endl;
		std::cout << "ai_family : " << p->ai_family << std::endl;
		std::cout << "ai_protocol : " << p->ai_protocol << std::endl;
		std::cout << "ai_addr : " << p->ai_addr << std::endl;
		std::cout << "ai_addrlen : " << p->ai_addrlen << std::endl;
#endif
		if ((_listenSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			continue;
		}
		if (setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) == -1)
			continue ;
		if (fcntl(_listenSocket, F_SETFL, O_NONBLOCK) == -1)
			continue ;
		if (bind(_listenSocket, p->ai_addr, p->ai_addrlen) == 0)
			break;
		if (close(_listenSocket) == -1)
			throw serverError("close", strerror(errno));
	}
	if (p == NULL)
		throw serverError("bind", strerror(errno));
#ifdef DEBUG
	std::cout << "Socket created" << std::endl;
#endif
	freeaddrinfo(result);
}


/**
 **	Quit && Exit method
 **/

void	Server::clearServer(void) //TODO link with signal??!!
{
	//TODO delete channels before user bc channels are links to users
	//Close connection and fd and delete users
	struct epoll_event	ev;

	if (!this->_clientsOnServer.empty())
	{
		std::map<int, Client*>::const_iterator it_end = this->_clientsOnServer.end();
		for (std::map<int, Client*>::const_iterator it_begin = this->_clientsOnServer.begin(); it_begin != it_end; it_begin++)
		{
			try
			{
				if (epoll_ctl(this->_pollFd, EPOLL_CTL_DEL, it_begin->second->getFd(), &ev) == -1)
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
	if (close(this->_pollFd) == -1)
		std::cerr << "close issue" << std::endl;
}

void	Server::deleteClient(Client* user, epoll_event ep_event)
{
	struct epoll_event	ev;

	memset(&ev, 0, sizeof(ev));
	if (epoll_ctl(this->_pollFd, EPOLL_CTL_DEL, ep_event.data.fd, &ev) == -1)
		throw serverError("epoll_ctl", strerror(errno));
	if (close(ep_event.data.fd) == -1)
		throw serverError("close", strerror(errno));
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

