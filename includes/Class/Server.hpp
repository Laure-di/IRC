#ifndef SERVER_HPP
# define SERVER_HPP

#define MAX_EVENTS	20
#define BACKLOG		10
#define TIME_OUT	-1
#define BUFFER_SIZE 500000
#define HOSTNAME "mumurdelamachine.42.fr"
#define	VERSION	"1.0"
#define MOTD_FILE "conf/motd.txt"

class Server
{
	private:
		int								_port;
		std::string						_hostname;
		std::string						_version;
		std::string						_launchingDate;
		unsigned						_passwordHash;
		int								_listenSocket, _pollFd;
		epoll_event						_ep_event[MAX_EVENTS];
		std::string						_messageOftheDay;
		cmd_dict						_cmdDict;
		std::map<const int, Client*>	_clients;
		std::map<std::string, Channel*>	_channels;
		std::map<std::string, time_t>	_nicknameUnavailable;
		const std::string	_adminLogin;
		const unsigned _adminPasswordHash;

		void									_createPoll(void);
		void									_acceptNewClient(int listenSocket, int pollfd);
		int										_handleMessage(epoll_event ep_event);

	public:
		Server(int port, std::string password, char *portStr);

		unsigned								getPasswordHash(void)const;
		std::string								getLaunchingDate(void)const;
		std::string								getHostname(void)const;
		std::string								getVersion(void)const;
		std::string								getMessageOfTheDay(void);
		std::map<std::string, Channel*>			getAllChannels(void);
		Channel*								getChannelByName(const std::string name);
		Client*									getClientByFd(size_t fd);
		struct epoll_event						getEventFd(Client *client);
		Client*									getClientByNickname(const std::string nickname) const;
		std::vector<Client*>					getAllClientsMatching(std::string pattern) const;
		std::vector<Client*>					getAllClients(void)const;
		const std::map<std::string, time_t>&	getNicknameUnavailable(void)const;


		void									createNewChannel(int creator, std::string name);

		void									sendMsg(const std::string msg, const int fd);
		void									sendMsg(NumericReplies reply, const int fd);
		void									sendMsg(const std::string msg, Client *client);
		void									sendMsg(const std::string msg, std::vector<Channel*> channels);
		void									sendAllUsers(int socket);
		void									sendAllChannels(int socket);
		bool									checkPassword(const std::string password) const;
		bool									checkAdmin(const std::string login, const std::string password) const;
		void									addChannel(std::string name, Client* client);
		void									shapeMessageOftheDay(std::string fileName, int socket);
		void									executeCommands(char *buffer, Client* currentClient);
		void									changeNicknameAsKeysInChannels(std::string oldNickname, std::string newNickname);
		void									checkAndJoinChannel(int socket, std::string channelName, std::string key);
		void									checkAndLeaveChannel(int socket, std::string channelName, std::string leaveMessage);
		void									printCurrentLocaltime(int socket);
		bool									isInChannel(const std::string nickname) const;

		void									executeCommands(std::string buffer, Client *client);
		void									execute(void);

		void									createCmdDict(void);
		void									createAndBind(char *port);

		void									deleteClient(Client* user, epoll_event ep_event);
		void									clearServer(void);


		class serverError: public std::exception{
			private:
				std::string	_msg;
			public:
				serverError(const std::string type, const std::string error_msg) : _msg(type + ": " + error_msg){}
				~serverError()throw(){}
				const char* what() const throw()
				{
					return (this->_msg.c_str());
				}
	};

};


#endif
