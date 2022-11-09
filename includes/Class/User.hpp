#ifndef USER_HPP
# define USER_HPP

class User
{
	private:
		std::string			_nickname;//unique nickname
		std::string			_username;
		std::string			_fullName;
		const int			_fd;
		std::string			_hostname;
		int					_mode;
		int					_status;
		//TODO last ping (time)
		//TODO last activity (time)
		//TODO add channels
	
	public:
		User(const int fd, std::string hostname);
		~User();
		std::string		getNickname(void)const;
		std::string		getUsername(void)const;
		int				getFd(void)const;
		std::string		getHostname(void)const;
		int				getStatus(void)const;
		int				getMode(void)const;
		void			setNickname(std::string nickname);
		void			setUsername(std::string username);
		void			setFullName(std::string fullName);
		void			setMode(int mode);
		void			setStatus(int status);
};

#endif
