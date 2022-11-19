#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "../include.hpp"

enum ChannelClientModes
{
	VOICE = 1,
	OPERATOR = 2,
	CREATOR = 4,
};

enum ChannelModes
{
	INVITATION = 1,
	SECRET = 2, //
	KEY = 4,
	MODERATED = 8, //
	OUTSIDE = 16,
	PRIVATE = 32, //
	TOPIC = 64,
	LIMIT = 128,
};

class Channel
{
	private:
		Server *_server;
		std::string _name;
		std::string _topic;
		unsigned char _mode;
		std::map<std::string, Client*>	_clients;
		std::map<std::string, Client*>	_clientsBanned;
		std::map<std::string, unsigned char>	_clientsMode;
		std::vector<std::string> _banMasks;
		std::vector<std::string> _banExceptionMasks;
		std::vector<std::string> _inviteMasks;
		unsigned _keyHash;
		size_t _maxLimit;

	public:
		Channel(Server *server, std::string name, Client* creator);
		std::string getName(void);
		void setName(std::string name);
		std::string getTopic(void);
		void setTopic(std::string topic);
		void clearTopic(void);
		size_t getNumberOfUsers(void);
		size_t getMaxLimitOfUsers(void);
		void setMode(Commands command);
		std::string getModeStr(void) const;
		Client *findClientByNickname(std::string nickname);
		bool checkOperatorByNickname(std::string nickname);
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
		void addMode(int mask);
		void remMode(int mask);
		void modMode(int mask, bool add);
		void modKey(bool add, std::string key);
		void modLimit(bool add, std::string max);
		unsigned char getMode();
		unsigned char getMode(int socket);
		void modClientMode(int socket, std::string nickname, unsigned char mask, bool add);
		void modClientMask(unsigned char mask, bool add, std::string type);
		bool isInvited(std::string nickname);
		bool isBanned(std::string nickname);
		bool isExcepted(std::string nickname);
		bool checkPassword(std::string key);
		void sendInfo(int socket);
};

#endif
