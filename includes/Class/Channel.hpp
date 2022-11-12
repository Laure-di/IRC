#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include "User.hpp"
#include "Server.hpp"

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		char _mode;
		std::map<std::string, User*>	_usersOnChannel;
		std::map<std::string, User*>	_userOperator;
		std::map<std::string, User*>	_usersBanned;

	public:
		Channel(std::string name, User* creator);
		std::string get_name(void);
		void set_name(std::string name);
		std::string get_topic(void);
		void set_topic(std::string topic);
		void clear_topic(void);
		void set_mode(Commands command);
		User *findUserByNickname(std::string nickname);
		User *findOperatorByNickname(std::string nickname);
		User *findBannedUserByNickname(std::string nickname);

};

#endif
