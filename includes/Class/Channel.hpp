#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "../include.hpp"

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		char _mode;
		std::map<std::string, Client*>	_clientsOnChannel;
		std::map<std::string, Client*>	_clientsOperator;
		std::map<std::string, Client*>	_clientsBanned;

	public:
		Channel(std::string name, Client* creator);
		std::string getName(void);
		void setName(std::string name);
		std::string getTopic(void);
		void setTopic(std::string topic);
		void clearTopic(void);
		void setMode(Commands command);
		std::string getModeStr(void);
		Client *findClientByNickname(std::string nickname);
		Client *findOperatorByNickname(std::string nickname);
		Client *findBannedUserByNickname(std::string nickname);
		void kickClient(std::string nickname);
		void kickOperator(std::string nickname);
		void sendMsg(std::string message);

};

#endif
