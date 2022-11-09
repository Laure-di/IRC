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
#include <map>
#include <deque>

#include "../Exceptions.hpp"
#include "User.hpp"

#define MAX_EVENTS	512
#define HOSTNAME	"localhost"
#define BUFFER_SIZE	50000
#define BACKLOG		10

class Server
{
	private:
		void						_createPoll(void);
		int							_waitPool(void);
		void						_acceptNewClient(int listenSocket, int poolSocket);
		void						_handleMessage(int i);
		int							_port;
		std::string					_hostname;
		std::string					_password;
		sockaddr_in					_addr;
		int							_listenSocket, _poolSocket;
		epoll_event					_ev, _ep_event[MAX_EVENTS];
		std::map<const int, User*>	_usersOnServer;



	public:
		Server(int port, std::string password);
		void						execute(void);
		const std::string&			getHostname(void) const;
		const int&					getListenSocket(void) const;
		std::deque<User*>			getAllUsers(void)const;
		User*						getUserByNickname(std::string nickname)const;
		User*						getUserByFd(const int fd)const;
		void						setHostname(std::string);
		void						quit(void);

};

#endif
