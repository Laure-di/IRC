#ifndef ERRORREPLIES_HPP
# define ERRORREPLIES_HPP

#include <iostream>
#include <string>

//Connexion &&& registration
# define ERR_NOTREGISTERED											(":You have not registered\r\n") //451
# define ERR_NOPERMFORHOST											(":Your host isn't among the privilegedi\r\n") //463
# define ERR_YOUREBANNEDCREEP										(":You are banned from this server\r\n") //465
// PASS
# define ERR_NEEDMOREPARAMS(command)								(command + " :Not enough parameters\r\n") //461
# define ERR_ALREADYREGISTRED										(":Unauthorized command (already registered)\r\n")//462
// NICK
# define ERR_NONICKNAMEGIVEN										(":No nickname given\r\n") //431
# define ERR_NICKNAMEINUSE(nick)									(nick + " :Nickname is already in use\r\n") //433
# define ERR_ERRONEUSNICKNAME(nick)									(nick + " :Erroneous nickname\r\n") //432
# define ERR_NICKCOLLISION(nick, host, user)						(nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
# define ERR_RESTRICTED												(":Your connection is restricted!\r\n") //484
# define ERR_UNAVAILRESOURCE(nickChannel)							(nickChannel + " :Nick/channel is temporarily unavailable\r\n") //437
//INVITE
# define ERR_NOSUCHNICK(nick)										(nick + " :No such nick/channel\r\n")//401
# define ERR_USERONCHANNEL(user, channel)							(user + " " + channel + " :is already on channel\r\n") //443
# define ERR_FILEERROR(file_op, file)								(":File error doing " + file_op + " on " + file + "\r\n") //424
//USERS
# define ERR_NOSUCHSERVER(server_name)								(server_name + " :No such server\r\n") //402
# define ERR_USERSDISABLED											(":USERS has been disabled\r\n") //446
//JOIN
# define ERR_NOSUCHCHANNEL(channel_name)							(channel_name + " :No such channel\r\n") //403
# define ERR_TOOMANYCHANNELS(channel_name)							(channel_name + " :You have joined too many channels\r\n") //405
# define ERR_TOOMANYTARGETS(target, error_code, message)			(target + " :" + error_code + " recipients. " + message + "\r\n") //407
# define ERR_CHANNELISFULL(channel)									(channel + " :Cannot join channel (+l)\r\n") //471
# define ERR_UNKNOWNMODE(mode, channel)								(mode + " :is unknown mode char to me for " + channel + "\r\n") //472
# define ERR_INVITEONLYCHAN(channel)								(channel + "  :Cannot join channel (+i)\r\n") //473
# define ERR_BANNEDFROMCHAN(channel)								(channel + " :Cannot join channel (+b)\r\n") //474
# define ERR_BADCHANNELKEY(channel)									(channel + " :Cannot join channel (+k)\r\n") //475
# define ERR_BADCHANMASK(channel)									(channel + " :Bad Channel Mask\r\n") //476
//PRIVMSG
# define ERR_CANNOTSENDTOCHAN(channel_name)							(channel_name + " :Cannot send to channel")//404
# define ERR_NORECIPIENT(command)									(":No recipient given " + command + "\r\n") //411
# define ERR_NOTEXTTOSEND											(":No text to send\r\n") //412
# define ERR_NOTOPLEVEL(mask)										(mask + " :No toplevel domain specified\r\n") //413
# define ERR_WILDTOPLEVEL(mask)										(mask + "<mask> :Wildcard in toplevel domain\r\n") //414
# define ERR_BADMASK(mask)											(mask + " :Bad Server/host mask\r\n") //415
//WHOWAS
# define ERR_WASNOSUCHNICK(nickname)								(nickname + " :There was no such nickname\r\n") //406
//PING && PONG
# define ERR_NOORIGIN												(":No origin specified\r\n")//409
//OTHER ERRORS
# define ERR_UNKNOWNCOMMAND(command)								(command + " :Unknown command\r\n") //421
# define ERR_NOPRIVILEGES											(":Permission Denied- You're not an IRC operator") //481
# define ERR_UNIQOPPRIVSNEEDED										(":You're not the original channel operator\r\n") //485
# define ERR_CHANOPRIVSNEEDED(channel)								(channel + " :You're not channel operator\r\n") //482
//MODT
# define ERR_NOMOTD													(":MOTD File is missing\r\n") //422
//ADMIN
# define ERR_NOADMININFO(server)									(server + " :No administrative info available\r\n") //423
//MODE
# define ERR_USERNOTINCHANNEL(nick, channel)						(nick + " " + channel + " :They aren't on that channel\r\n") //441
# define ERR_KEYSET(channel)										(channel + " :Channel key already set\r\n") //467
# define ERR_NOCHANMODES(channel)									(channel + " :Channel doesn't support modes\r\n") //477
# define ERR_UMODEUNKNOWNFLAG										( ":Unknown MODE flag\r\n") //501
# define ERR_USERSDONTMATCH											(":Cannot change mode for other users\r\n") //502
//PART
# define ERR_NOTONCHANNEL(channel)									(channel + " :You're not on that channel\r\n") //442
//KICK ??
# define ERR_BANLISTFULL(channel, mode)								(channel + " " + mode + " :Channel list is full\r\n") //478
//OPER
# define ERR_PASSWDMISMATCH											( ":Password incorrect\r\n") //464
# define ERR_CANTKILLSERVER											(":You can't kill a server!\r\n") //483
# define ERR_NOOPERHOST												(":No O-lines for your host\r\n") //491

#endif


