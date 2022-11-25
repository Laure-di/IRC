#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../include.hpp"

enum ClientModes
{
    NONE = 0,
    AWAY = 1,
    INVISIBLE = 2,
    RESTRICTED = 4,
    SERVEROPERATOR = 8,
	WALLOPS	= 16,
};

enum ClientStatus
{
	ALIVE = 0,
	PINGED = 1,
	DEAD = 3,
};

class Client
{
	private:
		std::string						_nickname;//unique nickname
		std::string						_username;
		std::string						_fullName;
		std::string						_buffer;
		const int						_fd;
		std::string						_hostname;
		unsigned						_mode;
		int								_status;
		bool							_pwd;
		bool							_isAuthenticated;
		std::map<std::string, Channel*>	_channels;
		std::string						_awayMessage;
		//TODO last ping (time)
		//TODO last activity (time)
		//TODO add channels

	public:
		Client(const int fd, std::string hostname);
		~Client();
		std::string				getNickname(void)const;
		std::string				getPrefix(void)const;
		std::string				getNicknameWithPrefix(Channel *channel)const;
		std::string				getUsername(void)const;
		int						getFd(void)const;
		std::string				getHostname(void)const;
		std::string 			getFullIdentifier(void) const;
		int						getStatus(void)const;
		int						getMode(void)const;
		std::string				getModeStr(void) const;
		bool					getPwd(void)const;
		bool					getIsAuthenticated(void)const;
		std::vector<Channel*>	getAllChannels(void)const;
		std::string				getBuffer(void)const;
		std::string				getAwayMessage(void) const;
		void					setNickname(std::string nickname);
		void					setUsername(std::string username);
		void					setFullName(std::string fullName);
		void					setMode(unsigned mode);
		void					addMode(unsigned mask);
		void					remMode(unsigned mask);
		void					modMode(unsigned mask, bool add);
		void					setMode(Commands command);
		void					setStatus(int status);
		void					setPwd(bool pwd);
		void					setIsAuthenticated(bool authenticate);
		void					setAwayMessage(std::string awayMessage);
		void					addChannel(Channel* channel, std::string channelName);
		void					append(std::string buffer);
		void					clearBuffer(void);
		void					removeChannel(std::string channelName);
		void					removeFromAllChannels(void);
		void					leaveChannels(Server *server);
};

#endif
