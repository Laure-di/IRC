#ifndef PARSER_HPP
# define PARSER_HPP

void					print_debug(std::vector<std::string> print);
void					printCmd(Commands commands);
void					printAllCmds(std::vector<Commands> print);
std::string				handlePrefix(std::string *cmd);
void					parseCmd(std::string cmd, std::vector<Commands> *command);
std::vector<Commands>	manageMultipleCommands(std::vector<std::string> listOfCommands);
bool					checkCommandLength(std::vector<std::string> listOfCommands);
bool					checkMaxParam(std::vector<Commands> commands);


#endif
