#include "../includes/include.hpp"

/**
  * @brief when a client is first fully register this function is call.
  */
void	welcomeClient(Server *server, int socket, Client *currentClient)
{
	if (isClientFullyRegister(currentClient))
	{
		std::vector<std::string> params;
		Commands modtCmd("MOTD", "", params, false);
		server->sendMsg(RPL_WELCOME(currentClient->getFullIdentifier()), socket);
		server->sendMsg(RPL_YOURHOST(server->getHostname(), server->getVersion()), socket);
		server->sendMsg(RPL_CREATED(server->getLaunchingDate()), socket);
		server->sendMsg(RPL_MYINFO(server->getHostname(), server->getVersion(), CLIENT_MODE, CHANNEL_MODE), socket);
		motd(server, socket, modtCmd);
	}
	return ;
}

/**
 * @brief if a client as been kill by an operator, nobody can connect within a delay of 60 sec.
 * https://stackoverflow.com/questions/12121091/diffrence-of-two-time-in-c
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

/**
 * @brief check if a string is a number. Check for main argument.
 */
bool isNumber(std::string nb) {
	for (unsigned int i = 0; i < nb.size(); i++) {
		if (isdigit(nb[i]) == 0)
			return false;
	}
	return true;
}

/**
 * @brief command user check if the parameter are valid
 */
int	areParamsValid(std::vector<std::string> params)
{
	int mode;
	if (params.empty())
		return (mode = -1);
	if (isNumber(params[1]))
		return (mode = toInt(params[1]));
	else if (params[1] != params[0])
		return (mode = -1);
	return (mode = 0);
}

/**
 * @brief check if client is fully register and can execute other commands
 */
bool	isClientFullyRegister(Client* user)
{
	if (!(user->getNickname()).empty() && !(user->getUsername()).empty())
		return (true);
	return (false);
}

/**
 * @brief check if command is a registration one
 */
bool	isRegistrationCmd(std::string command)
{
	if (command == "NICK" || command == "CAP" || command == "USER" || command == "PASS" || command == "QUIT" || command == "SERVICE")
		return (true);
	return (false);
}

/**
 * @brief take all the std::string command of the vector of Commands struct and transform to uppercase
 */
void	transformCmdsToUpper(std::vector<Commands> *commandsList)
{
	std::vector<Commands>::iterator it;
	for (it = commandsList->begin(); it != commandsList->end(); it++)
		stringToUpper(&it->command);
}

/**
 * @brief transform string to uppercase
 */
void	stringToUpper(std::string *string)
{
	std::transform(string->begin(), string->end(), string->begin(), ::toupper);
}

/**
 * @brief check if in the client buffer there is "\n" which mean the command is complere and can be executed
 */
bool	isCmdFull(std::string string)
{
	if (string.find("\n") != std::string::npos)
		return true;
	return false;
}

/**
 * @brief split a string into different cmd string (delimiter = "\r\n")
 */
std::vector<std::string>		splitCmd(std::string toSplit, std::string delimiter)
{
	std::vector<std::string>	result;
	size_t	start, end = delimiter.size() * -1;
	std::string				toAdd;

	while (end != std::string::npos)
	{
		start = end + delimiter.size();
		end = toSplit.find(delimiter, start);
		if (end != std::string::npos)
			toAdd = toSplit.substr(start, end - start);
		else
			toAdd = toSplit.substr(start, toSplit.size() - 1 - start);
		if (!toAdd.empty())
			result.push_back(toAdd);
	};

	return (result);
}

/**
 * @brief Check if a character is a letter
 */
bool isLetter(const char c)
{
	return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}

/**
 * @brief Check if a character is a digit
 */
bool isDigit(const char c) {
	return '0' <= c && c <= '9';
}

/**
 * @brief Check if a character is a number
 */
bool isNumber(char *str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (!isDigit(str[i]))
		{
			std::cerr << "The port argument must be a positive number" << std::endl;
			return (false);
		}
		i++;
	}
	return (true);
}

/**
 * @brief Check if a character is a hexadecimal digit letters
 */
bool isHexDigit(const char c) {
	return (isDigit(c) || ('A' <= c && c <= 'F'));
}

/**
 * @brief Check if a character is an hyphen
 */
bool isHyphen(const char c) {
	return c == '-';
}

/**
 * @brief Check if a character is among %x5B-60 / %x7B-7
 */
bool isSpecial(const char c) {
	std::string specialCharacters = ";[]`_^{|}\\";
	return specialCharacters.find(c) != std::string::npos;
}

/**
 * @brief Convert int to string
 */
std::string toString(const int nb) {
	std::ostringstream ss;
	ss << nb;
	return ss.str();
}

/**
 * @brief Parse string to int
 */
int toInt(std::string string) {
	int nb;
	std::istringstream(string) >> nb;
	return nb;
}

/**
 * @brief Check that username match ( letter / special ) *8( letter / digit / special / "-" )
 */
bool checkUsername(std::string username) {
	std::string forbiddenCharacters = "\0\n\r@";
	for (size_t i = 0; i < forbiddenCharacters.size(); i++) {
		if (username.find(forbiddenCharacters[i]) != std::string::npos)
			return false;
	}
	return true;
};

/**
 * @brief Check that nickname match ( letter / special ) *8( letter / digit / special / "-" )
 */
bool checkNickname(std::string nickname) {
	if (nickname.empty())
		return false;
	if (nickname.length() > 9)
		return false;
	char firstChar = nickname[0];
	if (!(isLetter(firstChar) || isSpecial(firstChar)))
		return false;
	for (size_t i = 1; i < nickname.size(); i++) {
		char nChar = nickname[i];
		if (!(isLetter(nChar) || isDigit(nChar) || isSpecial(nChar) || isHyphen(nChar)))
			return false;
	}
	return true;
};

/**
 * @brief Quick and dirty hash https://stackoverflow.com/a/107657
 */
unsigned hasher(const char *s)
{
	unsigned hash = 42;
	while (*s)
		hash = hash * 101 + *s++;
	return hash;
}

/**
 * @brief Check chanstring
 *
 * chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
 * chanstring =/ %x2D-39 / %x3B-FF; any octet except NUL, BELL, CR, LF, " ", "," and ":"
 */
bool checkChanstring(std::string name) {
	std::string forbiddenCharacters = "\a\n\r ,:";
	for (size_t i = 0; i < forbiddenCharacters.size(); i++) {
		if (name.find(forbiddenCharacters[i]) != std::string::npos)
			return false;
	}
	return true;
};

/**
 * @brief Check channelid
 *
 * channelid  = 5( %x41-5A / digit )   ; 5( A-Z / 0-9 )
 */
bool checkChannelid(std::string channelid) {
	for (size_t i = 0; i < channelid.size(); i++) {
		if (!isupper(channelid[i]) || !isdigit(channelid[i]))
			return false;
	}
	return true;
};

/**
 * @brief Check if channel name is correct
 *
 * channel =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring [ ":" chanstring ]
 */
bool checkChannelName(std::string name) {
	if (name.length() < 2)
		return false;
	char firstChar = name[0];
	if (firstChar != '#' && firstChar != '+' && firstChar != '!' && firstChar != '&')
		return false;
	if (firstChar == '!')
	{
		if (!checkChannelid(name.substr(1,5)))
			return false;
		name.erase(0, 6);
	}
	else
		name.erase(0, 1);
	return checkChanstring(name);
};

/**
 * @brief Check if the string is a channel name (starting by a channel character)
 */
bool isChannelName(std::string name) {
	char firstChar = name[0];
	if (firstChar != '#' && firstChar != '+' && firstChar != '!' && firstChar != '&')
		return false;
	return true;
}

/**
 * @brief Append a string vector to a string vector
*/
void	addElementsVector(std::vector<std::string> *list, std::vector<std::string> toAdd)
{
	std::vector<std::string>::iterator	it;
	for (it = toAdd.begin(); it != toAdd.end(); it++)
	{
		if (!it->empty())
			list->push_back(*it);
	}
}

/**
 * @brief Split a string by a delimiter
*/
std::vector<std::string> splitBy(std::string string, std::string delimiter)
{
	std::vector<std::string>	result;
	size_t			start = 0;
	size_t			end;
	std::string		toAdd;

	while (true)
	{
		end = string.find(delimiter, start);
		if (end  == std::string::npos)
			break;
		toAdd = string.substr(start, end - start);
		result.push_back(toAdd);
		start = end + 1;
	}
	result.push_back(string.substr(start, string.length()));
	return (result);
}

/**
 * @brief Wildcard pattern matching algorithm
 *
 * Help from https://www.geeksforgeeks.org/wildcard-pattern-matching/
 */
bool strmatch(std::string string, std::string pattern)
{
	size_t stringLen = string.size();
	size_t patternLen = pattern.size();
	if (patternLen == 0)
		return (stringLen == 0);
	bool lookup[stringLen + 1][patternLen + 1];
	memset(lookup, false, sizeof(lookup));
	lookup[0][0] = true;
	for (size_t j = 1; j <= patternLen; j++)
		if (pattern[j - 1] == '*')
			lookup[0][j] = lookup[0][j - 1];
	for (size_t i = 1; i <= stringLen; i++) {
		for (size_t j = 1; j <= patternLen; j++) {
			if (pattern[j - 1] == '*')
				lookup[i][j] = lookup[i][j - 1] || lookup[i - 1][j];
			else if (pattern[j - 1] == '?' || string[i - 1] == pattern[j - 1])
				lookup[i][j] = lookup[i - 1][j - 1];
			else
				lookup[i][j] = false;
		}
	}
	return lookup[stringLen][patternLen];
}

/**
 * @brief Apply flags to client
 */
void applyModeChangesClient(Server *server, int socket, std::string flags, Client *client)
{
	// Add custom error message? From Modern IRC: The text used in the last param of this message may vary.
	char firstChar = flags[0];
	if (firstChar != '-' && firstChar != '+')
		return server->sendMsg(ERR_UMODEUNKNOWNFLAG, socket);
	bool add = (firstChar == '+');
	for (size_t i = 1; i < flags.length(); i++)
	{
		char nChar = flags[i];
		switch (nChar)
		{
			case 'i':
				client->modMode(INVISIBLE, add);
				break;
			case 'r':
				client->modMode(RESTRICTED, add);
				break;
			case 'w':
				client->modMode(WALLOPS, add);
				break;
			case 'o':
				// Only for removing for client
				if (add)
				{
					server->sendMsg(ERR_UMODEUNKNOWNFLAG, socket);
					break;
				}
				client->modMode(OPERATOR, add);
			case '+':
				add = true;
				break;
			case '-':
				add = false;
				break;
			default:
				server->sendMsg(ERR_UMODEUNKNOWNFLAG, socket);
		}
	}
	if (client->getMode() == NONE)
		return 	server->sendMsg("You do not have any mode set\r\n", socket);
	server->sendMsg("Your user mode is " + client->getModeStr() + "\r\n", socket);
}

/**
 * @brief Apply flags to client or channels
 */
void applyModeChangesChannel(Server *server, int socket, std::string flags, std::string param, Channel *channel)
{
	Client *client = server->getClientByFd(socket);
	std::string nickname = client->getNickname();
	if (!(channel->checkOperatorByNickname(nickname)))
		return server->sendMsg(ERR_CHANOPRIVSNEEDED(channel->getName()), socket);
	char firstChar = flags[0];
	if (firstChar != '-' && firstChar != '+')
		return server->sendMsg(ERR_UMODEUNKNOWNFLAG, socket);
	bool add = (firstChar == '+');
	for (size_t i = 1; i < flags.length(); i++)
	{
		char nChar = flags[i];
		switch (nChar)
		{
			case 'b':
				if (param.empty())
				{
					server->sendMsg(ERR_NEEDMOREPARAMS(std::string("MODE")), socket);
					break;
				}
				channel->modClientMask(nChar, add, param);
				break;
			case 'e':
				if (param.empty())
				{
					server->sendMsg(ERR_NEEDMOREPARAMS(std::string("MODE")), socket);
					break;
				}
				channel->modClientMask(nChar, add, param);
				break;
			case 'I':
				if (param.empty())
				{
					server->sendMsg(ERR_NEEDMOREPARAMS(std::string("MODE")), socket);
					break;
				}
				channel->modClientMask(nChar, add, param);
				break;
			case 'i':
				channel->modMode(INVITATION, add);
				break;
			case 'k':
				if (add && param.empty())
				{
					server->sendMsg(ERR_NEEDMOREPARAMS(std::string("MODE")), socket);
					break;
				}
				channel->modKey(add, param);
				break;
			case 'l':
				if (add && toInt(param) < 0)
				{
					server->sendMsg(ERR_UMODEUNKNOWNFLAG, socket);
					break;
				}
				if (add && param.empty())
				{
					server->sendMsg(ERR_NEEDMOREPARAMS(std::string("MODE")), socket);
					break;
				}
				channel->modLimit(add, param);
				break;
			case 'm':
				channel->modMode(MODERATED, add);
				break;
			case 'n':
				channel->modMode(OUTSIDE, add);
				break;
			case 'p':
				channel->modMode(PRIVATE, add);
				break;
			case 's':
				channel->modMode(SECRET, add);
				break;
			case 't':
				channel->modMode(TOPIC, add);
				break;
			case 'o':
				channel->modClientMode(socket, param, OPERATOR, add);
				break;
			case 'O':
				channel->modClientMode(socket, param, CREATOR, add);
				break;
			case 'v':
				channel->modClientMode(socket, param, VOICE, add);
				break;
			case '+':
				add = true;
				break;
			case '-':
				add = false;
				break;
			default:
				server->sendMsg(ERR_UMODEUNKNOWNFLAG, socket);
		}
	}
	if (channel->getMode() == NONE)
		return 	server->sendMsg("No mode is set for " + channel->getName() + "\r\n", socket);
	server->sendMsg("The channel " + channel->getName() + " mode is " + channel->getModeStr() + "\r\n", socket);
}
