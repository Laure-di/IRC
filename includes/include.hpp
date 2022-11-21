//Include of Class
#ifndef INCLUDE_HPP
# define INCLUDE_HPP

//Include librairies
#include <iostream>
#include <string>
#include <cctype>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
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
//#include <bits/signum.h>
#include <bits/types/sig_atomic_t.h>
#include <bits/types/siginfo_t.h>
#include <bits/stdc++.h>
#include <netdb.h>


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
#include "execution_utils.hpp"
#include "commandsUtils.hpp"

#define CLIENT_MODE "coucou"
#define CHANNEL_MODE "hello"

struct	Commands
{
	std::string					command;
	std::string					prefix;
	std::vector<std::string>	params;
	bool						colon;
	Commands(std::string _command, std::string _prefix, std::vector<std::string> _params, bool _colon)
	{
		command = _command;
		prefix = _prefix;
		params = _params;
		colon = _colon;

	}
};

#endif
