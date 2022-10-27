/*Constructeur && Destructeur*/

Server::Server(int port, std::string password)
{
	int			opt = 1;
	std::string	hostname[1024];

	this->_password = password;
	this->_port = port;
	gethostname(hostname, 1024);
	std::cout << "DEBUG hostname display : " << hostname << std::endl;
	this->_hostname = hostname;
	//Initialise client_socket to 0
	std::memset(&_clientSocket, 0, MAX_CLIENTS * sizeof(int));
	/*for (i = 0; i < MAX_CLIENTS; i++)
    {
        _clientSocket[i] = 0;
    }*/

	//Creation of the master socket
	if (_masterSocket = socket(AF_INET, SOCK_STREAM, 0) == -1)
	{
		std::cerr << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	//Set socket to allow multi connections && reuse of socket
	if (setsockopt(_masterSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		std::cerr << std::sterror(errno) << std::enld;
		exit(EXIT_FAILURE);
	}
	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
	//binding the socket to the port
	if (bind(_masterSocket, (struct sockadrr *)&_addr, sizeof(_addr)) == 1)
	{
		std::cerr << std::sterror(errno) << std::enld;
		exit(EXIT_FAILURE);
	}
	if (listen(_masterSocket, MAX_CLIENTS) == -1)
	{
		std::cerr << std::error(errno)<< std::enld;
	}

}
