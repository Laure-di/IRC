//Include of Class
#ifndef INCLUDE_HPP
# define INCLUDE_HPP

#ifndef CLIENT_HPP
#include "./Class/Client.hpp"
#endif
#ifndef CHANNEL_HPP
#include "./Class/Channel.hpp"
#endif
#ifndef SERVER_HPP
#include "./Class/Server.hpp"
#endif

//Include other header files
#ifndef COMMANDRESPONSES_HPP
#include "CommandResponses.hpp"
#endif
#ifndef COMMANDS_HPP
#include "Commands.hpp"
#endif
#ifndef EXCEPTIONS_HPP
#include "Exceptions.hpp"
#endif
#ifndef PARSER_HPP
#include "parser.hpp"
#endif
#ifndef UTILS_HPP
#include "utils.hpp"
#endif

//Include librairies
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <cerrno>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <map>
#include <deque>
#include <vector>
#include <sstream>
#include <algorithm>
#include <signal.h>
#include <bits/signum.h>
#include <bits/types/sig_atomic_t.h>
# include <bits/types/siginfo_t.h>

struct	Commands
{
	std::string				command;
	std::string				prefix;
	std::deque<std::string>	params;
	bool					colon;
	/*Commands(std::string cmd, std::string pref, std::deque<std::string> parameters)
	{
		command = cmd;
		prefix = pref;
		params = parameters;
	}*/
};

#endif
