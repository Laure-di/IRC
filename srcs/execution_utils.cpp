#include "../includes/include.hpp"

bool	canRegisterPass(Client* user)
{
	if ((user->getNickname()).empty() && (user->getUsername().empty()))
		return (true);
	return (false);
}

bool	isFullyClientRegister(Client* user)
{
	if ((user->getNickname()).empty() || (user->getUsername()).empty())
		return (false);
	return (true);
}

bool	isRegistrationCmd(std::string command)
{
	if (command == "NICK" || command == "CAP" || command == "USER" || command == "PASS")
		return (true);
	return (false);
}

void	stringToUpper(std::string *string)
{
	std::transform(string->begin(), string->end(), string->begin(), std::ptr_fun<int, int>(std::toupper));
}
