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

//USERS
# define ERR_NOSUCHSERVER(server_name)								(server_name + " :No such server\r\n") //402

//JOIN
# define ERR_NOSUCHCHANNEL(channel_name)							(channel_name + " :No such channel\r\n")//403
# define ERR_TOOMANYCHANNELS(channel_name)							(channel_name + " :You have joined too many channels\r\n")//405
# define ERR_TOOMANYTARGETS(target, error_code, message)			(target + " :" + error_code + " recipients. " + message + "\r\n") //407


//PRIVMSG
# define ERR_CANNOTSENDTOCHAN(channel_name)							(channel_name + " :Cannot send to channel")//404

//WHOWAS
# define ERR_WASNOSUCHNICK(nickname)								(nickname + " :There was no such nickname\r\n") //406

//PING
# define ERR_NOORIGIN												(":No origin specified\r\n")//409

