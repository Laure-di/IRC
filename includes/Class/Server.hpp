_mast#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>

class Server
{
	private:
		int					_port;
		std::string			_hostname;
		std::string			_password;
		struct sockaddr_in	_addr;
		int					_masterSocket, _newSocket, _clientSocket[MAX_CLIENTS]; //TODO need to define MAX_CLIENTS
		fd_set				_readfds;
		char				_buffer[1025];//class or straight to the code?
	public:
		Server(int port, std::string password);

		const std::string&		getHostaneme(void) const;
		const int&				getMasterSocket(void) const;

		//METHOD A IMPLEMENTER
		//create_socket
		//bind_socket
}

#endif
