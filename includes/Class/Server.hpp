_mast#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>

class Server
{
	private:
		void				_createPoll(void);
		int					_waitPool(void);

		int					_port;
		std::string			_hostname;
		std::string			_password;
		struct sockaddr_in	_addr;
		int					_masterSocket, _clientSocket;
		struct epoll_event	_ev, _ep_event[MAX_EVENTS];//TODO define MAX_EVENTS
		fd_set				_readfds;
		char				_buffer[1025];//map ??
	public:
		Server(int port, std::string password);
		void					execute(void);
		const std::string&		getHostname(void) const;
		const int&				getMasterSocket(void) const;

}

#endif
