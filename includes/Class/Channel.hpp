#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "../include.hpp"

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		char _mode;
		std::map<std::string, Client*>	_usersOnChannel;
		std::map<std::string, Client*>	_userOperator;
		std::map<std::string, Client*>	_usersBanned;

	public:
		Channel(std::string name, Client* creator);
		std::string get_name(void);
		void set_name(std::string name);
		std::string get_topic(void);
		void set_topic(std::string topic);
		void clear_topic(void);
		void set_mode(Commands command);
		Client *findUserByNickname(std::string nickname);
		Client *findOperatorByNickname(std::string nickname);
		Client *findBannedUserByNickname(std::string nickname);
		void kickUser(std::string nickname);

};

#endif
