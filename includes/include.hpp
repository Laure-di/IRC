// Include of Class
#ifndef INCLUDE_HPP
#define INCLUDE_HPP

// Include librairies
#include <algorithm>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <bits/types/sig_atomic_t.h>
#include <bits/types/siginfo_t.h>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <deque>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Channel;
class Client;
class Server;
struct Commands;
struct NumericReplies;
typedef void cmd_func(Server *server, int socket, Commands command);
typedef void (*cmd_func_ptr)(Server *server, int socket, Commands command);
typedef std::map<std::string, cmd_func_ptr> cmd_dict;

#include "./Class/Channel.hpp"
#include "./Class/Client.hpp"
#include "./Class/Server.hpp"
#include "commandResponses.hpp"
#include "commands.hpp"
#include "exceptions.hpp"
#include "parser.hpp"
#include "utils.hpp"

#define CLIENT_MODE "-/+airow"
#define CHANNEL_MODE "-/+ikmnpstl"

static bool is_running = true;

struct Commands
{
	std::string              command;
	std::string              prefix;
	std::vector<std::string> params;
	bool                     colon;
	Commands(std::string _command, std::string _prefix, std::vector<std::string> _params, bool _colon)
	{
		command = _command;
		prefix = _prefix;
		params = _params;
		colon = _colon;
	}
};

#endif
