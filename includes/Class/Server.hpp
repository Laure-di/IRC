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
<<<<<<< HEAD
		void							_createPoll(void);
		int								_waitPool(void);
		void							_acceptNewClient(int listenSocket, int poolSocket);
		void							_handleMessage(int i);
		int								_port;
		std::string						_hostname;
		unsigned						_passwordHash;
		sockaddr_in						_addr;
		int								_listenSocket, _pollfd;
		epoll_event						_ev, _ep_event[MAX_EVENTS];//TODO degager de la class
		std::map<const int, Client*>	_clientsOnServer;
		std::map<std::string, Channel*>	_channels;
		cmd_dict						_cmdDict;
		std::string						_messageOfTheDay;
=======
		void								_createPoll(void);
		int									_waitPool(void);
		void								_acceptNewClient(int listenSocket, int poolSocket);
		void								_handleMessage(int i);
		int									_port;
		std::string							_hostname;
		unsigned							_password_hash;
		sockaddr_in							_addr;
		int									_listenSocket, _pollfd;
		epoll_event							_ev, _ep_event[MAX_EVENTS];//TODO degager de la class
		std::map<const int, Client*>		_usersOnServer;
		std::map<std::string, Channel*>		_channels;
		cmd_dict							_cmd_dict;
		std::map<std::string, time_t>		_nicknameUnavailable;
>>>>>>> fcfa0b2d0b945ce74a15f5d971a2282b1d8c94f8


	public:
		Server(int port, std::string password);
<<<<<<< HEAD
		void						execute(void);
		const std::string&			getHostname(void) const;
		const int&					getListenSocket(void) const;
		std::deque<Client*>			getAllClients(void)const;
		std::deque<Client*>			getAllClientsMatching(std::string pattern)const;
		void						setHostname(std::string);
		void						clearServer(void);
		void						deleteClient(Client* client);
		void						printAllUsersFd(void);//TODO delete just debug
		void						sendMsg(const std::string msg, const int fd);
		void						sendMsg(NumericReplies reply, const int fd);
		void						sendMsg(const std::string msg, Client* client);
		void						createCmdDict(void);
		Client*						getClientByNickname(const std::string nickname) const;
		Client*						getClientByFd(size_t fd);
		bool						checkPassword(const std::string password) const;
		Channel*					getChannelByName(const std::string name);
		Channel*					addChannel(std::string name, Client* client);
		std::string					getMessageOfTheDay(void);
		void						executeCommands(char *buffer, Client* currentClient);
		std::map<std::string, Channel*> getChannels(void);
		void 							changeNicknameAsKeysInChannels(std::string oldNickname, std::string newNickname);
		void createNewChannel(int creator, std::string name);
=======
		void										execute(void);
		const std::string&							getHostname(void) const;
		void										setHostname(std::string hostname);
		const int&									getListenSocket(void) const;
		std::vector<Client*>						getAllUsers(void)const;
		Client*										getUserByNickname(std::string nickname)const;
		Client*										getClientByFd(const int fd)const;
		const std::map<std::string, time_t>&		getNicknameUnavailable(void)const;
		void										addNicknameUnavailable(std::string nick, time_t time);
		void										clearServer(void);
		void										deleteUser(Client* user);
		void										printAllUsersFd(void);//TODO delete just debug
		void										sendMsgToFd(const std::string msg, const int fd);
		void										sendMsgToFd(NumericReplies reply, const int fd);
		void										createCmdDict(void);
		Client*										findUserByNickname(const std::string nickname);
		Client*										findUserByFd(int fd);
		bool										checkPassword(const std::string password) const;
		Channel*									findChannelByName(const std::string name);
		Channel*									addChannel(std::string name, Client* user);
		void										executeCommands(char *buffer, Client* currentClient);
>>>>>>> fcfa0b2d0b945ce74a15f5d971a2282b1d8c94f8
};

#endif
