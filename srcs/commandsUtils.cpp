#include "../includes/include.hpp"

bool	isUnavailableNickname(Server* server, std::string nick)
{
	std::map<std::string, time_t>::const_iterator it = server->getNicknameUnavailable().end();
	if (server->getNicknameUnavailable().find(nick) == it)
		return (false);
	return (true);
}

bool isNumber(std::string nb) {
	for (unsigned int i = 0; i < nb.size(); i++) {
		if (isdigit(nb[i]) == 0)
			return false;
	}
	return true;
}

int	areParamsValid(std::vector<std::string> params)
{
	int mode;
	if (params.empty())
		return (mode = -1);
	if (isNumber(params[1]))
		return (mode = toInt(params[1]));
	else if (params[1] != params[0])
		return (mode = -1);
	return (mode = -1); //TODO change
}
