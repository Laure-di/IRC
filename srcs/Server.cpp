/*Constructeur && Destructeur*/

//TODO add client message

#include "../includes/Class/Server.hpp"

Server::Server(int port, std::string password)
{
	int			optval = 1;

	this->_password = password;
	this->_port = port;
	this->_hostname = HOSTNAME;
	//Creation of the master socket
	if ((this->_masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	//Set socket to allow multi connections && reuse of socket
	if (setsockopt(this->_masterSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	//binding the socket to the port
	if (bind(this->_masterSocket,(const struct sockaddr *)&this->_addr, sizeof(this->_addr)) == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		if (close(this->_masterSocket) == -1)
			std::cerr << "Error: couldn't close properly the server's socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(this->_masterSocket, BACKLOG) == -1) //TODO Define Baklog 10
	{
		std::cerr << std::strerror(errno)<< std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_createPoll(void)
{
	if ((this->_clientSocket = epoll_create1(0)) == -1)
		std::cerr << std::strerror(errno) << std::endl;
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_masterSocket;
	if (epoll_ctl(this->_clientSocket, EPOLL_CTL_ADD, this->_masterSocket, &this->_ev) == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_acceptNewClient(int masterSocket, int clientSocket)
{
	socklen_t				addrlen;
	struct sockaddr_storage client_addr;
	int						client_fd;

	addrlen = sizeof(struct sockaddr_storage);
	if ((client_fd = accept(masterSocket, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen)) == - 1)
		std::cerr << std::strerror(errno) << std::endl;
	//TODO create a new User
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = client_fd;
	if (epoll_ctl(clientSocket, EPOLL_CTL_ADD, client_fd, &_ev) == -1)
		std::cerr << strerror(errno) << std::endl;
	std::cout << "nouvelle connexion" << std::endl;
}

void	Server::_handleMessage(int i)
{
	char	buffer[BUFFER_SIZE];//TODO define a BUFFER_SIZE
	ssize_t	numbytes;

	memset(buffer, 0, BUFFER_SIZE);
	numbytes = recv(this->_ep_event[i].data.fd, buffer, BUFFER_SIZE, 0);
	if (numbytes == -1)
		std::cerr << "recv error" << std::endl;
	else if (numbytes == 0) //client close connection
	{
		std::cerr << "Socket closed by client" << std::endl;
		if (epoll_ctl(this->_clientSocket, EPOLL_CTL_DEL, this->_ep_event[i].data.fd, &this->_ev) == -1)
			std::cerr << std::strerror(errno) << std::endl;
		if (close(this->_ep_event[i].data.fd) == -1)
			std::cerr << std::strerror(errno) << std::endl;
	}
	else
	{
		buffer[numbytes] = '\0';
		std::cout << buffer << std::endl;
	}

}

void	Server::execute(void)
{
	int	nfds = 0;
	this->_createPoll();
	while (1)
	{
		if ((nfds = epoll_wait(this->_clientSocket, this->_ep_event, MAX_EVENTS, -1)) == -1)
			std::cerr << std::strerror(errno) << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			if ((this->_ep_event[i].events & EPOLLIN) == EPOLLIN)
			{
				if (this->_ep_event[i].data.fd == this->_masterSocket)
					this->_acceptNewClient(this->_masterSocket, this->_clientSocket);
				else
				{
					this->_handleMessage(i);
				}
			}
		}

	}

}

//TO DO 2 Functions
// STOP SERVER
// CLEAR SERVER

const std::string&		Server::getHostname(void)const
{
	return (this->_hostname);
}

const int&				Server::getMasterSocket(void)const
{
	return (this->_masterSocket);
}

void				Server::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

