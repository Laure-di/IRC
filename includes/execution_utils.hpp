#ifndef EXECUTION_UTILS_HPP
# define EXECUTION_UTILS_HPP

bool		canRegisterPass(Client* user);
bool		isClientFullyRegister(Client* user);
bool		isRegistrationCmd(std::string command);
void		stringToUpper(std::string *string);
void		transformCmdsToUpper(std::vector<Commands> *commandsList);
bool		checkCmdLength(std::vector<std::string> listOfCommands);

#endif
