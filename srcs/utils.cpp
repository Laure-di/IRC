#include <algorithm>
#include <iostream>
#include <string>
#include <deque>

#include "../includes/Class/Server.hpp"


std::deque<std::string>		split(std::string toSplit, std::string delimiter)
{
	std::deque<std::string>	result;
	size_t	start, end = delimiter.size() * -1;
	std::string				toAdd;

	do
	{
		start = end + delimiter.size();
		end = toSplit.find(delimiter, start);
		toAdd = toSplit.substr(start, end - start);
		if (!toAdd.empty())
			result.push_back(toAdd);

	} while (end != std::string::npos);

	return (result);
}

void	addElementsDeque(std::deque<std::string> *list, std::deque<std::string> toAdd)
{
	std::deque<std::string>::iterator	it;
	for (it = toAdd.begin(); it != toAdd.end(); it++)
		list->push_back(*it);
}

