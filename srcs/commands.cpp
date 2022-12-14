#include "../includes/include.hpp"

/*
 * List of commands with their reference in the RFC 2812
 * https://datatracker.ietf.org/doc/html/rfc2812
 */

/*
 * 3.1 Connection Registration
 */

/**
 * 3.1.1 Password message
 *
 * @brief The PASS command is used to set a 'connection password'.
 */
void pass(Server *server, int socket, Commands command)
{
	if (command.params.empty() || command.params[0].empty())
		return (server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket));
	Client *client = server->getClientByFd(socket);
	if (client != NULL)
	{
		if (client->getPwd())
			return (server->sendMsg(ERR_ALREADYREGISTRED, socket));
		if (server->checkPassword(command.params[0]))
		{
			client->setPwd(true);
			return;
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
void nick(Server *server, int socket, Commands command)
{
	Client *client = server->getClientByFd(socket);
	if (client->getPwd() == false)
	{
		server->sendMsg(ERR_CLIENT(std::string("NICK: You must connect with a password first")), socket);
		server->deleteClient(socket);
		return;
	}
	if (command.params.empty() || command.params[0].empty())
		return server->sendMsg(ERR_NONICKNAMEGIVEN, socket);
	std::string nickname = command.params[0];
	if (client->getNickname() == nickname)
		return server->sendMsg("NICK " + nickname + "\r\n", socket);
	if (!checkNickname(nickname) || command.colon == true)
		return server->sendMsg(ERR_ERRONEUSNICKNAME(nickname), socket);
	if (server->getClientByNickname(nickname))
	{
		server->sendMsg(ERR_NICKNAMEINUSE(nickname), socket);
		server->deleteClient(socket);
		return;
	}
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
		server->sendMsg("NICK " + nickname + "\r\n", socket);
	}
};

/**
 * 3.1.3 User message
 *
 * @brief The USER command is used at the beginning of connection to specify the username, hostname and realname of a
 * new user.
 */
void user(Server *server, int socket, Commands command)
{
	Client *client = server->getClientByFd(socket);
	if (client->getPwd() == false)
		return server->sendMsg(ERR_CLIENT(std::string("USER: You must connect with a password first")), socket);
	if (!client->getUsername().empty())
		return server->sendMsg(ERR_ALREADYREGISTRED, socket);
	if (command.params.size() < 4)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	int mode;
	if ((mode = areParamsValid(command.params)) == -1)
		return server->sendMsg(ERR_CLIENT((std::string) "USER : you are not using the correct syntax"), socket);
	std::string userName = command.params[0];
	std::string fullName = command.params[3];
	if (!checkUsername(userName))
		return;
	client->setUsername(userName);
	client->setFullName(fullName);
	if (-1 < mode)
		client->setMode(mode & 0b110);
	welcomeClient(server, socket, client);
};

/**
 * 3.1.4 Oper message
 *
 * @brief A normal user uses the OPER command to obtain operator privileges.
 */
void oper(Server *server, int socket, Commands command)
{
	if (command.params.size() < 2)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	std::string password = command.params[1];
	if (!server->checkAdmin(name, password))
		return server->sendMsg(ERR_PASSWDMISMATCH, socket);
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
	if (command.params.size() < 2)
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
	return;
}

/**
 * 3.1.7 Quit
 *
 * @brief A client session is terminated with a quit message.
 */
void quit(Server *server, int socket, Commands command)
{
	Client				*clientQuitting = server->getClientByFd(socket);
	std::vector<Channel *> channelsToInform = clientQuitting->getAllChannels();
	std::string            msgChannel;
	std::string            nickname = clientQuitting->getNickname();

	if (command.params.empty())
		server->sendMsg(QUIT("QUIT\r\n"), socket);
	else if (!command.params[0].empty())
		server->sendMsg(QUIT("QUIT : " + command.params[0] + "\r\n"), socket);
	if (!command.params.empty())
		msgChannel = ":" + clientQuitting->getFullIdentifier() +
					 " QUIT the server with the comment : " + command.params[0] + "\r\n";
	else
		msgChannel = ":" + clientQuitting->getFullIdentifier() + " QUIT\r\n";
	server->deleteClient(clientQuitting->getFd());
	if (!channelsToInform.empty())
		server->sendMsg(msgChannel, channelsToInform);
}

/**
 * 3.1.8 Squit
 *
 * @brief The SQUIT command is available only to operators.
 */
void squit(Server *server, int socket, Commands command)
{
	Client *client = server->getClientByFd(socket);
	if (!(client->getMode() & SERVEROPERATOR))
		return (server->sendMsg(ERR_NOPRIVILEGES, socket));
	if (command.params.size() != 2)
		return (server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket));
	if (server->getHostname().compare(command.params[0]) != 0 && command.params[0].compare("127.0.0.1"))
		return (server->sendMsg(ERR_NOSUCHSERVER(command.params[0]), socket));
	std::vector<std::string> msg;
	msg.push_back("terminate the connection with the comment :" + command.params[1]);
	Commands cmd("WALLOPS", "", msg, false);
	wallops(server, socket, cmd);
	throw Server::serverError(client->getNickname(), " close the server because of :" + command.params[1]);
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
	if (command.params.empty())
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	Client *client = server->getClientByFd(socket);
	if (command.params[0] == "0")
		return client->leaveChannels(server);
	std::vector<std::string> names = splitBy(command.params[0], ",");
	std::vector<std::string> keys;
	if (command.params.size() > 1)
		keys = splitBy(command.params[1], ",");
	for (size_t i = 0; i < names.size(); i++)
	{
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
	if (command.params.empty())
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string>           names = splitBy(command.params[0], ",");
	std::vector<std::string>::iterator name;
	std::string                        leaveMessage;
	if (command.params.size() > 1)
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
	Client  *client = NULL;
	Channel *channel = NULL;
	if (command.params.size() == 0)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	std::string flags;
	if (command.params.size() > 1)
		flags = command.params[1];
	if (isChannelName(name))
	{
		channel = server->getChannelByName(name);
		if (!channel)
			return server->sendMsg(ERR_NOSUCHCHANNEL(name), socket);
		if (command.params.size() == 1)
			return server->sendMsg("The channel " + channel->getName() + " mode is " + channel->getModeStr() + "\r\n",
								   socket);
		std::string param;
		if (command.params.size() > 2)
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
	if (command.params.empty())
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string channelName = command.params[0];
	Channel    *channel = server->getChannelByName(channelName);
	if (!channel)
		return;
	Client     *currentUser = server->getClientByFd(socket);
	std::string nickname = currentUser->getNickname();
	if (!channel->getClientByNickname(nickname))
		return server->sendMsg(ERR_NOTONCHANNEL(channel->getName()), socket);
	if (command.params.size() == 1 && !command.colon)
	{
		if (channel->getTopic().empty())
			return server->sendMsg(RPL_NOTOPIC(channel->getName()), socket);
		return server->sendMsg(RPL_TOPIC(channel->getName(), channel->getTopic()), socket);
	}
	if ((channel->getMode() & TOPIC) && !(channel->checkOperatorByNickname(nickname)))
		return server->sendMsg(ERR_CHANOPRIVSNEEDED(channel->getName()), socket);
	if (command.params.size() == 1)
		channel->clearTopic();
	else
		channel->setTopic(command.params[1]);
	if (channel->getTopic().empty())
		return server->sendMsg(RPL_NOTOPIC(channel->getName()), socket);
	server->sendMsg(RPL_TOPIC(channel->getName(), channel->getTopic()), socket);
}

/**
 * 3.2.5 Names message
 *
 * @brief By using the NAMES command, a user can list all nicknames that are
 * visible to him.
 */
void names(Server *server, int socket, Commands command)
{
	if (!command.params.size())
		return server->sendAllUsers(socket);
	std::vector<std::string>                 channelNames = splitBy(command.params[0], ",");
	std::vector<std::string>::const_iterator cit;
	for (cit = channelNames.begin(); cit != channelNames.end(); cit++)
	{
		std::string channelName = *cit;
		Channel    *channel = server->getChannelByName(channelName);
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
	if (command.params.empty())
		return server->sendAllChannels(socket);
	std::vector<std::string>                 channelNames = splitBy(command.params[0], ",");
	std::vector<std::string>::const_iterator cit;
	for (cit = channelNames.begin(); cit != channelNames.end(); cit++)
	{
		Channel *channel = server->getChannelByName(*cit);
		if (channel && !(channel->getMode() & SECRET))
			channel->sendInfo(socket);
	}
	server->sendMsg(RPL_LISTEND, socket);
}

/**
 * 3.2.7 Invite message
 *
 * @brief The INVITE command is used to invite a user to a channel.
 */
void invite(Server *server, const int socket, Commands command)
{
	if (command.params.size() < 2)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string nickname = command.params[0];
	std::string channelName = command.params[1];
	Client     *invitedClient = server->getClientByNickname(nickname);
	if (!invitedClient)
		return server->sendMsg(ERR_NOSUCHNICK(nickname), socket);
	Channel *channel = server->getChannelByName(channelName);
	if (!channel)
		return server->sendMsg(ERR_NOSUCHNICK(channelName), socket);
	Client     *client = server->getClientByFd(socket);
	std::string clientNickname = client->getNickname();
	if (!channel->getClientByNickname(client->getNickname()))
		return server->sendMsg(ERR_NOTONCHANNEL(channelName), socket);
	if (channel->getClientByNickname(nickname))
		return server->sendMsg(ERR_USERONCHANNEL(nickname, channelName), socket);
	if ((channel->getMode() & INVITATION) && !channel->checkOperatorByNickname(clientNickname))
		return server->sendMsg(ERR_CHANOPRIVSNEEDED(channelName), socket);
	if (invitedClient->getMode() & AWAY)
		server->sendMsg(RPL_AWAY(nickname, invitedClient->getAwayMessage()), socket);
	server->sendMsg(RPL_INVITING(nickname, channelName), invitedClient->getFd());
	channel->modClientMask('I', true, nickname);
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
	if (command.params.size() < 2)
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string> channels = splitBy(command.params[0], ",");
	std::vector<std::string> users = splitBy(command.params[1], ",");
	Client				  *currentClient = server->getClientByFd(socket);
	std::string              nickname = currentClient->getNickname();
	std::string              fullIdentifier = currentClient->getFullIdentifier();
	std::string              kickMessage;
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
		Channel    *channel = server->getChannelByName(channelName);
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
			Client     *client = server->getClientByNickname(clientName);
			if (!client)
			{
				server->sendMsg(ERR_USERNOTINCHANNEL(clientName, channelName), socket);
				continue;
			}
			channel->sendMsg(":" + fullIdentifier + " KICK " + channelName + " " + clientName + " :" + kickMessage +
							 "\r\n");
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
	if (command.params.size() == 0)
		return server->sendMsg(ERR_NORECIPIENT(command.command), socket);
	if (command.params.size() == 1)
		return server->sendMsg(ERR_NOTEXTTOSEND, socket);
	std::vector<std::string>           recipients = splitBy(command.params[0], ",");
	Client                            *client = server->getClientByFd(socket);
	std::string                        nickname = client->getNickname();
	std::vector<std::string>::iterator it;
	for (it = recipients.begin(); it != recipients.end(); it++)
	{
		std::string name = *it;
		std::string msg = ":" + client->getFullIdentifier() + " " + command.command + " " + name + " :" +
						  command.params[1] + "\r\n";
		if (checkChannelName(name))
		{
			Channel *channel = server->getChannelByName(name);
			if (channel)
				channel->sendMsg(msg, client);
			else
				server->sendMsg(ERR_NOSUCHNICK(nickname), socket);
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
			else
				server->sendMsg(ERR_NOSUCHNICK(nickname), socket);
		}
	}
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
	(void)command;
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

/**
 * 3.4.2 Lusers message
 *
 * @brief The LUSERS command is used to get statistics about the size of the
 * IRC network.
 */
void lusers(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.3 Version message
 *
 * @brief The VERSION command is used to query the version of the server
 * program.
 */
void version(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.4 Stats message
 *
 * @brief The stats command is used to query statistics of certain server.
 */
void stats(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.5 Links message
 *
 * @brief With LINKS, a user can list all servernames, which are known by the
 * server answering the query.
 */
void links(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.6 Time message
 *
 * @brief The time command is used to query local time from the specified
 * server.
 */
void time(Server *server, int socket, Commands command)
{
	(void)command;
	server->printCurrentLocaltime(socket);
}

/**
 * 3.4.7 Connect message
 *
 * @brief The CONNECT command can be used to request a server to try to
 * establish a new connection to another server immediately.
 */
void connect(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.8 Trace message
 *
 * @brief TRACE command is used to find the route to specific server and
 * information about its peers.
 */
void trace(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.9 Admin command
 *
 * @brief  The admin command is used to find information about the administrator
 * of the given server
 */
void admin(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.4.10 Info command
 *
 * @brief The INFO command is REQUIRED to return information describing the
 * server.
 */
void info(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/*
 * 3.5 Service Query and Commands
 */

/**
 * 3.5.1 Servlist message
 *
 * @brief The SERVLIST command is used to list services currently connected to
 * the network and visible to the user issuing the command.
 */
void servlist(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 3.5.2 Squery
 *
 * @brief The SQUERY command is used similarly to PRIVMSG.  The only difference
 * is that the recipient MUST be a service.
 */
void squery(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

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
	if (command.params.size() == 0)
		return server->sendWho(socket, server->getAllClients());
	std::string params = command.params[0];
	if (checkChannelName(params))
	{
		Channel *channel = server->getChannelByName(params);
		if (channel)
			return server->sendWho(socket, channel->getAllClients());
		return;
	}
	Client               *client = server->getClientByFd(socket);
	std::vector<Client *> listOfVisibles = server->getAllClientsVisibleForClient(client);
	server->sendWho(socket, server->getAllClientsMatching(params, listOfVisibles));
}

/**
 * 3.6.2 Whois query
 *
 * @brief This command is used to query information about a particular user.
 */
void whois(Server *server, int socket, Commands command)
{
	Client *client = server->getClientByFd(socket);
	std::string nickname = client->getNickname();
	if (command.params.empty())
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<std::string> nicknameMasks = splitBy(command.params[0], ",");
	std::vector<Client *>    listOfClients = server->getAllClientsMatching(nicknameMasks);
	if (listOfClients.empty())
		return server->sendMsg(ERR_NONICKNAMEGIVEN, socket);
	std::vector<Client *>::const_iterator cit;
	for (cit = listOfClients.begin(); cit != listOfClients.end(); cit++)
	{
		Client *client = *cit;
		server->sendMsg(client->getWhoIsMessage(), socket);
	}
	server->sendMsg(RPL_ENDOFWHO(), socket);
}

/**
 * 3.6.3 Whowas
 *
 * @brief Whowas asks for information about a nickname which no longer exists.
 */
void whowas(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/*
 * 3.7 Miscellaneous messages
 */

/**
 * 3.7.1 Kill message
 *
 * @brief The KILL command is used to cause a client-server connection to be
 * closed by the server which has the actual connection.
 */

void kill(Server *server, int socket, Commands command)
{
	Client *killer = server->getClientByFd(socket);
	if (!(killer->getMode() & SERVEROPERATOR))
		return (server->sendMsg(ERR_NOPRIVILEGES, socket));
	if (command.params.size() < 2)
		return (server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket));
	std::string nickname = command.params[0];
	if (server->getHostname().compare(nickname) == 0 || nickname.compare("127.0.0.1") == 0)
		return (server->sendMsg(ERR_CANTKILLSERVER, socket));
	Client *victim = server->getClientByNickname(nickname);
	if (server->getClientByNickname(nickname) == NULL)
		return (server->sendMsg(ERR_NOSUCHNICK(nickname), socket));
	server->deleteClient(victim->getFd());
	server->addNicknameUnavailable(nickname);
}

/**
 * 3.7.2 Ping message
 *
 * @brief The PING command is used to test the presence of an active client or
 * server at the other end of the connection.
 * https://www.digitalocean.com/community/tutorials/compare-strings-in-c-plus-plus
 */
void ping(Server *server, int socket, Commands command)
{
	if (command.params.size() == 0)
		return server->sendMsg(ERR_NOORIGIN, socket);
	if (1 <= command.params.size())
	{
		std::string expediteur = command.params[0];
		if (command.params.size() == 2)
		{
			std::string serverRecipient = command.params[1];
			if (server->getHostname().compare(serverRecipient) != 0 && serverRecipient != "127.0.0.1")
				return server->sendMsg(ERR_NOSUCHSERVER(serverRecipient), socket);
		}
		return server->sendMsg(PONG(server->getHostname()), socket);
	}
}

/**
 * 3.7.3 Pong message
 *
 * @brief PONG message is a reply to ping message.
 */
void pong(Server *server, int socket, Commands command)
{
	if (command.params.size() == 0)
		return server->sendMsg(ERR_NOORIGIN, socket);
	if (1 <= command.params.size())
	{
		std::string expediteur = command.params[0];
		if (command.params.size() == 2)
		{
			std::string serverRecipient = command.params[1];
			if (server->getHostname().compare(serverRecipient) != 0 && serverRecipient != "127.0.0.1")
				return server->sendMsg(ERR_NOSUCHSERVER(serverRecipient), socket);
		}
		Client *client = server->getClientByFd(socket);
		if (client)
			client->setStatus(ALIVE);
	}
}

/**
 * 3.7.4 Error
 *
 * @brief The ERROR command is for use by servers when reporting a serious or
 * fatal error to its peers.
 */
void error(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/*
 * 4.Optional features
 */

/**
 * 4.1 Away
 *
 * @brief  With the AWAY command, clients can set an automatic reply string for
 * any PRIVMSG commands directed at them (not to a channel they are on).
 */
void away(Server *server, int socket, Commands command)
{
	Client     *client = server->getClientByFd(socket);
	std::string nickname = client->getNickname();
	if (command.params.size() == 0)
	{
		client->remMode(AWAY);
		return server->sendMsg(RPL_UNAWAY, socket);
	}
	client->addMode(AWAY);
	client->setAwayMessage(command.params[0]);
	server->sendMsg(RPL_NOWAWAY, socket);
}

/**
 * 4.2 Rehash message
 *
 * @brief The rehash command is an administrative command which can be used by
 * an operator to force the server to re-read and process its configuration
 * file.
 */
void rehash(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 4.3 Die message
 *
 * @brief An operator can use the DIE command to shutdown the server.
 */
void die(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 4.4 Restart message
 *
 * @brief  An operator can use the restart command to force the server to
 * restart itself.
 */
void restart(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 4.5 Summon message
 *
 * @brief The SUMMON command can be used to give users who are on a host
 * running an IRC server a message asking them to please join IRC.
 */
void summon(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 4.6 Users
 *
 * @brief The USERS command returns a list of users logged into the server in a
 * format similar to the UNIX commands who(1), rusers(1) and finger(1).
 */
void users(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 4.7 Operwall message
 *
 * @brief The WALLOPS command is used to send a message to all currently
 * connected users who have set the 'w' user mode for themselves.
 */
void wallops(Server *server, int socket, Commands command)
{
	if (command.params.empty())
		return server->sendMsg(ERR_NEEDMOREPARAMS(command.command), socket);
	std::vector<Client *>           vec = server->getAllClients();
	std::vector<Client *>::iterator it = vec.begin();
	for (; it != vec.end(); it++)
	{
		if ((*it)->getMode() & WALLOPS)
			server->sendMsg(
					WALLOPS(server->getHostname(), server->getClientByFd(socket)->getNickname(), command.params[0]),
					(*it)->getFd());
	}
}

/**
 * 4.8 Userhost message
 *
 * @brief he USERHOST command takes a list of up to 5 nicknames, each
 * separated by a space character and returns a list of information
 * about each nickname that it found.
 */
void userhost(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}

/**
 * 4.9 Ison message
 * @brief The ISON command was implemented to provide a quick and efficient
 * means to get a response about whether a given nickname was currently
 * on IRC.
 */
void ison(Server *server, int socket, Commands command)
{
	(void)command;
	server->sendMsg(RPL_CLIENT((std::string) "This feature will be realease next month! Be patient!"), socket);
}
