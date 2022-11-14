#ifndef SERVER_HPP
# define SERVER_HPP

#include "../include.hpp"

//Variables server use
#define MAX_EVENTS	20
#define HOSTNAME	"localhost"
#define BUFFER_SIZE	50000
#define BACKLOG		10

//Variables commands use
#define	CMD_MAX_LGHT	510
#define MAX_PARAM		15

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
		std::map<const int, Client*>		_usersOnServer;
		std::map<std::string, Channel*>	_channels;
		cmd_dict						_cmd_dict;


	public:
		Server(int port, std::string password);
		void						execute(void);
		const std::string&			getHostname(void) const;
		const int&					getListenSocket(void) const;
		std::deque<Client*>			getAllUsers(void)const;
		Client*						getUserByNickname(std::string nickname)const;
		Client*						getUserByFd(const int fd)const;
		void						setHostname(std::string);
		void						clearServer(void);
		void						deleteUser(Client* user);
		void						printAllUsersFd(void);//TODO delete just debug
		void						sendMsgToFd(const std::string msg, const int fd);
		void						sendMsgToFd(NumericReplies reply, const int fd);
		void						createCmdDict(void);
		Client*						findUserByNickname(const std::string nickname);
		Client*						findUserByFd(const int fd);
		bool						checkPassword(const std::string password) const;
		Channel*					findChannelByName(const std::string name);
		Channel*					addChannel(std::string name, Client* user);
		void						executeCommands(char *buffer, Client* currentClient);
};

#endif
