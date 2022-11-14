//Include of Class
#ifndef INCLUDE_HPP
# define INCLUDE_HPP

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
#include <bits/types/siginfo_t.h>
#include <bits/stdc++.h>

class Channel;
class Client;
class Server;
struct Commands;
struct NumericReplies;
typedef void cmd_func(Server *server, int socket, Commands command);
typedef void (*cmd_func_ptr)(Server *server, int socket, Commands command);
typedef std::map<std::string, cmd_func_ptr> cmd_dict;

#include "utils.hpp"
#include "./Class/Client.hpp"
#include "./Class/Channel.hpp"
#include "./Class/Server.hpp"
#include "CommandResponses.hpp"
#include "Commands.hpp"
#include "Exceptions.hpp"
#include "parser.hpp"

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
