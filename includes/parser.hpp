void		print_debug(std::deque<std::string> print);
void		print_struct(Commands commands);
void		printAllCmds(std::deque<Commands> print);
void		handlePrefix(std::string *cmd, Commands *command);
Commands	parseCmd(std::string cmd);
std::deque<Commands>	manageMultipleCommands(std::deque<std::string> listOfCommands);

