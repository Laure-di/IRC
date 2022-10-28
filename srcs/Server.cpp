/*Constructeur && Destructeur*/

//TODO add client message

Server::Server(int port, std::string password)
{
	int			opt = 1;
	std::string	hostname[1024];

	this->_password = password;
	this->_port = port;
	gethostname(hostname, 1024);
	std::cout << "DEBUG hostname display : " << hostname << std::endl;
	this->_hostname = hostname;
	//Creation of the master socket
	if (this->_masterSocket = socket(AF_INET, SOCK_STREAM, 0) == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	//Set socket to allow multi connections && reuse of socket
	if (setsockopt(this->_masterSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		std::cerr << std::sterror(errno) << std::enld;
		exit(EXIT_FAILURE);
	}
	this->_addr.sin_family = AF_INET;
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    this->addr.sin_port = htons(port);
	//binding the socket to the port
	if (bind(this->_masterSocket, static_cast<struct sockadrr *>&this->_addr, sizeof(this->_addr)) == 1)
	{
		std::cerr << std::sterror(errno) << std::enld;
		exit(EXIT_FAILURE);
	}
	if (listen(this->_masterSocket, BACKLOG) == -1)
	{
		std::cerr << std::error(errno)<< std::enld;
		exit(EXIT_FAILURE);
	}
}

void	Server::_createPoll(void)
{
	if ((this->_clientSocket = epoll_create1(0)) == -1)
		std::cerr << std::error(errno) << std::enld;
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = this->_masterSocket;
	if (epoll_ctl(this->_clientSocket, EPOLL_CTL_ADD, this->_masterSocket, &this->_ev) == -1)
	{
		std::cerr << std::error(errno) << st::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_acceptNewClient(int masterSocket, int clientSocket)
{
	socklen_t				addrlen;
	struct sockaddr_storage client_addr;
	struct sockaddr_in		*ptr;
	int						client_fd;

	addrlen = sizeof(struct sockaddr_storage);
	if ((client_fd = accept(masterSocket, static_cast<struct sockaddr*>&client_adress, &addrlen)) == - 1)
		std::cerr << std::error(errno) << std::endl;
	//TODO create a new User
	this->_ev.events = EPOLLIN;
	this->_ev.data.fd = client_fd;
	if (epoll_ctl(clientSocket, EPOLL_CTL_ADD, client_fd, &ev) == -1)
		std::cerr << error(errno) << std::endl;
}

void	Server::execute(void)
{
	int	nfds = 0;
	this->_createPoll();
	while (1)
	{
		if ((nfds = epoll_wait(this->_clientSocket, this->_ep_event, MAX_EVENT, -1)) == -1)
			std::cerr << std::error(errno) << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			if ((this->_ep_event[i].events & EPOLIN) == EPOLIN)
			{
				if (this->_ep_event[i].data.fd == this->_masterSocket)
					this->_acceptNewClient(this->_masterSocket, this->_clientSocket);
				else
				{
					//Receive data from existing connection
				}
			}
		}

	}

}

//TO DO 2 Functions
// STOP SERVER
// CLEAR SERVER

const std::string&		Server::getHostname(void)
{
	return (this->_hostname);
}

const int&				Server::getMasterSocket(void)const
{
	return (this->_masterSocket);
}
