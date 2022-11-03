#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <cerrno>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define MAX_EVENTS	500000
#define HOSTNAME	"localhost"
#define BUFFER_SIZE	50000
#define BACKLOG		10

class Server
{
	private:
		void				_createPoll(void);
		int					_waitPool(void);
		void				_acceptNewClient(int masterSocket, int clientSocket);
		void				_handleMessage(int i);
		int					_port;
		std::string			_hostname;
		std::string			_password;
		struct sockaddr_in	_addr;
		int					_masterSocket, _clientSocket;
		struct epoll_event	_ev, _ep_event[MAX_EVENTS];//TODO define MAX_EVENTS
	public:
		Server(int port, std::string password);
		void					execute(void);
		const std::string&		getHostname(void) const;
		const int&				getMasterSocket(void) const;
		void					setHostname(std::string);

};

#endif
