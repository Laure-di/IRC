#include "../includes/Commands.hpp"

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
		server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	// Add check if already connected (send ERR_ALREADYREGISTRED) and check password
};

/**
 * 3.1.2 Nick message
 *
 * @brief The NICK command is used to give user a nickname or change the existing one.
 */
void nick(Server *server, int socket, Commands command) {
	if(command.params.size() == 0)
		return server->sendNumericReplyToFd(ERR_NONICKNAMEGIVEN, socket);
	std::string nickname = command.params[0];
	if (!checkNickname(nickname))
		return server->sendNumericReplyToFd(ERR_ERRONEUSNICKNAME(nickname), socket);
	if (server->findUserByNickname(nickname))
		return server->sendNumericReplyToFd(ERR_NICKNAMEINUSE(nickname), socket);
	// Add check for ERR_NICKCOLLISION (nickname taken on another server)
	// Add check for ERR_UNAVAILRESOURCE (nickname in server list of forbidden nickames)
	// Add check for ERR_RESTRICTED (user mode "+r")
	User *currentUser = server->findUserByFd(socket);
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
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	User *currentUser = server->findUserByFd(socket);
	if (!currentUser->getUsername().empty())
		return server->sendNumericReplyToFd(ERR_ALREADYREGISTRED, socket);
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
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string name = command.params[0];
	std::string password = command.params[1];
	if (!server->checkPassword(password))
		return server->sendNumericReplyToFd(ERR_PASSWDMISMATCH, socket);
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
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);

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
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
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
   characteristics of a channel.
 */
void mode(Server *server, int socket, Commands command)
{
	if(command.params.size() < 2)
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string nickname = command.params[0];
	User* currentUser = server->getUserByFd(socket);
	if (currentUser->getNickname() != nickname)
		return server->sendNumericReplyToFd(ERR_USERSDONTMATCH, socket);
}

/**
 * 3.2.4 Topic message
 *
 * @brief The TOPIC command is used to change or view the topic of a channel.
 */
void topic(Server *server, int socket, Commands command)
{
	if(command.params.size() < 1)
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string channelName = command.params[0];
	Channel* channel = server->findChannelByName(channelName);
	if (!channel)
		return;
	User *currentUser = server->findUserByFd(socket);
	if (channel->findUserByNickname(currentUser->getNickname()))
		return server->sendNumericReplyToFd(ERR_NOTONCHANNEL(channel->get_name()), socket);
	if(command.params.size() == 1) {
		if (channel->get_topic().empty())
			return server->sendNumericReplyToFd(RPL_NOTOPIC(channel->get_name()), socket);
		return server->sendNumericReplyToFd(RPL_TOPIC(channel->get_name(), channel->get_topic()), socket);
	}
	if (channel->findOperatorByNickname(currentUser->getNickname()))
		return server->sendNumericReplyToFd(ERR_CHANOPRIVSNEEDED(channel->get_name()), socket);
	// Add check for ERR_NOCHANMODES
	std::string topic = command.params[1];
	if (topic == ":")
		return channel->clear_topic();
	channel->set_topic(topic);
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

}

/**
 * 3.2.7 Invite message
 *
 * @brief The INVITE command is used to invite a user to a channel.
 */
void invite(Server *server, int socket, Commands command)
{
	if(command.params.size() != 2)
		return server->sendNumericReplyToFd(ERR_NEEDMOREPARAMS(command.command), socket);
	std::string nickname = command.params[0];
	std::string channelName = command.params[1];

	User *invitedUser = server->findUserByNickname(nickname);
	if (!invitedUser)
		return server->sendNumericReplyToFd(ERR_NOSUCHNICK(nickname), socket);
	Channel *channel = server->findChannelByName(channelName);
	if (!channel)
		return server->sendNumericReplyToFd(ERR_NOSUCHNICK(channelName), socket);
	User *currentUser = server->findUserByFd(socket);
	if (!channel->findUserByNickname(currentUser->getNickname()))
		return server->sendNumericReplyToFd(ERR_NOTONCHANNEL(channelName), socket);
	if (channel->findUserByNickname(nickname))
		return server->sendNumericReplyToFd(ERR_USERONCHANNEL(nickname, channelName), socket);
	// Add check for ERR_CHANOPRIVSNEEDED (channel is invite only and currentUser is not an operator)
	// Add check for RPL_AWAY (Check that invitedUser is away and notify currentUser ? Not defined)
	server->sendNumericReplyToFd(RPL_INVITING(nickname, channelName), invitedUser->getFd());
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

}

/*
* 3.3 Sending messages
*/
cmd_func privmsg;
cmd_func notice;

/*
* 3.4 Server queries and commands
*/
cmd_func motd;
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
cmd_func who;
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
