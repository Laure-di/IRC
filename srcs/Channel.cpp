#include "../includes/Class/Channel.hpp"

Channel::Channel(std::string name, User* creator): _name(name)
{
	_userOperator[creator->getNickname()] = creator;
	_usersOnChannel[creator->getNickname()] = creator;
}

std::string Channel::get_name(void) {
	return _name;
}

void Channel::set_name(std::string name) {
	_name = name;
}

std::string Channel::get_topic(void) {
	return _topic;
}

void Channel::set_topic(std::string topic) {
	_topic = topic;
}

void Channel::clear_topic(void) {
	_topic.clear();
}

User *Channel::findUserByNickname(const std::string nickname)
{
	return _usersOnChannel[nickname];
}

User *Channel::findOperatorByNickname(const std::string nickname)
{
	return _userOperator[nickname];
}

User *Channel::findBannedUserByNickname(const std::string nickname)
{
	return _usersBanned[nickname];
}
