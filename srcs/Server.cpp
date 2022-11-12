/*Constructeur && Destructeur*/

//TODO add client message

#include "../includes/Class/Server.hpp"
#include "../includes/Class/User.hpp"
#include <signal.h> //TODO mettre dans Server.hpp
<<<<<<< HEAD
#include "utils.cpp"
=======
#include "../includes/utils.hpp"
#include "../includes/parser.hpp"
>>>>>>> b2237d17f6306b1433d2d374532e61df8deafd82

static bool	is_running=true;

Server::Server(int port, std::string password):_port(port), _hostname(HOSTNAME), _password_hash(hasher(password.c_str()))
{
	int			optval = 1;

	memset(&this->_ev, 0, sizeof(epoll_event));
	memset(&this->_ep_event, 0, sizeof(epoll_event) * MAX_EVENTS);
	memset(&this->_addr, 0, sizeof(sockaddr_in));
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
	this->_usersOnServer[client_fd] = new User(client_fd, this->getHostname()); //TODO check hostname
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
		User* userToDel = this->getUserByFd(this->_ep_event[i].data.fd);
		std::cerr << "Socket closed by client" << std::endl; //TODO delete before set as finish
		if (epoll_ctl(this->_pollfd, EPOLL_CTL_DEL, this->_ep_event[i].data.fd, &this->_ev) == -1)
			throw serverError("epoll_ctl", strerror(errno));
		if (close(this->_ep_event[i].data.fd) == -1)
			throw serverError("close", strerror(errno));
		this->deleteUser(userToDel);
	}
	else
	{
		std::string				toSplit(buffer);
		std::deque<std::string>	listOfCommands = split(toSplit, "\r\n");
		std::deque<Commands>	commandsList = manageMultipleCommands(listOfCommands);
		printAllCmds(commandsList);
	}

}

void	stopServer(int signal)
{
	(void)signal;
	std::cout << "\r";
	is_running = false;
}

void	Server::execute(void)
{
	int	nfds = 0;
	this->_createPoll();
	is_running = true;
	signal(SIGINT, stopServer);
	while (is_running)
	{
<<<<<<< HEAD
		if ((nfds = epoll_wait(this->_pollfd, this->_ep_event, MAX_EVENTS, -1)) == -1) //TODO define last arg as TIME OUT //INFO with a value of -1 it's going to wait indefinitly
=======
		if ((nfds = epoll_wait(this->_pollfd, this->_ep_event, MAX_EVENTS, -1)) == -1)//TODO define last arg as TIME OUT //INFO with a value of -1 it's going to wait indefinitly 
>>>>>>> b2237d17f6306b1433d2d374532e61df8deafd82
			std::cerr << "QUID MESSAGE OU NON" << std::endl;//this->clearServer ??
															//this->clearServer();
		for (int i = 0; i < nfds; i++)
		{
			if ((this->_ep_event[i].events & EPOLLIN) == EPOLLIN)
			{
				if (this->_ep_event[i].data.fd == this->_listenSocket)
					this->_acceptNewClient(this->_listenSocket, this->_pollfd);
				else
				{
					this->_handleMessage(i);
				}
			}
		}
	}
	signal(SIGINT, SIG_DFL);
}


void	Server::clearServer(void) //TODO link with signal??!!
{
	//TODO delete channels before user bc channels are links to users
	//Close connection and fd and delete users
	if (!this->_usersOnServer.empty())
	{
		std::map<int, User*>::const_iterator it_end = this->_usersOnServer.end();
		for (std::map<int, User*>::const_iterator it_begin = this->_usersOnServer.begin(); it_begin != it_end; it_begin++)
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

//TO DO 2 Functions
// STOP SERVER
// CLEAR SERVER

const std::string&		Server::getHostname(void)const
{
	return (this->_hostname);
}

const int&				Server::getListenSocket(void)const
{
	return (this->_listenSocket);
}

void				Server::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

std::deque<User*>	Server::getAllUsers(void)const
{
	std::deque<User*> allUsers;

	for (std::map<int, User*>::const_iterator it = this->_usersOnServer.begin(); it!= this->_usersOnServer.end(); it++)
	{
		allUsers.push_back(it->second);
	}

	return (allUsers);
}

User*	Server::getUserByFd(const int fd)const
{
	std::map<int, User*>::const_iterator it = this->_usersOnServer.end();
	if (this->_usersOnServer.find(fd) != it)
		return (this->_usersOnServer.find(fd)->second);
	return (NULL);
}

void	Server::printAllUsersFd(void)
{
	if (!this->_usersOnServer.empty())
	{
		std::map<int, User*>::const_iterator it;
		std::map<int, User*>::const_iterator ite = this->_usersOnServer.end();
		for (it = this->_usersOnServer.begin(); it != ite; it++)
		{
			std::cout << (it->second)->getHostname() << std::endl;
		}
	}

}

void	Server::deleteUser(User* user)
{
	std::map<int, User*>::iterator it;
	std::map<int, User*>::iterator ite = this->_usersOnServer.end();
	for (it = this->_usersOnServer.begin(); it != ite;)
	{
		if (user == it->second)
		{
			delete it->second;
			//close (it->second->getFd());
			this->_usersOnServer.erase(it++);
		}
		else
			it++;
	}
}

void	Server::sendMsgToFd(const std::string msg, const int fd)
{
	send(fd, msg.c_str(), msg.length(), MSG_DONTWAIT);
}

void	Server::createCmdDict(void) {
	_cmd_dict["PASS"] = &pass;
	_cmd_dict["NICK"] = &nick;
	_cmd_dict["USER"] = &user;
	_cmd_dict["OPER"] = &oper;
	_cmd_dict["MODE"] = &mode;
	_cmd_dict["SERVICE"] = &service;
	_cmd_dict["QUIT"] = &quit;
	_cmd_dict["SQUIT"] = &squit;
	_cmd_dict["JOIN"] = &join;
	_cmd_dict["PART"] = &part;
	_cmd_dict["TOPIC"] = &topic;
	_cmd_dict["NAMES"] = &names;
	_cmd_dict["LIST"] = &list;
	_cmd_dict["INVITE"] = &invite;
	_cmd_dict["KICK"] = &kick;
	_cmd_dict["PRIVMSG"] = &privmsg;
	_cmd_dict["NOTICE"] = &notice;
	_cmd_dict["MOTD"] = &motd;
	_cmd_dict["LUSERS"] = &lusers;
	_cmd_dict["VERSION"] = &version;
	_cmd_dict["STATS"] = &stats;
	_cmd_dict["LINKS"] = &links;
	_cmd_dict["TIME"] = &time;
	_cmd_dict["CONNECT"] = &connect;
	_cmd_dict["TRACE"] = &trace;
	_cmd_dict["ADMIN"] = &admin;
	_cmd_dict["INFO"] = &info;
	_cmd_dict["SERVLIST"] = &servlist;
	_cmd_dict["SQUERY"] = &squery;
	_cmd_dict["WHO"] = &who;
	_cmd_dict["WHOIS"] = &whois;
	_cmd_dict["WHOWAS"] = &whowas;
	_cmd_dict["KILL"] = &kill;
	_cmd_dict["PING"] = &ping;
	_cmd_dict["PONG"] = &pong;
	_cmd_dict["ERROR"] = &error;
	_cmd_dict["AWAY"] = &away;
	_cmd_dict["REHASH"] = &rehash;
	_cmd_dict["DIE"] = &die;
	_cmd_dict["RESTART"] = &restart;
	_cmd_dict["SUMMON"] = &summon;
	_cmd_dict["USERS"] = &users;
	_cmd_dict["WALLOPS"] = &wallops;
	_cmd_dict["USERHOST"] = &userhost;
	_cmd_dict["ISON"] = &ison;
}

User* Server::findUserByNickname(const std::string nickname)
{
	std::map<int, User*>::const_iterator currentUser;
	for (currentUser = _usersOnServer.cbegin(); currentUser != _usersOnServer.cend(); currentUser++)
	{
		if (currentUser->second->getNickname() == nickname)
			return currentUser->second;
	}
	return nullptr;
}

User* Server::findUserByFd(const int fd)
{
	return _usersOnServer[fd];
}

void	Server::sendNumericReplyToFd(NumericReplies reply, const int fd)
{
	std::string msg = ":" + getHostname() + " " + toString(reply.num) + " " + findUserByFd(fd)->getNickname() + " " + reply.msg;
	sendMsgToFd(msg, fd);
}

bool	Server::checkPassword(const std::string password) const
{
	return hasher(password.c_str()) == _password_hash;
}

Channel* Server::findChannelByName(const std::string name)
{
	return _channels[name];
}

Channel* Server::addChannel(std::string name, User* user)
{
	Channel *newChannel = new Channel(name, user);
	_channels[name] = newChannel;
	return newChannel;
}
