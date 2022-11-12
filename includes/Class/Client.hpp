#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <unistd.h>
#include "Server.hpp"

enum UserModes
{
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
		void			setNickname(std::string nickname);
		void			setUsername(std::string username);
		void			setFullName(std::string fullName);
		void			setMode(int mode);
		void			setMode(Commands command);
		void			setStatus(int status);
		void			setPwd(bool pwd);
};

#endif
