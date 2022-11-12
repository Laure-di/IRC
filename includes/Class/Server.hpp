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
#include "../Commands.hpp"
#include "../CommandResponses.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

//Variables server use
#define MAX_EVENTS	20
#define HOSTNAME	"localhost"
#define BUFFER_SIZE	50000
#define BACKLOG		10

//Variables commands use
#define	CMD_MAX_LGHT	510
#define MAX_PARAM		15

struct	Commands
{
	std::string				command;
	std::string				prefix;
	std::deque<std::string>	params;
	bool					colon;
	/*Commands(std::string cmd, std::string pref, std::deque<std::string> parameters)
	{
		command = cmd;
		prefix = pref;
		params = parameters;
	}*/
};

class Server
{
	private:
		void							_createPoll(void);
		int								_waitPool(void);
		void							_acceptNewClient(int listenSocket, int poolSocket);
		void							_handleMessage(int i);
		int								_port;
		std::string						_hostname;
		unsigned						_password_hash;
		sockaddr_in						_addr;
		int								_listenSocket, _pollfd;
		epoll_event						_ev, _ep_event[MAX_EVENTS];//TODO degager de la class
		std::map<const int, User*>		_usersOnServer;
		std::map<std::string, Channel*>	_channels;
		cmd_dict						_cmd_dict;


	public:
		Server(int port, std::string password);
		void						execute(void);
		const std::string&			getHostname(void) const;
		const int&					getListenSocket(void) const;
		std::deque<User*>			getAllUsers(void)const;
		User*						getUserByNickname(std::string nickname)const;
		User*						getUserByFd(const int fd)const;
		void						setHostname(std::string);
		void						clearServer(void);
		void						deleteUser(User* user);
		void						printAllUsersFd(void);//TODO delete just debug
		void						sendMsgToFd(const std::string msg, const int fd);
		void						sendNumericReplyToFd(NumericReplies reply, const int fd);
		void						createCmdDict(void);
		User*						findUserByNickname(const std::string nickname);
		User*						findUserByFd(const int fd);
		bool						checkPassword(const std::string password) const;
		Channel*					findChannelByName(const std::string name);
		Channel*					addChannel(std::string name, User* user);

};

#endif
