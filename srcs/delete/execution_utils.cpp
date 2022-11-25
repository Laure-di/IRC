#include "../includes/include.hpp"


bool	canRegisterPass(Client* user)
{
	if ((user->getNickname()).empty() && (user->getUsername()).empty())
		return (true);
	return (false);
}

bool	isClientFullyRegister(Client* user)
{
	if (!(user->getNickname()).empty() && !(user->getUsername()).empty())
		return (true);
	return (false);
}

bool	isRegistrationCmd(std::string command)
{
	if (command == "NICK" || command == "CAP" || command == "USER" || command == "PASS" || command == "QUIT" || command == "SERVICE")
		return (true);
	return (false);
}

void	transformCmdsToUpper(std::vector<Commands> *commandsList)
{
/*	for (int i = 0; i < (int)commandsList.size(); i++)
	{
		stringToUpper(&commandsList[i].command);
	}*/
	std::vector<Commands>::iterator it;
	for (it = commandsList->begin(); it != commandsList->end(); it++)
		stringToUpper(&it->command);
}

void	stringToUpper(std::string *string)
{
	std::transform(string->begin(), string->end(), string->begin(), ::toupper);
}

bool	isCmdFull(std::string string)
{
	if (string.find("\n") != std::string::npos)
		return true;
	return false;
}

