#include "../includes/Class/Server.hpp"

//TODO handle signal

int		main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "To launch IRC : ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	//No Issue we are building the server
	int			port;
	std::string pwd;

	port = std::atoi(argv[1]);
	pwd = argv[2];

	Server IRCServer(port, pwd);
	IRCServer.execute();
}
