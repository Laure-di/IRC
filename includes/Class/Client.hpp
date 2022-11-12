#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../include.hpp"

enum UserModes
{
	None = 0,
    Away = 1,
    Invisible = 2,
    Wallops = 4,
    Restricted = 8,
    Operator = 16,
    LocalOperator = 32,
    ServerNotices = 64
};

class Client
{
	private:
		std::string			_nickname;//unique nickname
		std::string			_username;
		std::string			_fullName;
		const int			_fd;
		std::string			_hostname;
		int					_mode;
		int					_status;
		bool				_pwd;
		Client*				_lastExpediteur;
		Client*				_lastRecipient;
		Channel*			_activeChannel;
		//TODO last ping (time)
		//TODO last activity (time)
		//TODO add channels

	public:
		Client(const int fd, std::string hostname);
		~Client();
		std::string		getNickname(void)const;
		std::string		getUsername(void)const;
		int				getFd(void)const;
		std::string		getHostname(void)const;
		int				getStatus(void)const;
		int				getMode(void)const;
		bool			getPwd(void)const;
		Client			*getLastExpediteur(void)const;
		Client			*getLastRecipient(void)const;
		Channel			*getActiveChannel(void)const;
		void			setNickname(std::string nickname);
		void			setUsername(std::string username);
		void			setFullName(std::string fullName);
		void			setMode(int mode);
		void			setMode(Commands command);
		void			setStatus(int status);
		void			setPwd(bool pwd);
		void			setLastExpediteur(Client* expediteur);
		void			setLastRecipient(Client* recipient);
		void			setActiveChannel(Channel* active);
};

#endif
