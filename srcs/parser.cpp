#include "../includes/include.hpp"

#define MAX_CMD_LGTH 510
#define MAX_PARAMS 15

void print_debug(std::vector<std::string> print)
{
	std::cout << "params : " << std::endl;
	std::vector<std::string>::iterator it;
	int                                i = 1;
	for (it = print.begin(); it != print.end(); it++)
		std::cout << i++ << " parameter " << *it << std::endl;
}

void printCmd(Commands commands)
{
	std::cout << "Prefix : " << commands.prefix << std::endl;
	std::cout << "Command : " << commands.command << std::endl;
	std::cout << "Paramaters : " << std::endl;
	print_debug(commands.params);
}

void printAllCmds(std::vector<Commands> print)
{
	std::vector<Commands>::iterator it;
	for (it = print.begin(); it != print.end(); it++)
		printCmd(*it);
}

std::string handlePrefix(std::string *cmd)
{
	std::string prefix;
	if (cmd->find(":") == 0)
	{
		size_t end;
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

void parseCmd(std::string cmd, std::vector<Commands> *command)
{
	std::vector<std::string> rslt;
	std::string              cd;
	std::string              prefix;
	std::vector<std::string> params;
	bool                     colon = false;

	prefix = handlePrefix(&cmd);
	if (cmd.substr(0, 5) == "JOIN ")
	{
		cd = "JOIN";
		std::string tmp = cmd.substr(5, cmd.length());
		if (!tmp.empty())
			params.push_back(tmp);
	}
	else if (cmd.substr(0, 8) == "PRIVMSG ")
	{
		cd = "PRIVMSG";
		std::string tmp = cmd.substr(8, cmd.length());
		size_t n = tmp.find(" ");
		if (n != std::string::npos)
		{
			std::string tmp2 = tmp.substr(0, n);
			std::string tmp3 = tmp.substr(n + 1, tmp.length());
			params.push_back(tmp2);
			params.push_back(tmp3);
		}
		else if (!tmp.empty())
			params.push_back(tmp);

	}
	else if (cmd.find(":") != std::string::npos)
	{
		colon = true;
		std::vector<std::string> first;
		rslt = splitBy(cmd, ":");
		if ((*rslt.begin()).find(" ") != std::string::npos)
		{
			first = splitBy(*rslt.begin(), " ");
			pop_front<std::string>(rslt);
			cd = *(first.begin());
			if (1 < first.size())
			{
				pop_front<std::string>(first);
				addElementsVector(&params, first);
				addElementsVector(&params, rslt);
			}
		}
		else
		{
			cd = *(rslt.begin());
			pop_front<std::string>(rslt);
			addElementsVector(&params, rslt);
		}
	}
	else
	{
		colon = false;
		rslt = splitBy(cmd, " ");
		cd = *(rslt.begin());
		if (rslt.size() > 1)
		{
			pop_front<std::string>(rslt);
			if (!rslt.empty())
				params = rslt;
		}
	}
#ifdef DEBUG
	std::cout << "Command : " << cd << std::endl;
	std::cout << "Params : ";
	print_debug(params);
	std::cout << std::endl;
#endif
	command->push_back(Commands(cd, prefix, params, colon));
}

std::vector<Commands> manageMultipleCommands(std::vector<std::string> listOfCommands)
{
	std::vector<Commands>              commandsToExecute;
	std::vector<std::string>::iterator it;
	for (it = listOfCommands.begin(); it != listOfCommands.end(); it++)
	{
		if (it->size() < MAX_CMD_LGTH)
			parseCmd(*it, &commandsToExecute);
		else
			std::cerr << *it << "command too long to be executed" << std::endl;
	}
	return (commandsToExecute);
}
