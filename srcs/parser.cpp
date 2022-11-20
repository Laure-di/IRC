#include "../includes/include.hpp"

#define MAX_CMD_LGTH 510
#define	MAX_PARAMS	15
#define DEBUG

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

std::string		handlePrefix(std::string *cmd)
{
	std::string	prefix;
	if (cmd->find(":") == 0)
	{
		size_t	end;
		end = cmd->find(" ");
		prefix = cmd->substr(0, end + 1);
		cmd->erase(0, end + 1);
#ifdef DEBUG
		std::cout << prefix << std::endl;
		std::cout << *cmd << std::endl;
#endif
	}
	return (prefix);
}


void	parseCmd(std::string cmd, std::vector<Commands> *command) //change return type to Commands
{
	std::vector<std::string>	rslt;
	std::string					cd;
	std::string					prefix;
	std::vector<std::string>	params;
	bool						colon;

	//(void)command;

	prefix = handlePrefix(&cmd);
	if (cmd.find(":") != std::string::npos)
	{
		colon = true;
		std::vector<std::string>first;
		rslt = splitBy(cmd, ":");
#ifdef DEBUG
		std::cout << "after split : : " << std::endl;
		print_debug(rslt);
		std::cout << "rslt begin()" << std::endl;
		std::cout << *rslt.begin() << std::endl;
#endif
		first = splitBy(*rslt.begin(), " ");
#ifdef DEBUG
		std::cout << "after split : : " << std::endl;
		print_debug(first);
#endif
		pop_front<std::string>(rslt);
		cd =*(first.begin());
		if (1 < first.size())
		{
			pop_front<std::string>(first);
			addElementsVector(&params, first);
			//params = first;
			addElementsVector(&params, rslt);
		}
#ifdef DEBUG
		std::cout << "Case of command with colon : " << std::endl;
		std::cout << "command : " << cd << std::endl;
		print_debug(params);
#endif
	}
	else
	{
		colon = false;
		rslt = splitBy(cmd, " ");
		cd = *(rslt.begin());
		std::cout << rslt.size() << std::endl;
		if (rslt.size() > 1)
		{
			pop_front<std::string>(rslt);
			params = rslt;
		}
		command->push_back(Commands(cd, prefix, params, colon));
#ifdef DEBUG
		std::cout << "command : " << cd << std::endl;
		print_debug(params);
#endif
	}
}

std::vector<Commands>	manageMultipleCommands(std::vector<std::string> listOfCommands)
{
	std::vector<Commands>							commandsToExecute;
	std::vector<std::string>::iterator				it;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		parseCmd(*it, &commandsToExecute);
	}
	return (commandsToExecute);
}
