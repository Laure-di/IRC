#include "../includes/include.hpp"

/**
 * Check command length
 *
 * @brief check if the length if max 512 (include the \r\n) if one of the commands if > 512 none will be executed and an error message will be display */

bool	checkCmdLength(std::vector<std::string> listOfCommands)
{
	std::vector<std::string>::iterator	it;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		if (510 < it->size())
		{
			std::cerr << *it << " : command too long to be executed" << std::endl;
			return (false);
		}
	}
	return (true);
}

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
	std::vector<Commands>::iterator it;
	for (it = commandsList->begin(); it != commandsList->end(); it++)
		stringToUpper(&it->command);
}

void	stringToUpper(std::string *string)
{
	//	std::transform(string->begin(), string->end(), string->begin(), std::ptr_fun<int, int>(std::toupper));
	//	std::transform(string->begin(), string->end(), string->begin(), static_cast<int (*)(int)>(&std::toupper));
	std::transform(string->begin(), string->end(), string->begin(), ::toupper);
}
