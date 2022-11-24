#include "../includes/include.hpp"

void	welcomeClient(Server *server, int socket, Client *currentUser)
{
	if (isClientFullyRegister(currentUser))
	{
		std::vector<std::string> params;
		Commands modtCmd("MOTD", "", params, false);
		server->sendMsg(RPL_WELCOME(currentUser->getFullIdentifier()), socket);
		server->sendMsg(RPL_YOURHOST(server->getHostname(), server->getVersion()), socket);
		server->sendMsg(RPL_CREATED(server->getLaunchingDate()), socket);
		server->sendMsg(RPL_MYINFO(server->getHostname(), server->getVersion(), CLIENT_MODE, CHANNEL_MODE), socket);
		motd(server, socket, modtCmd);
	}
	return ;
}
/*
 * @Brief if a client as been kill by an operator, nobody can connect within a delay of 60 sec.
 *		  https://stackoverflow.com/questions/12121091/diffrence-of-two-time-in-c
*/
bool	isUnavailableNickname(Server* server, std::string nick)
{
	std::map<std::string, time_t>::const_iterator it = server->getNicknameUnavailable().end();
	if (server->getNicknameUnavailable().find(nick) != it)
	{
		double killTime = difftime(time(0), server->getNicknameUnavailable().find(nick)->second);
		if (killTime < 60)
			return (true);
		return (false);
	}
	return (false);
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
	return (mode = 0); //TODO change
}
