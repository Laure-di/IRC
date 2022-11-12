#include "../includes/Class/Channel.hpp"

Channel::Channel(std::string name, Client* creator): _name(name)
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

Client *Channel::findUserByNickname(const std::string nickname)
{
	return _usersOnChannel[nickname];
}

Client *Channel::findOperatorByNickname(const std::string nickname)
{
	return _userOperator[nickname];
}

Client *Channel::findBannedUserByNickname(const std::string nickname)
{
	return _usersBanned[nickname];
}

void Channel::kickUser(std::string nickname)
{
	_usersOnChannel.erase(nickname);
}
