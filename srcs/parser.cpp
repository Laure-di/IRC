#include "../includes/include.hpp"

#define MAX_CMD_LGTH 510
#define	MAX_PARAMS	15

void	print_debug(std::vector<std::string> print)
{
	std::cout << "params : " << std::endl;
	std::vector<std::string>::iterator	it;
	int	i = 1;
	for (it = print.begin() ;it != print.end(); it++)
		std::cout << i++ << " parameter " <<*it << std::endl;
}

void	printCmd(Commands commands)
{
	std::cout << "Prefix : " << commands.prefix << std::endl;
	std::cout << "Command : " << commands.command << std::endl;
	std::cout << "Paramaters : " << std::endl;
	print_debug(commands.params);
}

void	printAllCmds(std::vector<Commands> print)
{
	std::vector<Commands>::iterator	it;
	for (it = print.begin() ;it != print.end(); it++)
		printCmd(*it);
}

/*bool	checkCommandLenght(std::vector<std::string> listOfCommands)
  {
  std::vector<std::string>::iterator	it;
  for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
  {
  if (MAX_CMD_LGTH < it->size())
  return (false);
  }
  return (true);
  }*/

bool	checkMaxParam(Commands commands)
{
	if (MAX_PARAMS < commands.params.size())
		return (false);
	return (true);
}

void	handlePrefix(std::string *cmd, Commands *command)
{
	if (cmd->find(":") == 0)
	{
		size_t	end;
		end = cmd->find(" ");
		command->prefix = cmd->substr(0, end + 1);
		cmd->erase(0, end + 1);
#ifdef DEBUG
		std::cout << command->prefix << std::endl;
		std::cout << *cmd << std::endl;
#endif
	}
}

Commands	parseCmd(std::string cmd) //change return type to Commands
{
	std::vector<std::string>	rslt;
	Commands				command;

	handlePrefix(&cmd, &command);
	if (cmd.find(":") != std::string::npos)
	{
		command.colon = true;
		std::vector<std::string>first;
		rslt = split(cmd, ":");
		first = split(*(rslt.begin()), " ");
		pop_front<std::string>(rslt);
		command.command =*(first.begin());
		if (1 < first.size())
		{
			pop_front<std::string>(first);
			command.params = first;
			addElementsVector(&command.params, rslt);
		}
#ifdef DEBUG
		std::cout << "Case of command with colon : " << std::endl;
		std::cout << "command : " << command.command << std::endl;
		print_debug(command.params);
#endif
	}
	else
	{
		command.colon = false;
		rslt = split(cmd, " ");
		command.command = *(rslt.begin());
		std::cout << rslt.size() << std::endl;
		if (rslt.size() > 1)
		{
			pop_front<std::string>(rslt);
			command.params = rslt;
		}
#ifdef DEBUG
		std::cout << "command : " << command.command << std::endl;
		print_debug(command.params);
#endif
	}
	return (command);

}

std::vector<Commands>	manageMultipleCommands(std::vector<std::string> listOfCommands)
{
	std::vector<Commands>							commandsToExecute;
	std::vector<std::string>::iterator				it;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		commandsToExecute.push_back(parseCmd(*it));
	}
	return (commandsToExecute);
}
