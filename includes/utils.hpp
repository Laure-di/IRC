#ifndef UTILS_HPP
# define UTILS_HPP

std::deque<std::string>		split(std::string to_split, std::string delimiter);
void						addElementsDeque(std::deque<std::string> *list, std::deque<std::string> toAdd);
bool isLetter(const char c);
bool isDigit(const char c);
bool isHexDigit(const char c);
bool isHyphen(const char c);
bool isSpecial(const char c);
std::string toString(const int nb);
int toInt(std::string string);
bool checkUsername(std::string username);
bool checkNickname(std::string nickname);
unsigned hasher(const char *s);
bool checkChanstring(std::string name);
bool checkChannelName(std::string name);
std::vector<std::string>		splitComma(std::string string);
bool strmatch(std::string string, std::string pattern);
void applyModeChanges(Server *server, int socket, std::string flags, std::string params, Client *client, Channel *channel);
void printWho(Server *server, int socket, std::deque<Client *>listOfClients);


#endif
