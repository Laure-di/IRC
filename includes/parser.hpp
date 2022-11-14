#ifndef PARSER_HPP
# define PARSER_HPP

void					print_debug(std::deque<std::string> print);
void					printCmd(Commands commands);
void					printAllCmds(std::deque<Commands> print);
void					handlePrefix(std::string *cmd, Commands *command);
Commands				parseCmd(std::string cmd);
std::deque<Commands>	manageMultipleCommands(std::deque<std::string> listOfCommands);
bool					checkCommandLength(std::deque<std::string> listOfCommands);
bool					checkMaxParam(std::deque<Commands> commands);


#endif
