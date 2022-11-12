#include <algorithm>
#include <iostream>
#include <string>
#include <deque>

#include "../includes/Class/Server.hpp"
#include "../includes/utils.hpp"

#define MAX_CMD_LGTH 510
#define	MAX_PARAMS	15

void	print_debug(std::deque<std::string> print)
{
	std::cout << "params : " << std::endl;
	std::deque<std::string>::iterator	it;
	int	i = 1;
	for (it = print.begin() ;it != print.end(); it++)
		std::cout << i++ << " parameter " <<*it << std::endl;
}

void	print_struct(Commands commands)
{
		std::cout << "Prefix : " << commands.prefix << std::endl;
		std::cout << "Command : " << commands.command << std::endl;
		std::cout << "Paramaters : " << std::endl;
		print_debug(commands.params);
}

void	printAllCmds(std::deque<Commands> print)
{
	std::deque<Commands>::iterator	it;
	for (it = print.begin() ;it != print.end(); it++)
		print_struct(*it);
}

bool	checkCommandLenght(std::deque<std::string> listOfCommands)
{
	std::deque<std::string>::iterator	it;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		if (MAX_CMD_LGTH < *it.size())
			return (false);
	}
	return (true);
}

bool	checkMaxParam(std::deque<Commands> commands)
{
	if (MAX_PARAMS < params.size())
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
	std::deque<std::string>	rslt;
	Commands				command;

	handlePrefix(&cmd, &command);
	if (cmd.find(":") != std::string::npos)
	{
		command.colon = true;
		std::deque<std::string>first;
		rslt = split(cmd, ":");
		first = split(*(rslt.begin()), " ");
		rslt.pop_front();
		command.command =*(first.begin()).toupper();
		first.pop_front();
		command.params = first;
		addElementsDeque(&command.params, rslt);
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
		command.command = *(rslt.begin()).toupper();
		rslt.pop_front();
		command.params = rslt;
#ifdef DEBUG
		std::cout << "command : " << command.command << std::endl;
		print_debug(command.params);
#endif
	}
	return (command);

}

std::deque<Commands>	manageMultipleCommands(std::deque<std::string> listOfCommands)
{
	std::deque<Commands>							commandsToExecute;
	std::deque<std::string>::iterator				it;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		commandsToExecute.push_back(parseCmd(*it));
	}
	return (commandsToExecute);
}
