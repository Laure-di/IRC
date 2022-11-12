#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
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

/**
 * @brief Check if a character is a letter
 */
bool isLetter(const char c) {
	return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}

/**
 * @brief Check if a character is a digit
 */
bool isDigit(const char c) {
	return '0' <= c && c <= '9';
}

/**
 * @brief Check if a character is a hexadecimal digit letters
 */
bool isHexDigit(const char c) {
	return (isDigit(c) || ('A' <= c && c <= 'F'));
}

/**
 * @brief Check if a character is an hyphen
 */
bool isHyphen(const char c) {
	return c == '-';
}

/**
 * @brief Check if a character is among %x5B-60 / %x7B-7
 */
bool isSpecial(const char c) {
	std::string specialCharacters = ";[]`_^{|}\\";
	return specialCharacters.find(c) != std::string::npos;
}

/**
 * @brief Convert int to string
 */
std::string toString(const int nb) {
	std::ostringstream ss;
	ss << nb;
	return ss.str();
}

/**
 * @brief Parse string to int
 */
int toInt(std::string string) {
	int nb;
	std::istringstream(string) >> nb;
	return nb;
}

/**
 * @brief Check that username match ( letter / special ) *8( letter / digit / special / "-" )
 */
bool checkUsername(std::string username) {
	std::string forbiddenCharacters = "\0\n\r@";
	for (size_t i = 0; i < forbiddenCharacters.size(); i++) {
		if (username.find(forbiddenCharacters[i]) != std::string::npos)
			return false;
	}
	return true;
};

/**
 * @brief Check that nickname match ( letter / special ) *8( letter / digit / special / "-" )
 */
bool checkNickname(std::string nickname) {
	if (nickname.empty())
		return false;
	if (nickname.length() > 9)
		return false;
	char firstChar = nickname[0];
	if (!(isLetter(firstChar) || isSpecial(firstChar)))
		return false;
	for (size_t i = 1; i < nickname.size(); i++) {
		char nChar = nickname[i];
		if (!(isLetter(nChar) || isDigit(nChar) || isSpecial(nChar) || isHyphen(nChar)))
			return false;
	}
	return true;
};

/**
 * @brief Quick and dirty hash https://stackoverflow.com/a/107657
 */
unsigned hasher(const char *s)
{
	unsigned hash = 42;
	while (*s)
		hash = hash * 101 + *s++;
	return hash;
}

/**
 * @brief Check chanstring
 *
 * chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
 * chanstring =/ %x2D-39 / %x3B-FF; any octet except NUL, BELL, CR, LF, " ", "," and ":"
 */
bool checkChanstring(std::string name) {
	std::string forbiddenCharacters = "\0\a\n\r ,:";
	for (size_t i = 0; i < forbiddenCharacters.size(); i++) {
		if (name.find(forbiddenCharacters[i]) != std::string::npos)
			return false;
	}
	return true;
};

/**
 * @brief Check channel name
 *
 * channel =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring [ ":" chanstring ]
 * At the moment, only check channel = ("#" / "&") chanstring
 */
bool checkChannelName(std::string name) {
	if (name.length() < 2)
		return false;
	char firstChar = name[0];
	if (firstChar != '#' && firstChar != '&')
		return false;
	name.erase(0, 1);
	return checkChanstring(name);
};


void	addElementsDeque(std::deque<std::string> *list, std::deque<std::string> toAdd)
{
	std::deque<std::string>::iterator	it;
	for (it = toAdd.begin(); it != toAdd.end(); it++)
		list->push_back(*it);
}

