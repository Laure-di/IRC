#include "../includes/include.hpp"

#define DEBUG


/****************************************************************************/
/***					Constructor && Destructor						 ***/
/***																	***/
/*************************************************************************/

/**
 ** @Brief : Socket creation with a domain IPV4 (AF_IFNET), a communication type TCP (SOCK_STREAM), and protocol IP (0).
 **			Set socket option to allow multi-connection and the reuse of socket.
 **			Bind the socket to the address and port number (with info from getaddrinfo)
 **/

Server::Server(int port, std::string password, char *portStr): _port(port), _hostname(HOSTNAME), _version(VERSION), _passwordHash(hasher(password.c_str())), _messageOftheDay(MOTD_FILE), _adminLogin("admin"), _adminPasswordHash(4141857313)
{
	this->createAndBind(portStr);
	memset(&_ep_event, 0, sizeof(epoll_event) * MAX_EVENTS);
	this->createCmdDict();
	if (listen(_listenSocket, BACKLOG) == -1)
		throw serverError("listen", strerror(errno));
	time_t      rawtime = time(NULL);
	struct tm   *info;
	info = localtime(&rawtime);
	std::string launchingDate = std::string(asctime(info));
	_launchingDate = launchingDate.substr(0, launchingDate.size()-1);;
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

	for (std::map<int, Client*>::const_iterator it = this->_clients.begin(); it!= this->_clients.end(); it++)
	{
		allClients.push_back(it->second);
	}

	return (allClients);
}

std::vector<Client*>	Server::getAllClientsMatching(std::string pattern, std::vector<Client*>listOfClients) const
{
	std::vector<Client*> allClients;
	std::vector<Client*>::iterator it;
	for (it = listOfClients.begin(); it!= listOfClients.end(); it++)
	{
		Client *target = *it;
		std::string nickname = target->getNickname();
		if (strmatch(nickname, pattern)
				|| strmatch(nickname, pattern)
				|| strmatch(nickname, pattern))
			allClients.push_back(target);
	}
	return (allClients);
}

std::vector<Client*>	Server::getAllClientsMatching(std::string nicknameMask) const
{
	std::vector<Client*> allClients;
	std::vector<Client*>::const_iterator it;
	for (it = _clients.begin(); it!= _clients.end(); it++)
	{
		Client *target = *it;
		std::string nickname = target->getNickname();
		if (strmatch(nickname, nicknameMask))
			allClients.push_back(target);
	}
	return (allClients);
}

std::vector<Client*>	Server::getAllClientsMatching(std::vector<std::string> nicknameMasks) const
{
	std::vector<Client*> allClients;
	std::vector<std::string>::iterator it;
	for (it = nicknameMasks.begin(); it!= nicknameMasks.end(); it++)
	{
		std::string nicknameMask = *it;
		allClients.push_back(getAllClientsMatching(nicknameMask));
	}
	return (allClients);
}

std::vector<Client *>		Server::getAllClientsVisibleForClient(Client *client)const
{
	std::vector<Client *> listOfVisibles;
	std::map<const int, Client*>::const_iterator cit;
	for (cit = _clients.begin(); cit != _clients.end(); cit++)
	{
		Client *target = cit->second;
		int targetMode = target->getMode();
		if (!(targetMode & INVISIBLE) && !client->isInSameChannel(target))
			listOfVisibles.push_back(target);
	}
	return listOfVisibles;
}

Client* Server::getClientByNickname(const std::string nickname) const
{
	std::map<const int, Client*>::const_iterator currentUser;
	for (currentUser = _clients.begin(); currentUser != _clients.end(); currentUser++)
	{
		if (currentUser->second->getNickname() == nickname)
			return currentUser->second;
	}
	return NULL;
}

Client* Server::getClientByFd(size_t fd)
{
	std::map<const int, Client*>::const_iterator cit;
	cit = _clients.find(fd);
	if (cit != _clients.end())
		return cit->second;
	return 0;
}


Channel* Server::getChannelByName(const std::string name)
{
	std::map<std::string, Channel*>::const_iterator cit;
	cit = _channels.find(name);
	if (cit != _channels.end())
		return cit->second;
	return 0;
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

void Server::createNewChannel(int creator, std::string name)
{
	//Add creation of channel
	(void)creator;
	(void)name;
}

void									Server::addNicknameUnavailable(std::string nickname)
{
	_nicknameUnavailable[nickname] = time(NULL);
}

/**
 ** @Brief manage an ifstream and return a formated string to send to client
 **		https://cplusplus.com/reference/ios/ios/exceptions/
 **		https://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badit-failbit-eofbit-and-perror/
 **/
void	Server::shapeMessageOftheDay(std::string fileName, int socket)
{
	std::string		msg;
	std::ifstream	file;
	file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
	try
	{
		file.open(fileName.c_str(), std::ifstream::in);
	}
	catch (std::ifstream::failure e) {
		throw numericRepliesError();
	}
	try
	{
		while (std::getline(file, msg))
			this->sendMsg(RPL_MOTD(msg), socket);
	}catch (std::istream::failure e){}
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
#ifdef DEBUG
	std::cout << "Sent in fd " << fd << " :\n---------------\n" << msg << "---------------\n\n";
#endif
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


void	Server::sendWho(int socket, std::vector<Client *>listOfClients)
{
	std::vector<Client *>::iterator it;
	for (it = listOfClients.begin(); it < listOfClients.end(); it++)
	{
		Client *target = *it;
		sendMsg(target->getWhoMessage(), socket);
	}
	sendMsg(RPL_ENDOFWHO(getClientByFd(socket)->getNickname()), socket);
}

void	Server::broadcast(std::string msg, int expediteur)
{
	std::string finalMsg = "message from : " + getClientByFd(expediteur)->getNickname() + " " + msg;
	std::map<const int, Client*>::iterator	it = _clients.begin();
	for (; it != _clients.end(); it++)
		sendMsg(finalMsg, it->second->getFd());
}


bool	Server::checkPassword(const std::string password) const
{
	return hasher(password.c_str()) == _passwordHash;
}

bool	Server::checkAdmin(const std::string login, const std::string password) const
{
	return (login == _adminLogin) & (hasher(password.c_str()) == _adminPasswordHash);
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
			return this->sendMsg(ERR_NOTREGISTERED, client->getFd());
		if (_cmdDict.find(commandsList[i].command) != _cmdDict.end())
		{
			if (commandsList[i].params.size() <= MAX_PARAMS)
				this->_cmdDict[commandsList[i].command](this, client->getFd(), commandsList[i]);
			else
			{
				std::string error("You can't use more than 15 parameters");
				this->sendMsg(ERR_CLIENT(error), client->getFd());
			}
		}
		else
			this->sendMsg(ERR_UNKNOWNCOMMAND(commandsList[i].command), client->getFd());
		if (i == commandsList.size())
			break;
		it++;
	};
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
	this->_clients[client_fd] = new Client(client_fd, inet_ntoa(client_addr.sin_addr)); //TODO check hostname
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = client_fd;
	if (epoll_ctl(pollfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
		throw serverError("epoll_ctl", strerror(errno));
#ifdef DEBUG
	std::cout << "nouvelle connexion" << std::endl;
#endif
}

void	Server::sendAllUsers(int socket)
{
	std::map<std::string, Channel*>::const_iterator cit;
	for (cit = _channels.begin(); cit != _channels.end(); cit++)
	{
		Channel *channel = cit->second;
		channel->sendListOfNames(socket);
	}
	std::vector<std::string> clientNotInChannels;
	std::map<const int, Client*>::const_iterator cit2;
	for (cit2 = _clients.begin(); cit2 != _clients.end(); cit2++)
	{
		Client *client = cit2->second;
		if (!isInChannel(client->getNickname()))
			clientNotInChannels.push_back(client->getNickname());
	}
	if (clientNotInChannels.size())
	{
		std::string delim = " ";
		std::ostringstream joinedClientNicknames;
		std::copy(clientNotInChannels.begin(), clientNotInChannels.end(), std::ostream_iterator<std::string>(joinedClientNicknames, delim.c_str()));
		std::string channelStatus = "*";
		std::string channelName = "channel";
		sendMsg(RPL_NAMREPLY(channelStatus, channelName, joinedClientNicknames.str()), socket);
		sendMsg(RPL_ENDOFNAMES(channelName), socket);
	}
}

void	Server::sendAllChannels(int socket)
{
	std::map<std::string, Channel*>::const_iterator cit;
	for (cit = _channels.begin(); cit != _channels.end(); cit++)
	{
		Channel *channel = cit->second;
		if (channel && !(channel->getMode() & SECRET))
			channel->sendInfo(socket);
	}
	sendMsg(RPL_LISTEND, socket);
}


int		Server::_handleMessage(epoll_event ep_event)
{
	char				buffer[BUFFER_SIZE];
	ssize_t				numbytes;
	Client* currentClient = this->getClientByFd(ep_event.data.fd);

	if (isCmdFull(currentClient->getBuffer()))
		currentClient->clearBuffer();
	memset(buffer, 0, BUFFER_SIZE);
	numbytes = recv(ep_event.data.fd, buffer, BUFFER_SIZE, 0);
#ifdef DEBUG
	std::cout << "Received from fd " << std::hex << ep_event.data.fd << " :\n---------------\n" << buffer << "---------------\n\n";
#endif
	if (numbytes <= 0)
		return (-1);
	else
	{
		buffer[numbytes] = '\0';
		currentClient->append(buffer);
#ifdef DEBUG
		std::cout << "Client Buffer " << currentClient->getBuffer() << " :\n---------------\n" << "---------------\n\n";
#endif
		if (isCmdFull(currentClient->getBuffer()))
		{
			this->executeCommands(currentClient->getBuffer(), currentClient);
		}
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
			return ;
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
						this->deleteClient(userToDel->getFd());
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
	_cmdDict["OPER"] = &oper;
	_cmdDict["MODE"] = &mode;
	_cmdDict["SERVICE"] = &service;
	_cmdDict["QUIT"] = &quit;
	_cmdDict["SQUIT"] = &squit;
	_cmdDict["JOIN"] = &join;
	_cmdDict["PART"] = &part;
	_cmdDict["TOPIC"] = &topic;
	_cmdDict["NAMES"] = &names;
	_cmdDict["LIST"] = &list;
	_cmdDict["INVITE"] = &invite;
	_cmdDict["KICK"] = &kick;
	_cmdDict["PRIVMSG"] = &privmsg;
	_cmdDict["NOTICE"] = &notice;
	_cmdDict["MOTD"] = &motd;
	_cmdDict["LUSERS"] = &lusers;
	_cmdDict["VERSION"] = &version;
	_cmdDict["STATS"] = &stats;
	_cmdDict["LINKS"] = &links;
	_cmdDict["TIME"] = &time;
	_cmdDict["CONNECT"] = &connect;
	_cmdDict["TRACE"] = &trace;
	_cmdDict["ADMIN"] = &admin;
	_cmdDict["INFO"] = &info;
	_cmdDict["SERVLIST"] = &servlist;
	_cmdDict["SQUERY"] = &squery;
	_cmdDict["WHO"] = &who;
	_cmdDict["WHOIS"] = &whois;
	_cmdDict["WHOWAS"] = &whowas;
	_cmdDict["KILL"] = &kill;
	_cmdDict["PING"] = &ping;
	_cmdDict["PONG"] = &pong;
	_cmdDict["ERROR"] = &error;
	_cmdDict["AWAY"] = &away;
	_cmdDict["REHASH"] = &rehash;
	_cmdDict["DIE"] = &die;
	_cmdDict["RESTART"] = &restart;
	_cmdDict["SUMMON"] = &summon;
	_cmdDict["USERS"] = &users;
	_cmdDict["WALLOPS"] = &wallops;
	_cmdDict["USERHOST"] = &userhost;
	_cmdDict["ISON"] = &ison;
}

void		Server::createAndBind(char *port)
{
	int		optval = 1;
	struct addrinfo hints, *result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port, &hints, &result) == -1)
		throw serverError("getaddrinfo", strerror(errno));
	if ((_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1)
		throw serverError("socket creation:", strerror(errno));
	setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval));
	fcntl(_listenSocket, F_SETFL, O_NONBLOCK);
	if (bind(_listenSocket, result->ai_addr, result->ai_addrlen) != 0)
	{
		freeaddrinfo(result);
		if (close(_listenSocket) == -1)
			throw serverError("close:", strerror(errno));
		throw serverError("bind:", strerror(errno));
	}
	freeaddrinfo(result);
#ifdef DEBUG
	std::cout << "Socket created" << std::endl;
#endif
}

/**
 **	Quit && Exit method
 **/

Channel  *Server::addChannel(std::string name, Client* creator)
{
	Channel *newChannel = new Channel(this, name, creator);
	_channels[name] = newChannel;
	return newChannel;
}

void	Server::deleteAllChannels(void)
{
	if (!_channels.empty())
	{
		std::map<std::string, Channel*>::iterator it = _channels.begin();
		for (; it != _channels.end(); it++)
			delete it->second;
		_channels.clear();
	}
}

void	Server::clearServer(void)
{
	struct epoll_event	ev;

	deleteAllChannels();
	if (!this->_clients.empty())
	{
		std::map<int, Client*>::const_iterator it_end = this->_clients.end();
		for (std::map<int, Client*>::const_iterator it_begin = this->_clients.begin(); it_begin != it_end; it_begin++)
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


void	Server::deleteClient(int socket)
{
	struct epoll_event	ev;
	std::map<int, Client*>::iterator	it;
	it = this->_clients.find(socket);
	if (it == _clients.end())
	{
		std::cerr << "Something went wrong, we can't find the client to delete" << std::endl;
		return ;
	}

	it->second->removeFromAllChannels();
	delete it->second;
	_clients.erase(it->first);
	if (epoll_ctl(this->_pollFd, EPOLL_CTL_DEL, socket, &ev) == -1)
		throw serverError("epoll_ctl:", strerror(errno));
	if (close(socket) == -1)
		throw serverError("close", strerror(errno));
	std::cout << "Connection close by client" << std::endl;
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
	std::string nickname = client->getNickname();
	if (!channel)
		channel = addChannel(channelName, client);
	else
	{
		if ((channel->getMode() & INVITATION) && !(channel->isInvited(nickname)))
			return sendMsg(ERR_INVITEONLYCHAN(channelName), socket);
		if (channel->isBanned(nickname) && !(channel->isExcepted(nickname)))
			return sendMsg(ERR_BANNEDFROMCHAN(channelName), socket);
		if ((channel->getMode() & LIMIT) && channel->getNumberOfUsers() == channel->getMaxLimitOfUsers())
			return sendMsg(ERR_CHANNELISFULL(channelName), socket);
		if ((channel->getMode() & KEY) && !channel->checkPassword(key))
			return sendMsg(ERR_BADCHANNELKEY(channelName), socket);
		channel->addClient(socket);

	}
	channel->sendJoin(client);
	channel->sendListOfNames(socket);
	if (!channel->getTopic().empty())
		channel->sendTopic(socket);
	else
		sendMsg(RPL_NOTOPIC(channelName), socket);
	client->addChannel(channel, channelName);
}

/**
 * @brief Helper function to check wether the channel name is correct and the
 * client can leave it
 */
void	Server::checkAndLeaveChannel(int socket, std::string channelName, std::string leaveMessage)
{
	Channel *channel = getChannelByName(channelName);
	if (!channel)
		return sendMsg(ERR_NOSUCHCHANNEL(channelName), socket);
	Client *client = getClientByFd(socket);
	if (!channel->getClientByNickname(client->getNickname()))
		return sendMsg(ERR_NOTONCHANNEL(channelName), socket);
	channel->sendPart(client, leaveMessage);
	channel->remClient(client->getNickname());
	client->removeChannel(channel->getName());
}

bool	Server::isInChannel(const std::string nickname) const
{
	std::map<std::string, Channel*>::const_iterator cit;
	for (cit = _channels.begin(); cit != _channels.end(); cit++)
	{
		Channel *channel = cit->second;
		if (channel->getClientByNickname(nickname))
			return true;
	}
	return false;
}
