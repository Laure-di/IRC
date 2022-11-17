#include "../includes/include.hpp"

bool	isUnavailableNickname(Server* server, std::string nick)
{
	std::map<std::string, time_t>::const_iterator it = server->getNicknameUnavailable().end();
	if (server->getNicknameUnavailable().find(nick) == it)
		return (false);
	return (true);
}
