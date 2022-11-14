#include "../includes/include.hpp"

/*
* 3.1 Connection Registration
*/

/**
 * 3.1.1 Password message
 *
 * @brief The PASS command is used to set a 'connection password'.
 */
void pass(Server *server, int socket, Commands command) {
	if(command.params.size() == 0)
		server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	// Add check if already connected (send ERR_ALREADYREGISTRED) and check password
};

/**
 * 3.1.2 Nick message
 *
 * @brief The NICK command is used to give user a nickname or change the existing one.
 */
void nick(Server *server, int socket, Commands command) {
	if(command.params.size() == 0)
		return server->sendMsgToFd(ERR_NONICKNAMEGIVEN, socket);
	std::string nickname = command.params[0];
	if (!checkNickname(nickname))
		return server->sendMsgToFd(ERR_ERRONEUSNICKNAME(nickname), socket);
	if (server->findUserByNickname(nickname))
		return server->sendMsgToFd(ERR_NICKNAMEINUSE(nickname), socket);
	// Add check for ERR_NICKCOLLISION (nickname taken on another server)
	// Add check for ERR_UNAVAILRESOURCE (nickname in server list of forbidden nickames)
	// Add check for ERR_RESTRICTED (user mode "+r")
	Client *currentUser = server->findClientByFd(socket);
	currentUser->setNickname(nickname);
};

/**
 * 3.1.3 User message
 *
 * @brief The USER command is used at the beginning of connection to specify the username, hostname and realname of a new user.
 */
void user(Server *server, int socket, Commands command)
{
	if(command.params.size() < 4)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	Client *currentUser = server->findClientByFd(socket);
	if (!currentUser->getUsername().empty())
		return server->sendMsgToFd(ERR_ALREADYREGISTRED, socket);
	std::string userName = command.params[0];
	int mode = toInt(command.params[1]);
	std::string fullName = command.params[3];
	if (!checkUsername(userName))
		return;
	currentUser->setUsername(userName);
	currentUser->setFullName(fullName);
	currentUser->setMode(mode);
};

/**
 * 3.1.4 Oper message
 *
 * @brief A normal user uses the OPER command to obtain operator privileges.
 */
void oper(Server *server, int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	std::string password = command.params[1];
	if (!server->checkPassword(password))
		return server->sendMsgToFd(ERR_PASSWDMISMATCH, socket);
	// Add check for ERR_NOOPERHOST (client not allowed to be an operator)

}

/**
 * 3.1.6 Service message
 *
 * @brief The SERVICE command to register a new service.
 */
void service(Server *server, int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);

}


/**
 * 3.1.7 Negociation message
 *
 * @brief The SERVICE command is used for capability negotiation between a server and a client.
 */
void cap(Server *server, int socket, Commands command)
{
	return ;
}

cmd_func quit;
cmd_func squit;

/*
* 3.2 Channel operations
*/

/**
 * 3.2.1 Join message
 *
 * @brief The JOIN command is used by a user to request to start listening to
 * the specific channel.
 */
void join(Server *server, int socket, Commands command)
{
	if(command.params.empty())
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
}

/**
 * 3.2.2 Part message
 *
 * @brief The PART command causes the user sending the message to be removed
 * from the list of active members for all given channels listed in the
 *  parameter string.
 */
void part(Server *server, int socket, Commands command)
{

}

/**
 * 3.1.5 User mode message
 * 3.2.3 Channel mode message
 *
 * @brief The user MODE's are typically changes which affect either how the
 * client is seen by others or what 'extra' messages the client is sent.
 * The channel MODE command is provided so that users may query and change the
 * characteristics of a channel.
 *
 * MODE follows IRSSI documentation, particularly the fact that if the target
 * nickname or channel is omitted, the active nickname or channel will be used.
 * https://irssi.org/documentation/help/mode/
 */
void mode(Server *server, int socket, Commands command)
{
	Client *client = NULL;
	Channel *channel = NULL;
	std::string flags;
	std::string params;
	if(command.params.size() == 0)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	if(command.params[0][0] == '+' || command.params[0][0] == '-')
	{
		client = server->findClientByFd(socket);
		channel = client->getActiveChannel();
		flags = command.params[0];
		if(command.params.size() > 1)
			params = command.params[0];
	}
	else
	{
		if (checkChannelName(name))
		{
			channel = server->findChannelByName(name);
			if (!channel)
				return server->sendMsgToFd(ERR_NOSUCHCHANNEL(name), socket);
			// if (command.params.size() == 0)
			// 	return server->sendMsgToFd(channel->getModeStr(), socket);
		}
		else
		{
			client = server->getUserByFd(socket);
			if (client->getNickname() != name)
				return server->sendMsgToFd(ERR_USERSDONTMATCH, socket);
			if (command.params.size() == 1)
				return server->sendMsgToFd("Your user mode is " + client->getModeStr(), socket);
		}
		flags = command.params[1];
		if(command.params.size() > 2)
			params = command.params[2];
	}
	applyModeChanges(server, socket, flags, params, client, channel);
}

/**
 * 3.2.4 Topic message
 *
 * @brief The TOPIC command is used to change or view the topic of a channel.
 */
void topic(Server *server, int socket, Commands command)
{
	if(command.params.size() < 1)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string channelName = command.params[0];
	Channel* channel = server->findChannelByName(channelName);
	if (!channel)
		return;
	Client *currentUser = server->findClientByFd(socket);
	if (channel->findClientByNickname(currentUser->getNickname()))
		return server->sendMsgToFd(ERR_NOTONCHANNEL(channel->getName()), socket);
	if(command.params.size() == 1) {
		if (channel->getTopic().empty())
			return server->sendMsgToFd(RPL_NOTOPIC(channel->getName()), socket);
		return server->sendMsgToFd(RPL_TOPIC(channel->getName(), channel->getTopic()), socket);
	}
	if (channel->findOperatorByNickname(currentUser->getNickname()))
		return server->sendMsgToFd(ERR_CHANOPRIVSNEEDED(channel->getName()), socket);
	// Add check for ERR_NOCHANMODES
	std::string topic = command.params[1];
	if (topic == ":")
		return channel->clearTopic();
	channel->setTopic(topic);
}

/**
 * 3.2.5 Names message
 *
 * @brief By using the NAMES command, a user can list all nicknames that are
 * visible to him.
 */
void names(Server *server, int socket, Commands command)
{

}

/**
 * 3.2.6 List message
 *
 * @brief The list command is used to list channels and their topics.
 */
void list(Server *server, int socket, Commands command)
{
	std::map<std::string, Channel*> channels = server->getChannels();
	std::map<std::string, Channel*>::iterator channelsIterator;
	for (channelsIterator = channels.begin(); channelsIterator != channels.end(); channelsIterator++)
	{
		Channel *channel = channelsIterator->second;
		std::string name = channel->getName();
		std::string topic = channel->getTopic();
		if (topic.empty())
			topic = toString(channel->getNumberOfUsers());
		server->sendMsgToFd(name + " " + topic + "\n", socket);
	}
}

/**
 * 3.2.7 Invite message
 *
 * @brief The INVITE command is used to invite a user to a channel.
 */
void invite(Server *server, int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string nickname = command.params[0];
	std::string channelName = command.params[1];

	Client *invitedUser = server->findUserByNickname(nickname);
	if (!invitedUser)
		return server->sendMsgToFd(ERR_NOSUCHNICK(nickname), socket);
	Channel *channel = server->findChannelByName(channelName);
	if (!channel)
		return server->sendMsgToFd(ERR_NOSUCHNICK(channelName), socket);
	Client *currentUser = server->findClientByFd(socket);
	if (!channel->findClientByNickname(currentUser->getNickname()))
		return server->sendMsgToFd(ERR_NOTONCHANNEL(channelName), socket);
	if (channel->findClientByNickname(nickname))
		return server->sendMsgToFd(ERR_USERONCHANNEL(nickname, channelName), socket);
	// Add check for ERR_CHANOPRIVSNEEDED (channel is invite only and currentUser is not an operator)
	// Add check for RPL_AWAY (Check that invitedUser is away and notify currentUser ? Not defined)
	server->sendMsgToFd(RPL_INVITING(nickname, channelName), invitedUser->getFd());
}

/**
 * 3.2.8 Kick command
 *
 * @brief The KICK command can be used to request the forced removal of a user
 * from a channel.
 *
 */
void kick(Server *server, int socket, Commands command)
{
	// Add check for ERR_BADCHANMASK ???
	// Should an operator be able to kick another operator ? I supposed so
	if(command.params.size() < 2)
		return server->sendMsgToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string> channels = splitComma(command.params[0]);
	std::vector<std::string> users = splitComma(command.params[1]);
	Client *currentUser = server->findClientByFd(socket);
	std::string kickMessage;
	if (channels.size() == 1 && users.size() == 1)
	{
		if (command.params.size() > 2)
			kickMessage = command.params[0];
		else
			kickMessage = currentUser->getNickname();
	}
	std::vector<std::string>::iterator channelsIterator;
	for (channelsIterator = channels.begin(); channelsIterator != channels.end(); channelsIterator++)
	{
		std::string channelName = *channelsIterator;
		Channel *channel = server->findChannelByName(channelName);
		if (!channel)
		{
			server->sendMsgToFd(ERR_NOSUCHCHANNEL(channelName), socket);
			continue;
		}
		if (!channel->findOperatorByNickname(currentUser->getUsername()))
		{
			server->sendMsgToFd(ERR_CHANOPRIVSNEEDED(channelName), socket);
			continue;
		}
		std::vector<std::string>::iterator usersIterator;
		for (usersIterator = users.begin(); usersIterator != users.end(); usersIterator++)
		{
			std::string userName = *usersIterator;
			Client *user = server->findUserByNickname(userName);
			if (!user) {
				server->sendMsgToFd(ERR_USERNOTINCHANNEL(userName, channelName), socket);
				continue;
			}
			if (!kickMessage.empty())
				server->sendMsgToFd("KICK " + channelName + " " + userName + " :" + kickMessage, socket);
			channel->kickClient(userName);
			channel->kickOperator(userName);
		}
	}
}

/*
* 3.3 Sending messages
*/

/**
 * 3.3.1 Private messages
 *
 * @brief PRIVMSG is used to send private messages between users, as well as to
 * send messages to channels.
 */
void privmsg(Server *server, int socket, Commands command)
{
	return;
}

/**
 * 3.3.2 Notice
 *
 * @brief The NOTICE command is used similarly to PRIVMSG.
 */
void notice(Server *server, int socket, Commands command)
{
	return;
}

/*
* 3.4 Server queries and commands
*/

/**
 * 3.4.1 Motd message
 *
 * @brief The MOTD command is used to get the "Message Of The Day" of the given
 *  server, or current server if <target> is omitted.
 */
void motd(Server *server, int socket, Commands command)
{
	// Reminder to create a message of the day when constructing the Server
	std::string messageOfTheDay = server->getMessageOfTheDay();
	if (messageOfTheDay.empty())
		return server->sendMsgToFd(ERR_NOMOTD, socket);
	server->sendMsgToFd(RPL_MOTDSTART(server->getHostname()), socket);
	server->sendMsgToFd(RPL_MOTD(messageOfTheDay), socket);
	server->sendMsgToFd(RPL_ENDOFMOTD, socket);
}

/**
 * 3.4.2 Lusers message
 *
 * @brief The LUSERS command is used to get statistics about the size of the
 * IRC network.
 */
void lusers(Server *server, int socket, Commands command)
{
	return;
}

cmd_func lusers;
cmd_func version;
cmd_func stats;
cmd_func links;
cmd_func time;
cmd_func connect;
cmd_func trace;
cmd_func admin;
cmd_func info;

/*
* 3.5 Service Query and Commands
*/
cmd_func servlist;
cmd_func squery;

/*
* 3.6 User based queries
*/

/**
 * 3.6.1 Who query
 *
 * @brief This command is used to query a list of users who match the provided mask.
 */
void who(Server *server, int socket, Commands command)
{
	if(command.params.size() == 0)
		return printWho(server, socket, server->getAllClients());
	std::string params = command.params[0];
	if(checkChannelName(params))
	{
		Channel *channel = server->findChannelByName(params);
		if (channel)
			return printWho(server, socket, channel->getAllClients());
		return;
	}
	return printWho(server, socket, server->getAllClientsMatching(params));
}

cmd_func whois;
cmd_func whowas;

/*
* 3.7 Miscellaneous messages
*/
cmd_func kill;
cmd_func ping;
cmd_func pong;
cmd_func error;

/*
* 4.1 Optional features
*/
cmd_func away;
cmd_func rehash;
cmd_func die;
cmd_func restart;
cmd_func summon;
cmd_func users;
cmd_func wallops;
cmd_func userhost;
cmd_func ison;
