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
		unsigned _mode;
		std::map<std::string, Client*>	_clients;
		std::map<std::string, unsigned>	_clientsMode;
		std::vector<std::string> _banMasks;
		std::vector<std::string> _banExceptionMasks;
		std::vector<std::string> _inviteMasks;
		unsigned _keyHash;
		size_t _maxLimit;

	public:
		Channel(Server *server, std::string name, Client* creator);
		std::string getName(void);
		std::string getChannelStatus(void);
		std::vector<Client*>	getAllClients(void)const;
		void setName(std::string name);
		std::string getTopic(void);
		std::map<std::string, Client*>	getClients(void)const;
		void setTopic(std::string topic);
		void clearTopic(void);
		size_t getNumberOfUsers(void);
		size_t getMaxLimitOfUsers(void);
		void setMode(Commands command);
		std::string getModeStr(void) const;
		Client *getClientByNickname(std::string nickname);
		bool checkOperatorByNickname(std::string nickname);
		bool checkVoiceByNickname(std::string nickname);
		void addClient(int socket);
		void remClient(std::string nickname);
		void sendMsg(std::string message);
		void sendMsg(NumericReplies reply);
		void sendMsg(std::string message, Client*sender);
		void sendMsg(std::string message, int fd);
		void sendJoin(Client *client);
		void sendPart(Client *Client, std::string leaveMessage);
		void sendTopic(int socket);
		void sendListOfNames(int socket);
		void changeNickname(std::string oldNickname, std::string newNickname);
		void addMode(int mask);
		void remMode(int mask);
		void modMode(int mask, bool add);
		void modKey(bool add, std::string key);
		void modLimit(bool add, std::string max);
		unsigned getMode();
		unsigned getMode(int socket);
		void modClientMode(int socket, std::string nickname, unsigned mask, bool add);
		void modClientMask(unsigned mask, bool add, std::string type);
		void sendClientMask(std::string maskName, std::vector<std::string> *masksList);
		bool isInvited(std::string nickname);
		bool isBanned(std::string nickname);
		bool isExcepted(std::string nickname);
		bool checkPassword(std::string key);
		void sendInfo(int socket);
		std::string getClientPrefix(const Client *client);
};

#endif
