#ifndef EXECUTIONUTILS_HPP
# define EXECUTIONUTILS_HPP

bool		isClientFullyRegister(Client* user);
bool		isRegistrationCmd(std::string command);
void		stringToUpper(std::string *string);
void		transformCmdsToUpper(std::vector<Commands> *commandsList);
bool		isCmdFull(std::string string);

#endif
