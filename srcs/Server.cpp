/*Constructeur && Destructeur*/

//TODO add client message

#include "../includes/Class/Server.hpp"

Server::Server(int port, std::string password):_password(password), _port(port), _hostname(HOSTNAME)
{
	int			optval = 1;

	memset(&this->_ev, 0, sizeof(epoll_event));
	memset(&this->_ep_event, 0, sizeof(epoll_event) * MAX_EVENTS);
	memset(&this->_addr, 0, sizeof(sockaddr_in));
	//Creation of the master socket
	if ((this->_listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	//Set socket to allow multi connections && reuse of socket
	if (setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	//binding the socket to the port
	if (bind(this->_listenSocket,(const struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		if (close(this->_listenSocket) == -1)
			std::cerr << "Error: couldn't close properly the server's socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(this->_listenSocket, BACKLOG) == -1) //TODO Define Baklog 10
	{
		std::cerr << std::strerror(errno)<< std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_createPoll(void)
{
	if ((this->_poolSocket = epoll_create1(0)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_listenSocket;
	if (epoll_ctl(this->_poolSocket, EPOLL_CTL_ADD, this->_listenSocket, &this->_ev) == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_acceptNewClient(int listenSocket, int poolSocket)
{
	socklen_t				addrlen;
	struct sockaddr_storage client_addr;
	int						client_fd;

	addrlen = sizeof(struct sockaddr_storage);
	if ((client_fd = accept(listenSocket, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen)) == - 1)
		std::cerr << std::strerror(errno) << std::endl;
	//TODO create a new User
	std::cout << client_fd << std::endl;
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = client_fd;
	if (epoll_ctl(poolSocket, EPOLL_CTL_ADD, client_fd, &this->_ev) == -1)
		std::cerr << strerror(errno) << std::endl;
	std::cout << "nouvelle connexion" << std::endl;
}

void	Server::_handleMessage(int i)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	numbytes;

	memset(buffer, 0, BUFFER_SIZE);
	numbytes = recv(this->_ep_event[i].data.fd, buffer, BUFFER_SIZE, 0);
	if (numbytes == -1)
		std::cerr << "recv error" << std::endl;
	else if (numbytes == 0) //client close connection
	{
		std::cerr << "Socket closed by client" << std::endl;
		if (epoll_ctl(this->_poolSocket, EPOLL_CTL_DEL, this->_ep_event[i].data.fd, &this->_ev) == -1)
			std::cerr << std::strerror(errno) << std::endl;
		if (close(this->_ep_event[i].data.fd) == -1)
			std::cerr << std::strerror(errno) << std::endl;
		//delete User
	}
	else
	{
		std::cout << buffer << std::endl;
	}

}

void	Server::execute(void)
{
	int	nfds = 0;
	this->_createPoll();
	while (1)
	{
		if ((nfds = epoll_wait(this->_poolSocket, this->_ep_event, MAX_EVENTS, -1)) == -1)
			std::cerr << std::strerror(errno) << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			if ((this->_ep_event[i].events & EPOLLIN) == EPOLLIN)
			{
				if (this->_ep_event[i].data.fd == this->_listenSocket)
					this->_acceptNewClient(this->_listenSocket, this->_poolSocket);
				else
				{
					this->_handleMessage(i);
				}
			}
		}

	}
}


void	Server::quit(void)
{
	if (close(this->_listenSocket) == -1)
		std::cerr << "close issue" << std::endl;
	if (close(this->_poolSocket) == -1)
		std::cerr << "close issue" << std::endl;
	for (int i = 0; i < MAX_EVENTS; i++)
	{
		if (close(this->_ep_event[i].data.fd) == -1)
			std::cerr << "close issue" << std::endl;
	}
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

