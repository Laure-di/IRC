#include "../includes/Class/Channel.hpp"

Channel::Channel(std::string name, Client* creator): _name(name)
{
	_clientsOperator[creator->getNickname()] = creator;
	_clientsOnChannel[creator->getNickname()] = creator;
}

std::string Channel::getName(void) {
	return _name;
}

void Channel::setName(std::string name) {
	_name = name;
}

std::string Channel::getTopic(void) {
	return _topic;
}

void Channel::setTopic(std::string topic) {
	_topic = topic;
}

void Channel::clearTopic(void) {
	_topic.clear();
}

Client *Channel::findClientByNickname(const std::string nickname)
{
	return _clientsOnChannel[nickname];
}

Client *Channel::findOperatorByNickname(const std::string nickname)
{
	return _clientsOperator[nickname];
}

Client *Channel::findBannedUserByNickname(const std::string nickname)
{
	return _clientsBanned[nickname];
}

void Channel::kickClient(std::string nickname)
{
	_clientsOnChannel.erase(nickname);
}

void Channel::kickOperator(std::string nickname)
{
	_clientsOperator.erase(nickname);
}

void Channel::sendMsg(std::string message)
{
	std::map<std::string, Client*>::iterator clientIterator;
	for (clientIterator = _clientsOnChannel.begin(); clientIterator != _clientsOnChannel.end(); clientIterator++)
	{
		Client *client = clientIterator->second;
		send(client->getFd(), message.c_str(), message.length(), MSG_DONTWAIT);
	}
}
