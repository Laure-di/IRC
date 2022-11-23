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

	if(command.params.empty() || command.params[0].empty())
		return (server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket));
	Client *client = server->getClientByFd(socket);
	if (client != NULL)
	{
		if (!canRegisterPass(client) || client->getPwd())
			return (server->sendMsg(ERR_ALREADYREGISTRED, socket));
		if (server->checkPassword(command.params[0]))
		{
			client->setPwd(true);
			return ;
		}
#ifdef DEBUG
		server->sendMsg("The password doesn't match the server password\r\n", socket);
#endif
	}
};

/**
 * 3.1.2 Nick message
 *
 * @brief The NICK command is used to give user a nickname or change the existing one.
 */
void nick(Server *server, int socket, Commands command) {

	Client *client = server->getClientByFd(socket);
	if (client->getPwd() == false)
		return server->sendMsg(ERR_CLIENT(std::string("NICK: You must connect with a password first\r\n")), socket);
	if(command.params.empty() || command.params[0].empty())
		return server->sendMsg(ERR_NONICKNAMEGIVEN, socket);
	std::string nickname = command.params[0];
	if (client->getNickname() == nickname)
		return server->sendMsg("NICK " + nickname + "\r\n", socket);
	if (!checkNickname(nickname) || command.colon == true)
		return server->sendMsg(ERR_ERRONEUSNICKNAME(nickname), socket);
	if (server->getClientByNickname(nickname))
		return server->sendMsg(ERR_NICKNAMEINUSE(nickname), socket);
	if (isUnavailableNickname(server, nickname))
		return server->sendMsg(ERR_UNAVAILRESOURCE(nickname), socket);
	if (client->getMode() == RESTRICTED)
		return server->sendMsg(ERR_RESTRICTED, socket);
	if (client->getNickname().empty())
	{
		client->setNickname(nickname);
		welcomeClient(server, socket, client);
	}
	else
	{
		client->setNickname(nickname);
		return server->sendMsg("NICK " + nickname + "\r\n", socket);
	}
};

/**
 * 3.1.3 User message
 *
 * @brief The USER command is used at the beginning of connection to specify the username, hostname and realname of a new user.
 */
void user(Server *server, int socket, Commands command)
{

	Client *currentUser = server->getClientByFd(socket);
	if (currentUser->getPwd() == false)
		return server->sendMsg(ERR_CLIENT(std::string("USER: You must connect with a password first\r\n")), socket);
	if (!currentUser->getUsername().empty())
		return server->sendMsg(ERR_ALREADYREGISTRED, socket);
	if(command.params.size() < 4)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	int mode;
	if ((mode = areParamsValid(command.params)) == -1)
		return server->sendMsg(ERR_CLIENT((std::string)"USER : you are not using the correct syntax"), socket);
	std::string userName = command.params[0];
	std::string fullName = command.params[3];
	if (!checkUsername(userName))
		return;
	currentUser->setUsername(userName);
	currentUser->setFullName(fullName);
	if (-1 < mode)
		currentUser->setMode(mode);
	welcomeClient(server, socket, currentUser);
};

/**
 * 3.1.4 Oper message
 *
 * @brief A normal user uses the OPER command to obtain operator privileges.
 */
void oper(Server *server, int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	std::string password = command.params[1];
	if (!server->checkAdmin(name, password))
		return server->sendMsg(ERR_PASSWDMISMATCH, socket);
	// TODO Add check for ERR_NOOPERHOST (client not allowed to be an operator)
	Client *client = server->getClientByFd(socket);
	client->addMode(SERVEROPERATOR);
	server->sendMsg(RPL_YOUREOPER, socket);
}

/**
 * 3.1.6 Service message
 *
 * @brief The SERVICE command to register a new service.
 */
void service(Server *server, int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);

}


/**
 * 3.1.7 Negociation message
 *
 * @brief The SERVICE command is used for capability negotiation between a server and a client.
 */
void cap(Server *server, int socket, Commands command)
{
	(void)server;
	(void)socket;
	(void)command;
	return ;
}


void	quit(Server *server, int socket, Commands command)
{
	Client					*clientQuitting = server->getClientByFd(socket);
	std::vector<Channel*>	channelsToInform = clientQuitting->getAllChannels();
	std::string				msgChannel;

	if (command.params.empty())
		server->sendMsg("QUIT\r\n", socket);
	else if (!command.params[0].empty())
		server->sendMsg("QUIT : " + command.params[0] + "\r\n", socket);
	server->deleteClient(clientQuitting->getFd());
	if (!command.params.empty())
		msgChannel = "QUIT : " + command.params[0] + "\r\n\r\n";
	else
		msgChannel = "QUIT\r\n";
	if (!channelsToInform.empty())
	{
		std::cout << msgChannel << std::endl;
		server->sendMsg(msgChannel, channelsToInform);
	}
}

//TODO a tester
void	squit(Server *server, int socket, Commands command)
{
	Client *client = server->getClientByFd(socket);
//	if (!(client->getMode() & SERVEROPERATOR))
//		return (server->sendMsg(ERR_NOPRIVILEGES, socket));
	if (command.params.size() != 2)
		return (server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket));
	std::cout << "Hostname " << server->getHostname() << std::endl;
	if (server->getHostname().compare(command.params[0]) != 0)
		return (server->sendMsg(ERR_NOSUCHSERVER(command.params[0]), socket));
	std::string msg = command.params[1];
	server->broadcast(msg, socket);
	throw Server::serverError(client->getNickname(), " close the server because of :" + msg);
}


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
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string> names = splitComma(command.params[0]);
	std::vector<std::string> keys;
	if (command.params.size() > 1)
		keys = splitComma(command.params[1]);
	for (size_t i = 0; i < names.size(); i++) {
		std::string key;
		if (i < keys.size())
			key = keys[i];
		server->checkAndJoinChannel(socket, names[i], key);
	}
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
	if(command.params.empty())
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string> names = splitComma(command.params[0]);
	std::vector<std::string>::iterator name;
	std::string leaveMessage;
	if(command.params.size() > 1)
		leaveMessage = command.params[1];
	for (name = names.begin(); name != names.end(); name++)
		server->checkAndLeaveChannel(socket, *name, leaveMessage);
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
	if(command.params.size() == 0)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	std::string flags;
	if(command.params.size() > 1)
		flags = command.params[1];
	if (checkChannelName(name))
	{
		channel = server->getChannelByName(name);
		if (!channel)
			return server->sendMsg(ERR_NOSUCHCHANNEL(name), socket);
		if (command.params.size() == 1)
			return server->sendMsg("The channel " + channel->getName() + " mode is " + channel->getModeStr() + "\r\n", socket);
		std::string param;
		if(command.params.size() > 2)
			param = command.params[2];

		return applyModeChangesChannel(server, socket, flags, param, channel);
	}
	client = server->getClientByFd(socket);
	if (client->getNickname() != name)
		return server->sendMsg(ERR_USERSDONTMATCH, socket);
	if (command.params.size() == 1)
		return server->sendMsg("Your user mode is " + client->getModeStr() + "\r\n", socket);
	applyModeChangesClient(server, socket, flags, client);
}

/**
 * 3.2.4 Topic message
 *
 * @brief The TOPIC command is used to change or view the topic of a channel.
 */
void topic(Server *server, int socket, Commands command)
{
	if(command.params.size() < 1)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string channelName = command.params[0];
	Channel* channel = server->getChannelByName(channelName);
	if (!channel)
		return;
	Client *currentUser = server->getClientByFd(socket);
	std::string nickname = currentUser->getNickname();
	if (channel->getClientByNickname(nickname))
		return server->sendMsg(ERR_NOTONCHANNEL(channel->getName()), socket);
	if (command.params.size() == 1 && !command.colon) {
		if (channel->getTopic().empty())
			return server->sendMsg(RPL_NOTOPIC(channel->getName()), socket);
		return server->sendMsg(RPL_TOPIC(channel->getName(), channel->getTopic()), socket);
	}
	if ((channel->getMode() & TOPIC) && !(channel->checkOperatorByNickname(nickname)))
		return server->sendMsg(ERR_CHANOPRIVSNEEDED(channel->getName()), socket);
	// Add check for ERR_NOCHANMODES
	std::string topic = command.params[1];
	if (command.params.size() == 1 && command.colon)
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
	if(!command.params.size())
		return server->sendAllUsers(socket);
	std::vector<std::string> channelNames = splitComma(command.params[0]);
	std::vector<std::string>::const_iterator cit;
	for (cit = channelNames.begin(); cit != channelNames.end(); cit++)
	{
		std::string channelName = *cit;
		Channel *channel = server->getChannelByName(channelName);
		if (channel)
			channel->sendListOfNames(socket);
		else
			server->sendMsg(RPL_ENDOFNAMES(channelName), socket);
	}
}

/**
 * 3.2.6 List message
 *
 * @brief The list command is used to list channels and their topics.
 */
void list(Server *server, int socket, Commands command)
{
	if(!command.params.size())
		return server->sendAllChannels(socket);
	std::vector<std::string> channelNames = splitComma(command.params[0]);
	std::vector<std::string>::const_iterator cit;
	for (cit = channelNames.begin(); cit != channelNames.end(); cit++)
	{
		Channel *channel = server->getChannelByName(*cit);
		if (channel && !(channel->getMode() & SECRET))
			channel->sendInfo(socket);
	}
}

/**
 * 3.2.7 Invite message
 *
 * @brief The INVITE command is used to invite a user to a channel.
 */
void invite(Server *server, const int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string nickname = command.params[0];
	std::string channelName = command.params[1];
	Client *invitedClient = server->getClientByNickname(nickname);
	if (!invitedClient)
		return server->sendMsg(ERR_NOSUCHNICK(nickname), socket);
	Channel *channel = server->getChannelByName(channelName);
	if (!channel)
		return server->sendMsg(ERR_NOSUCHNICK(channelName), socket);
	Client *client = server->getClientByFd(socket);
	if (!channel->getClientByNickname(client->getNickname()))
		return server->sendMsg(ERR_NOTONCHANNEL(channelName), socket);
	if (channel->getClientByNickname(nickname))
		return server->sendMsg(ERR_USERONCHANNEL(nickname, channelName), socket);
	if ((channel->getMode() & INVITATION) && !channel->checkOperatorByNickname(nickname))
		return server->sendMsg(ERR_CHANOPRIVSNEEDED(channelName), socket);
	if (invitedClient->getMode() & AWAY)
		return server->sendMsg(RPL_AWAY(nickname, invitedClient->getAwayMessage()), socket);
	server->sendMsg(RPL_INVITING(nickname, channelName), invitedClient->getFd());
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
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string> channels = splitComma(command.params[0]);
	std::vector<std::string> users = splitComma(command.params[1]);
	Client *currentClient = server->getClientByFd(socket);
	std::string nickname = currentClient->getNickname();
	std::string kickMessage;
	if (channels.size() == 1 && users.size() == 1)
	{
		if (command.params.size() > 2)
			kickMessage = command.params[2];
		else
			kickMessage = nickname;
	}
	std::vector<std::string>::iterator channelsIterator;
	for (channelsIterator = channels.begin(); channelsIterator != channels.end(); channelsIterator++)
	{
		std::string channelName = *channelsIterator;
		Channel *channel = server->getChannelByName(channelName);
		if (!channel)
		{
			server->sendMsg(ERR_NOSUCHCHANNEL(channelName), socket);
			continue;
		}
		if (!channel->checkOperatorByNickname(nickname))
		{
			server->sendMsg(ERR_CHANOPRIVSNEEDED(channelName), socket);
			continue;
		}
		std::vector<std::string>::const_iterator cit;
		for (cit = users.begin(); cit != users.end(); cit++)
		{
			std::string clientName = *cit;
			Client *client = server->getClientByNickname(clientName);
			if (!client) {
				server->sendMsg(ERR_USERNOTINCHANNEL(clientName, channelName), socket);
				continue;
			}
			channel->sendMsg("KICK " + channelName + " " + clientName + " :" + kickMessage + "\r\n");
			channel->remClient(clientName);
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
	if(command.params.size() == 0)
		return server->sendMsg(ERR_NORECIPIENT(command.command), socket);
	if(command.params.size() == 1)
		return server->sendMsg(ERR_NOTEXTTOSEND, socket);
	std::vector<std::string> recipients = splitComma(command.params[0]);
	Client *client = server->getClientByFd(socket);
	std::vector<std::string>::iterator it;
	for (it = recipients.begin(); it != recipients.end(); it++)
	{
		std::string name = *it;
		std::string msg = ":" + client->getFullIdentifier() + " " + command.command + " " + name + " :" + command.params[1] + "\r\n";
		if (checkChannelName(name))
		{
			Channel *channel = server->getChannelByName(name);
			if (channel)
				channel->sendMsg(msg, client);
		}
		else
		{
			Client *recipient = server->getClientByNickname(name);
			if (recipient)
			{
				if (recipient->getMode() & AWAY)
					server->sendMsg(RPL_AWAY(name, recipient->getAwayMessage()), socket);
				server->sendMsg(msg, recipient);
			}
		}
	}
	return;
}

/**
 * 3.3.2 Notice
 *
 * @brief "The NOTICE command is used similarly to PRIVMSG."
 */
void notice(Server *server, int socket, Commands command)
{
	privmsg(server, socket, command);
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
	(void) command;
	try
	{
		server->sendMsg(RPL_MOTDSTART(server->getClientByFd(socket)->getHostname()), socket);
		server->shapeMessageOftheDay(server->getMessageOfTheDay(), socket);
		server->sendMsg(RPL_ENDOFMOTD, socket);
	}
	catch (numericRepliesError &e)
	{
		return (e.sendReply(ERR_NOMOTD, socket, server));
	}

}

cmd_func lusers;
cmd_func version;
cmd_func stats;
cmd_func links;

void	time(Server *server, int socket, Commands command)
{
	(void) command;
	server->printCurrentLocaltime(socket);
}

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
		Channel *channel = server->getChannelByName(params);
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

/*
 * 3.7.2 Ping message
 *	@Brief The PING command is used to test the presence of an active client or
 *		   server at the other end of the connection.
 *		   https://www.digitalocean.com/community/tutorials/compare-strings-in-c-plus-plus
 */

void	ping(Server *server, int socket, Commands command)
{
	if (command.params.size() == 0)
		return server->sendMsg(ERR_NOORIGIN, socket);
	if (1 <= command.params.size())
	{
		std::string expediteur = command.params[0];
		if (command.params.size() == 2)
		{
			std::string serverRecipient = command.params[1];
			std::cout << server->getHostname() << std::endl;
			if (server->getHostname().compare(serverRecipient) != 0)
				return server->sendMsg(ERR_NOSUCHSERVER(serverRecipient), socket);
		}
		return server->sendMsg(PONG(server->getHostname()), socket);
	}

}

/*
 * 3.7.3 Pong message
 *	@Brief PONG message is a reply to ping message.
 */
void	pong(Server *server, int socket, Commands command)
{
	if (command.params.size() == 0)
		return server->sendMsg(ERR_NOORIGIN, socket);
	if (1 <= command.params.size())
	{
		std::string expediteur = command.params[0];
		if (command.params.size() == 2)
		{
			std::string serverRecipient = command.params[1];
			std::cout << server->getHostname() << std::endl;
			if (server->getHostname().compare(serverRecipient) != 0)
				return server->sendMsg(ERR_NOSUCHSERVER(serverRecipient), socket);
		}
		Client *client = server->getClientByFd(socket);
		if (client)
			client->setStatus(ALIVE);
	}

}

cmd_func error;

/*
 * 4.1 Optional features
 */
void away(Server *server, int socket, Commands command)
{
	Client *client = server->getClientByFd(socket);
	std::string nickname = client->getNickname();
	if(command.params.size() == 0)
	{
		client->remMode(AWAY);
		return server->sendMsg(RPL_NOWAWAY, socket);
	}
	client->addMode(AWAY);
	client->setAwayMessage(command.params[0]);
	server->sendMsg(RPL_UNAWAY, socket);
}

cmd_func rehash;
cmd_func die;
cmd_func restart;
cmd_func summon;
cmd_func users;
cmd_func wallops;
cmd_func userhost;
cmd_func ison;
