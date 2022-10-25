#ifndef COMMANDRESPONSES_HPP
# define COMMANDRESPONSES_HPP

#include <iostream>
#include <string>

///////////////////////////////////////GLOBAL REPLIES////////////////////////////////////////

//Connection & registration
# define RPL_WELCOME(nick, user, host)								("Welcome to the Internet Relay Network " + nick + "!" + user + "@" +  host + "\r\n") //001
# define RPL_CREATED(date)											(":This server was created " + date + "\r\n") //003
# define RPL_BOUNCE(server_name, port)								("Try server " + server_name + ", port " + port + "\r\n") //005
# define RPL_YOURESERVICE(servicename)								("You are service " + servicename + "\r\n") //383
//Service
# define RPL_YOURHOST(servername, version)							(":Your host is " + servername + ", running version " + version + "\r\n") //002
# define RPL_MYINFO(servername, version, user_modes, chann_modes)	(":" + servername + " " + version + " " + user_modes + " " + chann_modes + "\r\n")//004
//Userhost
# define RPL_USERHOST(userhost_list)								(userhost_list + "\r\n") //302


///////////////////////////////////////CMD REPLIES//////////////////////////////////////////

// ISON
# define RPL_ISON(user_list)										(user_list + "\r\n")//303
// INVITE
# define RPL_AWAY(pseudo, message)									(pseudo + " " + message + "\r\n") //301
# define RPL_INVITING(channel, nick)								(channel + " " + nick + "\r\n") //341
//WHO
# define RPL_WHOREPLY(chan, usr, host, server, nick, presence, role, status, real_name)		(chan + " " + usr + " " + host + " " + server + " " + nick + " " + presence + role + status ":0 " + real_name + "\r\n")
# define RPL_ENDOFWHO(name)											(name + " :End of WHO list\r\n"); //315
// AWAY
# define RPL_UNAWAY													(":You are no longer marked as being away\r\n") //305
# define RPL_NOWAWAY												(":You have been marked as being away\r\n") //306
// WHOIS
# define RPL_WHOISUSER(nick, user, host, realname)					(nick + " " + user + " " + host + " * :" + realname + "\r\n")  //311
# define RPL_WHOISSERVER(nick, server, info_server)					(nick + " " + server + " :" +  info + "\r\n") //312
# define RPL_WHOISOPERATOR(nick)									(nick + " :is an IRC operator\r\n") //313
# define RPL_WHOISIDLE(nick, integer)								(nick + " " + integer + " :seconds idle\r\n") //317
# define RPL_ENDOFWHOIS(nick)										(nick + " :End of WHOIS list\r\n") //318
# define RPL_WHOISCHANNELS(nick, channel_info)						(nick + " :" + channel_info + "\r\n") //319
// WHOWAS
# define RPL_WHOWASUSER(nick, user, host, real_name)				(nick + " " + user + " " + host + " * :" + real_name + "\r\n") //314
# define RPL_ENDOFWHOWAS(nick)										(nick + " :End of WHOWAS\r\n") //369
//LIST
# define RPL_LIST(channel, visible, topic)							(channel + " " + visible + " :" + topic + "\r\n") //322
# define RPL_LISTEND												(":End of LIST\r\n") //323
//MODE
# define RPL_UNIQOPIS(channel, nick)								(channel + " " + nick + "\r\n") //325
# define RPL_CHANNELMODEIS(channel, mode, params)					(channel + " " + mode + " " + params + "\r\n") //324
# define RPL_INVITELIST(channel, invitemask)						(channel + " " + invitemask + "\r\n") //346
# define RPL_ENDOFINVITELIST(channel)								(channel + " :End of channel invite list\r\n") //347
# define RPL_EXCEPTLIST(channel, exceptionmask)						(channel + " " + exceptionmask + "\r\n") //348
# define RPL_ENDOFEXCEPTLIST										(channel + " :End of channel exception list") //349
//TOPIC
# define RPL_NOTOPIC(channel)										(channel + " :No topic is set\r\n") //331
# define RPL_TOPIC(channel, topic)									(channel + " :" + topic) //332
//SUMMON
# define RPL_SUMMONING(user)										(user + " :Summoning user to IRC\r\n") //342
//VERSION
# define RPL_VERSION(version, debuglevel, server, comment)			(version + "." + debuglevel + " " + server + " :" + comment + "\r\n") //351
//JOIN
# define RPL_NAMREPLY(channelStatus, channel, nick)					(channelStatus + " " + channel + " :" + nick + "\r\n") //353
//NAMES
# define RPL_ENDOFNAMES(channel)									(channel + " :End of NAMES list\r\n") //366
//LINKS
# define RPL_LINKS(mask, server, server_info)						(mask + " " + server + " :0 " + server_info + "\r\n") // 364
# define RPL_ENDOFLINKS(mask)										(mask + " :End of LINKS list" + "\r\n") //365
//MODE
# define RPL_BANLIST(channel, banmask)								(channel + " " + banmask + "\r\n") //367
# define RPL_ENDOFBANLIST(channel)									(channel + " :End of channel ban list\r\n") //368
//INFO
# define RPL_INFO(string)											(":" + string + "\r\n") //371
# define RPL_ENDOFINFO												(":End of INFO list\r\n") //374
//MOTD
# define RPL_MOTDSTART(server)										(":- " + server + " Message of the day - \r\n") //375
# define RPL_MOTD(text)												(":- " + text + "\r\n") //372
# define RPL_ENDOFMOTD												(":End of MOTD command") //376
//OPER
# define RPL_YOUREOPER												(":You are now an IRC operator\r\n") //381
//REHASH
# define RPL_REHASHING(config_file)									(config_file + " :Rehashing\r\n") //382
//TIME
# define RPL_TIME(server, time)										(server + " :" + time + "\r\n") //391
//USERS
# define RPL_USERSSTART												(":UserID   Terminal  Host\r\n") //392
# define RPL_USERS(username, ttyline, hostname)						(":" + username + " " + ttyline + " " + hostname + "\r\n") //393
# define RPL_ENDOFUSERS												(":End of users\r\n") //394
# define RPL_NOUSERS												(":Nobody logged in\r\n") //395
//TRACE
# define RPL_TRACELINK(version, destination, server)				("Link " + version + " " + destination + " " + server + "\r\n") //200
# define RPL_TRACECONNECTING(classe, server)							("Try. " + class + " " + server + "\r\n") //201
# define RPL_TRACEHANDSHAKE(classe, server)							("H.S " +  classe + " " + server) //202




////////////////////////////////GLOBAL ERRORS/////////////////////////////////////////////


////////////////////////////////CMD ERRORS//////////////////////////////////////////////

// PASS
# define ERR_NEEDMOREPARAMS(command)								(command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTRED										(":Unauthorized command (already registered)\r\n")

// NICK
# define ERR_NONICKNAMEGIVEN										(":No nickname given\r\n")
# define ERR_NICKNAMEINUSE(nick)									(nick + " :Nickname is already in use\r\n")
# define ERR_ERRONEUSNICKNAME(nick)									(nick + " :Erroneous nickname\r\n")
# define ERR_NICKCOLLISION(nick, host, user)						(nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
# define ERR_RESTRICTED												(":Your connection is restricted!\r\n")
# define ERR_UNAVAILRESOURCE(nickChannel)							(nickChannel + " :Nick/channel is temporarily unavailable\r\n")


#endif
