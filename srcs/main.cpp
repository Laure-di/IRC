#include "../includes/include.hpp"

int		main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "To launch IRC : ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	int			port;
	std::string	pwd(argv[2]);

	port = std::atoi(argv[1]);

	try
	{
		Server IRCServer(port, pwd);
		IRCServer.execute();
		IRCServer.clearServer();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
