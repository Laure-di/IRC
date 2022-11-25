#include "../includes/include.hpp"

int isArgValid(char **argv)
{
	if (!isNumber(argv[1]))
		return 0;
	int port = std::atoi(argv[1]);
	if (port < 0 || 65535 < port)
	{
		std::cout << "The port must be between 0 and 2^16 -1" << std::endl;
		return 0;
	}
	return 1;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "To launch IRC : ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	if (!isArgValid(argv))
		return (1);
	int         port;
	std::string pwd(argv[2]);
	port = std::atoi(argv[1]);
	try
	{
		Server IRCServer(port, pwd, argv[1]);
		try
		{
			IRCServer.execute();
		}
		catch (Server::serverError &e)
		{
			std::cerr << e.what() << std::endl;
		}
		try
		{
			IRCServer.clearServer();
		}
		catch (Server::serverError &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	catch (Server::serverError &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return (0);
}
