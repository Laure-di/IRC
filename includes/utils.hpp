#ifndef UTILS_HPP
#define UTILS_HPP

/**
 * @brief delete the first element of the vector
 **/
template <typename T>
void pop_front(std::vector<T> &vec)
{
	std::reverse(vec.begin(), vec.end());
	vec.pop_back();
	std::reverse(vec.begin(), vec.end());
}

void                     addElementsVector(std::vector<std::string> *list, std::vector<std::string> toAdd);
bool                     isLetter(const char c);
bool                     isDigit(const char c);
bool                     isHexDigit(const char c);
bool                     isHyphen(const char c);
bool                     isSpecial(const char c);
std::string              toString(const int nb);
int                      toInt(std::string string);
bool                     isNumber(char *str);
bool                     checkUsername(std::string username);
bool                     checkNickname(std::string nickname);
unsigned                 hasher(const char *s);
bool                     checkChanstring(std::string name);
bool                     checkChannelName(std::string name);
bool                     isChannelName(std::string name);
bool                     strmatch(std::string string, std::string pattern);
void                     applyModeChangesChannel(Server *server, int socket, std::string flags, std::string param,
                                                  Channel *channel);
void                     applyModeChangesClient(Server *server, int socket, std::string flags, Client *client);
void                     printWho(Server *server, int socket, std::vector<Client *> listOfClients);
std::vector<std::string> splitCmd(std::string toSplit, std::string delimiter);
std::vector<std::string> splitBy(std::string string, std::string delimiter);
void                     checkAndJoinChannel(Server *server, int socket, std::string channelName, std::string key);
bool                     isClientFullyRegister(Client *user);
bool                     isRegistrationCmd(std::string command);
void                     stringToUpper(std::string *string);
void                     transformCmdsToUpper(std::vector<Commands> *commandsList);
bool                     isCmdFull(std::string string);
void                     welcomeClient(Server *server, int socket, Client *currentUser);
bool                     isUnavailableNickname(Server *server, std::string nick);
bool                     isNumber(std::string nb);
int                      areParamsValid(std::vector<std::string> params);

#endif
