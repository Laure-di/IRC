#ifndef COMMANDRESPONSES_HPP
#define COMMANDRESPONSES_HPP

#include "include.hpp"

struct NumericReplies
{
	int         num;
	std::string msg;

	NumericReplies(int num, std::string msg) : num(num), msg(msg){};
};

///////////////////////////////////////GLOBAL
/// REPLIES////////////////////////////////////////
// Connection & registration
#define RPL_WELCOME(fullIdentifier) \
	NumericReplies(001, "Welcome to the Internet Relay Network " + fullIdentifier + "\r\n")
#define RPL_CREATED(date) NumericReplies(003, ":This server was created " + date)
#define RPL_BOUNCE(serverName, port) NumericReplies(005, "Try server " + serverName + ", port " + port + "\r\n")
#define RPL_YOURESERVICE(servicename) NumericReplies(383, "You are service " + servicename + "\r\n")
// Service
#define RPL_YOURHOST(servername, version) \
	NumericReplies(002, ":Your host is " + servername + ", running version " + version + "\r\n")
#define RPL_MYINFO(servername, version, userModes, channModes) \
	NumericReplies(004, ":" + servername + " " + version + " " + userModes + " " + channModes + "\r\n")
// Userhost
#define RPL_USERHOST(userhost_list) NumericReplies(302, userhost_list + "\r\n")
///////////////////////////////////////CMD
/// REPLIES//////////////////////////////////////////
// INVITE
#define RPL_AWAY(nickname, awayMessage) NumericReplies(301, nickname + " :" + awayMessage + "\r\n")
#define RPL_INVITING(channel, nickname) NumericReplies(341, channel + " " + nickname + "\r\n")
// AWAY
#define RPL_UNAWAY NumericReplies(305, ":You are no longer marked as being away\r\n")
#define RPL_NOWAWAY NumericReplies(306, ":You have been marked as being away\r\n")
// WHO
#define RPL_WHOREPLY(chan, usr, host, server, nickname, presence, real_name)                                     \
	NumericReplies(352, chan + " " + usr + " " + host + " " + server + " " + nickname + " " + presence + ":0 " + \
								real_name + "\r\n")
#define RPL_ENDOFWHO(name) NumericReplies(315, name + " :End of WHO list\r\n")
// WHOIS
#define RPL_WHOISUSER(nickname, user, host, realname) \
	NumericReplies(311, nickname + " " + user + " " + host + " * :" + realname + "\r\n")
#define RPL_WHOISSERVER(nickname, server, infoServer) \
	NumericReplies(312, nickname + " " + server + " :" + info + "\r\n")
#define RPL_WHOISOPERATOR(nickname) NumericReplies(313, nickname + " :is an IRC operator\r\n")
#define RPL_WHOISIDLE(nick, integer) NumericReplies(317, nickname + " " + integer + " :seconds idle\r\n")
#define RPL_ENDOFWHOIS(nickname) NumericReplies(318, nickname + " :End of WHOIS list\r\n")
#define RPL_WHOISCHANNELS(nickname, channelInfo) NumericReplies(319, nickname + " :" + channelInfo + "\r\n")
// WHOWAS
#define RPL_WHOWASUSER(nickname, user, host, real_name) \
	NumericReplies(314, nickname + " " + user + " " + host + " * :" + real_name + "\r\n")
#define RPL_ENDOFWHOWAS(nickname) NumericReplies(369, nickname + " :End of WHOWAS\r\n")
// LIST
#define RPL_LIST(channel, clientCount, channelTopic) \
	NumericReplies(322, channel + " " + clientCount + " :" + channelTopic + "\r\n")
#define RPL_LISTEND NumericReplies(323, ":End of LIST\r\n")
// MODE
#define RPL_UNIQOPIS(channel, nickname) NumericReplies(325, channel + " " + nickname + "\r\n")
#define RPL_CHANNELMODEIS(msg) NumericReplies(324, msg + "\r\n")
#define RPL_INVITELIST(channel, invitemask) NumericReplies(346, channel + " " + invitemask + "\r\n")
#define RPL_ENDOFINVITELIST(channel) NumericReplies(347, channel + " :End of channel invite list\r\n")
#define RPL_EXCEPTLIST(channel, exceptionmask) NumericReplies(348, channel + " " + exceptionmask + "\r\n")
#define RPL_ENDOFEXCEPTLIST NumericReplies(349, channel + " :End of channel exception list\r\n")
// TOPIC
#define RPL_NOTOPIC(channel) NumericReplies(331, channel + " :No topic is set\r\n")
#define RPL_TOPIC(channel, channelTopic) NumericReplies(332, channel + " :" + channelTopic + "\r\n")
// VERSION
#define RPL_VERSION(serverVersion, debuglevel, server, comment) \
	NumericReplies(351, serverVersion + "." + debuglevel + " " + server + " :" + comment + "\r\n")
// JOIN
#define RPL_NAMREPLY(channelStatus, channel, nickName) \
	NumericReplies(353, channelStatus + " " + channel + " :" + nickName + "\r\n")
// NAMES
#define RPL_ENDOFNAMES(channel) NumericReplies(366, channel + " :End of NAMES list\r\n")
// LINKS
#define RPL_LINKS(mask, server, server_info) NumericReplies(364, mask + " " + server + " :0 " + server_info + "\r\n")
#define RPL_ENDOFLINKS(mask) NumericReplies(365, mask + " :End of LINKS list" + "\r\n")
// MODE
#define RPL_BANLIST(channel, banmask) NumericReplies(367, channel + " " + banmask + "\r\n")
#define RPL_ENDOFBANLIST(channel) NumericReplies(368, channel + " :End of channel ban list\r\n")
#define RPL_UMODEIS(userMode) NumericReplies(221, userMode + "\r\n")
// INFO
#define RPL_INFO(string) NumericReplies(371, ":" + string + "\r\n")
#define RPL_ENDOFINFO NumericReplies(374, ":End of INFO list\r\n")
// MOTD
#define RPL_MOTDSTART(server) NumericReplies(375, ":- " + server + " Message of the day - \r\n")
#define RPL_MOTD(text) NumericReplies(372, ":- " + text + "\r\n")
#define RPL_ENDOFMOTD NumericReplies(376, ":End of MOTD command\r\n")
// OPER
#define RPL_YOUREOPER NumericReplies(381, ":You are now an IRC operator\r\n")
// REHASH
#define RPL_REHASHING(config_file) NumericReplies(382, config_file + " :Rehashing\r\n")
// TIME
#define RPL_TIME(server, time) NumericReplies(391, server + " :" + time + "\r\n")
// USERS
#define RPL_USERSSTART NumericReplies(392, ":UserID   Terminal  Host\r\n")
#define RPL_USERS(username, ttyline, hostname) \
	NumericReplies(393, ":" + username + " " + ttyline + " " + hostname + "\r\n")
#define RPL_ENDOFUSERS NumericReplies(394, ":End of users\r\n")
#define RPL_NOUSERS NumericReplies(395, ":Nobody logged in\r\n")
// ADMIN
#define RPL_ADMINME(server) NumericReplies(256, server + " :Administrative info\r\n")
#define RPL_ADMINLOC1(admin_info) NumericReplies(257, ":" + admin_info + "\r\n")
#define RPL_ADMINLOC2(admin_info) NumericReplies(258, ":" + admin_info + "\r\n")
#define RPL_ADMINEMAIL(admin_info) NumericReplies(259, ":" + admin_info + "\r\n")
// TRY AGAIN MSG
#define RPL_TRYAGAIN(command) NumericReplies(263, command + " :Please wait a while and try again.\r\n")

// Connexion &&& registration
#define ERR_NOTREGISTERED NumericReplies(451, ":You have not registered\r\n")
#define ERR_NOPERMFORHOST NumericReplies(463, ":Your host isn't among the privilegedi\r\n")
#define ERR_YOUREBANNEDCREEP NumericReplies(465, ":You are banned from this server\r\n")
// PASS
#define ERR_NEEDMOREPARAMS(command) NumericReplies(461, command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED NumericReplies(462, ":Unauthorized command (already registered)\r\n")
// NICK
#define ERR_NONICKNAMEGIVEN NumericReplies(431, ":No nickname given\r\n")
#define ERR_NICKNAMEINUSE(nick) NumericReplies(433, nick + " :Nickname is already in use\r\n")
#define ERR_ERRONEUSNICKNAME(nick) NumericReplies(432, nick + " :Erroneous nickname\r\n")
#define ERR_NICKCOLLISION(nick, host, user) \
	NumericReplies(436, nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
#define ERR_RESTRICTED NumericReplies(484, ":Your connection is restricted!\r\n")
#define ERR_UNAVAILRESOURCE(nickChannel) \
	NumericReplies(437, nickChannel + " :Nick/channel is temporarily unavailable\r\n")
// INVITE
#define ERR_NOSUCHNICK(nick) NumericReplies(401, nick + " :No such nick/channel\r\n")
#define ERR_USERONCHANNEL(user, channel) NumericReplies(443, user + " " + channel + " :is already on channel\r\n")
#define ERR_FILEERROR(file_op, file) NumericReplies(424, ":File error doing " + file_op + " on " + file + "\r\n")
// USERS
#define ERR_NOSUCHSERVER(serverName) NumericReplies(402, serverName + " :No such server\r\n")
#define ERR_USERSDISABLED NumericReplies(446, ":USERS has been disabled\r\n")
// JOIN
#define ERR_NOSUCHCHANNEL(channelName) NumericReplies(403, channelName + " :No such channel\r\n")
#define ERR_TOOMANYCHANNELS(channelName) NumericReplies(405, channelName + " :You have joined too many channels\r\n")
#define ERR_TOOMANYTARGETS(target, error_code, message) \
	NumericReplies(407, target + " :" + error_code + " recipients. " + message + "\r\n")
#define ERR_CHANNELISFULL(channel) NumericReplies(471, channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(mode, channel) \
	NumericReplies(472, mode + " :is unknown mode char to me for " + channel + "\r\n")
#define ERR_INVITEONLYCHAN(channel) NumericReplies(473, channel + "  :Cannot join channel (+i)\r\n")
#define ERR_BANNEDFROMCHAN(channel) NumericReplies(474, channel + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANNELKEY(channel) NumericReplies(475, channel + " :Cannot join channel (+k)\r\n")
#define ERR_BADCHANMASK(channel) NumericReplies(476, channel + " :Bad Channel Mask\r\n")
// PRIVMSG
#define ERR_CANNOTSENDTOCHAN(channelName) NumericReplies(404, channelName + " :Cannot send to channel\r\n")
#define ERR_NORECIPIENT(command) NumericReplies(411, ":No recipient given " + command + "\r\n")
#define ERR_NOTEXTTOSEND NumericReplies(412, ":No text to send\r\n")
#define ERR_NOTOPLEVEL(mask) NumericReplies(413, mask + " :No toplevel domain specified\r\n")
#define ERR_WILDTOPLEVEL(mask) NumericReplies(414, mask + "<mask> :Wildcard in toplevel domain\r\n")
#define ERR_BADMASK(mask) NumericReplies(415, mask + " :Bad Server/host mask\r\n")
// WHOWAS
#define ERR_WASNOSUCHNICK(nickname) NumericReplies(406, nickname + " :There was no such nickname\r\n")
// PING && PONG
#define ERR_NOORIGIN NumericReplies(409, ":No origin specified\r\n")
// OTHER ERRORS
#define ERR_UNKNOWNCOMMAND(command) NumericReplies(421, command + " :Unknown command\r\n")
#define ERR_NOPRIVILEGES NumericReplies(481, ":Permission Denied- You're not an IRC operator\r\n")
#define ERR_UNIQOPPRIVSNEEDED NumericReplies(485, ":You're not the original channel operator\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) NumericReplies(482, channel + " :You're not channel operator\r\n")
// MODT
#define ERR_NOMOTD NumericReplies(422, ":MOTD File is missing\r\n")
// ADMIN
#define ERR_NOADMININFO(server) NumericReplies(423, server + " :No administrative info available\r\n")
// MODE
#define ERR_USERNOTINCHANNEL(nickname, channel) \
	NumericReplies(441, nickname + " " + channel + " :They aren't on that channel\r\n")
#define ERR_KEYSET(channel) NumericReplies(467, channel + " :Channel key already set\r\n")
#define ERR_NOCHANMODES(channel) NumericReplies(477, channel + " :Channel doesn't support modes\r\n")
#define ERR_UMODEUNKNOWNFLAG NumericReplies(501, ":Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH NumericReplies(502, ":Cannot change mode for other users\r\n")
// PART
#define ERR_NOTONCHANNEL(channel) NumericReplies(442, channel + " :You're not on that channel\r\n")
// KICK ??
#define ERR_BANLISTFULL(channel, mode) NumericReplies(478, channel + " " + mode + " :Channel list is full\r\n")
// OPER
#define ERR_PASSWDMISMATCH NumericReplies(464, ":Password incorrect\r\n")
#define ERR_CANTKILLSERVER NumericReplies(483, ":You can't kill a server!\r\n")
#define ERR_NOOPERHOST NumericReplies(491, ":No O-lines for your host\r\n")
#define RPL_CLIENT(line) NumericReplies(0, line + "\r\n")
#define ERR_CLIENT(line) NumericReplies(0, line + "\r\n")
#define PONG(expediteur) NumericReplies(0, "PONG: " + expediteur + "\r\n")
#define PING(expediteur) NumericReplies(0, "Ping message from " + expediteur + "\r\n")
#define WALLOPS(serverName, expediteur, msg) \
	NumericReplies(0, "Command from " + expediteur + " to " + msg + " " + " to/from " + serverName + "\r\n")
#define QUIT(line) NumericReplies(0, line)

#endif
