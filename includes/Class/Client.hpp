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
		int								_mode;
		int								_status;
		bool							_pwd;
		bool							_isAuthenticated;
		Client*							_lastExpediteur;
		Client*							_lastRecipient;
		Channel*						_activeChannel;
		std::map<std::string, Channel*>	_channels;
		//TODO last ping (time)
		//TODO last activity (time)
		//TODO add channels

	public:
		Client(const int fd, std::string hostname);
		~Client();
		std::string				getNickname(void)const;
		std::string				getPrefix(void)const;
		std::string				getNicknameWithPrefix(void)const;
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
		Client					*getLastExpediteur(void)const;
		Client					*getLastRecipient(void)const;
		Channel					*getActiveChannel(void)const;
		std::string				getBuffer(void)const;
		void					setNickname(std::string nickname);
		void					setUsername(std::string username);
		void					setFullName(std::string fullName);
		void					setMode(int mode);
		void					addMode(int mask);
		void					remMode(int mask);
		void					modMode(int mask, bool add);
		void					setMode(Commands command);
		void					setStatus(int status);
		void					setPwd(bool pwd);
		void					setIsAuthenticated(bool authenticate);
		void					setLastExpediteur(Client* expediteur);
		void					setLastRecipient(Client* recipient);
		void					setActiveChannel(Channel* active);
		void					addChannel(Channel* channel, std::string channelName);
		void					append(std::string buffer);
		void					clearBuffer(void);
};

#endif
