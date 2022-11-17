#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "../include.hpp"

class Channel
{
	private:
		Server *_server;
		std::string _name;
		std::string _topic;
		char _mode;
		std::map<std::string, Client*>	_clientsOnChannel;
		std::map<std::string, Client*>	_clientsOperator;
		std::map<std::string, Client*>	_clientsBanned;
		std::vector<std::string> _banMasks;
		std::vector<std::string> _banExceptionMasks;
		std::vector<std::string> _inviteMasks;

	public:
		Channel(Server *server, std::string name, Client* creator);
		std::string getName(void);
		void setName(std::string name);
		std::string getTopic(void);
		void setTopic(std::string topic);
		void clearTopic(void);
		size_t getNumberOfUsers(void);
		void setMode(Commands command);
		// std::string getModeStr(void);
		Client *findClientByNickname(std::string nickname);
		Client *findOperatorByNickname(std::string nickname);
		Client *findBannedUserByNickname(std::string nickname);
		void addClient(int socket);
		void deleteClient(std::string nickname);
		void sendMsg(std::string message);
		void sendJoin(std::string fullClientName);
		void sendPart(std::string fullClientName, std::string nickname, std::string leaveMessage);
		void sendTopic(int socket);
		void sendListOfNames(int socket);
		std::vector<Client*>			getAllClients(void)const;
		void changeNickname(std::string oldNickname, std::string newNickname);
};

#endif
