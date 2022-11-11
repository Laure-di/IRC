#include <algorithm>
#include <iostream>
#include <string>
#include <deque>

#include "../includes/Class/Server.hpp"

void	print_debug(std::deque<std::string> print)
{
	std::cout << "entre dans debug" << std::endl;
	std::deque<std::string>::iterator	it;
	int	i = 1;
	for (it = print.begin() ;it != print.end(); it++)
		std::cout << i++ << " argument " <<*it << std::endl;
}

std::deque<std::string>		split(std::string toSplit, std::string delimiter)
{
	std::deque<std::string>	result;
	size_t	start, end = delimiter.size() * -1;

	do
	{
		start = end + delimiter.size();
		end = toSplit.find(delimiter, start);
		if (end != std::string::npos || delimiter != "\r\n")//TODO probleme un element de trop
			result.push_back(toSplit.substr(start, end - start));
	} while (end != std::string::npos);

	return (result);
}

void	parseCmd(std::string cmd) //change return type to Commands
{
	std::deque<std::string>	rslt;
	Commands				command;
	int		i = cmd.find(":");

	std::cout << i << std::endl;

	if (cmd.find(":") == 0)
	{
		size_t	end;
		end = cmd.find(" ");
		command.prefix = cmd.substr(0, end + 1);
		cmd.erase(0, end + 1);
		std::cout << command.prefix << std::endl;
		std::cout << cmd << std::endl;
	}
	if (cmd.find(":") != std::string::npos) //SITUATION PRIVMSG ET USER
	{
		std::deque<std::string>first;
		rslt = split(cmd, ":");
		first = split(*(rslt.begin()), " ");
		print_debug(first);
	}
	else
	{
		rslt = split(cmd, " ");
		command.command = *(rslt.begin());
		rslt.erase(rslt.begin());
		command.params = rslt;
		print_debug(command.params);
		std::cout << "command : " << command.command << std::endl;
	}
	command.prefix = "coucou c'est loupé";

}

void	manageMultipleCommands(std::deque<std::string> listOfCommands, int nbCmd) //TODO change return type to Commands*
{
	//Commands commandsToExecute[nbCmd];
	(void)nbCmd;
	std::deque<std::string>::iterator	it;
//	int	i = 0;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		//commandsToExecute[i] = parseCmd(*it);
		parseCmd(*it);
//		i++;
	}
	//return (commandsToExecute);
}

