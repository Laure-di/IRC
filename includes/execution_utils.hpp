#ifndef EXECUTION_UTILS_HPP
# define EXECUTION_UTILS_HPP

bool		canRegisterPass(Client* user);
bool		isFullyClientRegister(Client* user);
bool		isRegistrationCmd(std::string command);
void		stringToUpper(std::string *string);
void		transformCmdsToUpper(std::deque<Commands> *commandsList);
bool		checkCmdLength(std::deque<std::string> listOfCommands);

#endif
