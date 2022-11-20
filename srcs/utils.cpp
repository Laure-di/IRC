#include "../includes/include.hpp"

//TODO change all split by this one
std::vector<std::string>		splitCmd(std::string toSplit, std::string delimiter)
{
	std::vector<std::string>	result;
	size_t	start, end = delimiter.size() * -1;
	std::string				toAdd;

	while (end != std::string::npos)
	{
		start = end + delimiter.size();
		end = toSplit.find(delimiter, start);
#ifdef DEBUG
		std::cout << "Size of string to split" << toSplit.size() << std::endl;
		std::cout << "value of end split " << end <<  std::endl;
#endif
		if (end != std::string::npos)
			toAdd = toSplit.substr(start, end - start);
		else
			toAdd = toSplit.substr(start, toSplit.size() - 1 - start);
		if (!toAdd.empty())
			result.push_back(toAdd);
#ifndef DEBUG
		std::cout << "code a supprimer dans srcs/utils.cpp splitCmd" << std::endl;
		std::cout << toAdd << std::endl;
#endif
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
	std::string forbiddenCharacters = "\0\a\n\r ,:";
	for (size_t i = 0; i < forbiddenCharacters.size(); i++) {
		if (name.find(forbiddenCharacters[i]) != std::string::npos)
			return false;
	}
	return true;
};

/**
 * @brief Check channel name
 *
 * channel =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring [ ":" chanstring ]
 * For now, only check channel = ("#" / "&") chanstring
 */
bool checkChannelName(std::string name) {
	if (name.length() < 2)
		return false;
	char firstChar = name[0];
	if (firstChar != '#' && firstChar != '&')
		return false;
	name.erase(0, 1);
	return checkChanstring(name);
};


void	addElementsVector(std::vector<std::string> *list, std::vector<std::string> toAdd)
{
	std::vector<std::string>::iterator	it;
	for (it = toAdd.begin(); it != toAdd.end(); it++)
	{
		if (!it->empty())
			list->push_back(*it);
	}
}

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
		std::cout << "value of string"  << string << std::endl;
		std::cout << "string to add : " << toAdd << std::endl;
		std::cout << "value of start :" << start << std::endl;
		std::cout << "value of end :" << end << std::endl;
		result.push_back(toAdd);
		start = end + 1;
	}
	result.push_back(string.substr(start, string.length()));
	return (result);
}

std::vector<std::string> splitComma(std::string string)
{
	std::vector<std::string>	result;
	size_t	start = 0;
	size_t	end;

	while (true)
	{
		end = string.find(",", start);
		if (end == std::string::npos)
			break;
		result.push_back(string.substr(start, end));
		start = end + 1;
	}
	result.push_back(string.substr(start, string.length()));
	return (result);
}
/**
 * @brief  Wildcard pattern matching algorithm
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
 * @brief Apply flags to client or channels
 */
void applyModeChanges(Server *server, int socket, std::string flags, std::string params, Client *client, Channel *channel)
{
	// Add custom error message, from Mordern IRC: The text used in the last param of this message may vary.
	(void) channel;
	(void) params;
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
				client->modMode(Invisible, add);
				break;
			case 'w':
				client->modMode(Wallops, add);
				break;
			case 'r':
				client->modMode(Restricted, add);
				break;
			case 'o':
				// Only for removing
				client->modMode(Operator, add);
				break;
			case 'O':
				client->modMode(LocalOperator, add);
				break;
			case 's':
				client->modMode(ServerNotices, add);
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
}

/**
 * @brief Print info on list of clients
 */
void printWho(Server *server, int socket, std::vector<Client *>listOfClients)
{
	if (listOfClients.empty())
		return;
	/*	std::vector<Client *>::iterator listOfClientsIterator;
		for (listOfClientsIterator = listOfClients.begin(); listOfClientsIterator < listOfClients.end(); listOfClientsIterator++)
		{
	// Get all params values
	// chan == activeChannel ?
	// server->sendMsgToFd(RPL_WHOREPLY(chan, usr, host, server, nickname, presence, role, status, real_name), socket);
	}*/
	NumericReplies msg = RPL_ENDOFWHO(server->getClientByFd(socket)->getNickname());
	server->sendMsg(msg, socket);
}

std::vector<std::string>		split(std::string string, std::string delimiter)
{
	std::vector<std::string>	result;
	size_t	start = 0;
	size_t	end;

	while (true)
	{
		end = string.find(delimiter, start);
		if (end == std::string::npos)
			break;
		result.push_back(string.substr(start, end));
		start = end + 1;
	}
	result.push_back(string.substr(start, string.length()));
	return (result);
}

void checkAndJoinChannel(Server *server, int socket, std::string channelName, std::string key)
{
	if (!checkChannelName(channelName))
		return server->sendMsg(ERR_BADCHANMASK(channelName), socket);
	Channel *channel = server->getChannelByName(channelName);
	if (!channel)
		return server->createNewChannel(socket, channelName);
}
