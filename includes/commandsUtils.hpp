#ifndef COMMANDSUTILS_HPP
# define COMMANDSUTILS_HPP

void	welcomeClient(Server *server, int socket, Client *currentUser);
bool	isUnavailableNickname(Server* server, std::string nick);
bool	isNumber(std::string nb);
int		areParamsValid(std::vector<std::string> params);

#endif
