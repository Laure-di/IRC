#include "../includes/include.hpp"

Channel::Channel(Server *server, std::string name, Client* creator): _server(server), _name(name)
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

void Channel::addClient(int socket)
{
	Client *client = _server->getClientByFd(socket);
	_clientsOnChannel[client->getNickname()] = client;
}

void Channel::deleteClient(std::string nickname)
{
	_clientsOnChannel.erase(nickname);
}

void Channel::deleteOperator(std::string nickname)
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

void Channel::sendJoin(std::string fullClientName)
{
	sendMsg(fullClientName + " JOIN " + _name);
}

void Channel::sendTopic(int socket)
{
	_server->sendMsg(RPL_TOPIC(_name, _topic), socket);
}

void Channel::sendListOfNames(int socket)
{
	// Add Channel Status
	// Add Nickname prefix for client mode
	std::vector<std::string> clients;
	std::map<std::string, Client*>::iterator clientIterator;
	for (clientIterator = _clientsOnChannel.begin(); clientIterator != _clientsOnChannel.end(); clientIterator++)
		clients.push_back(clientIterator->second->getNickname());
	std::string delim = " ";
	std::ostringstream joined;
	std::copy(clients.begin(), clients.end(), std::ostream_iterator<std::string>(joined, delim.c_str()));
	std::string channelStatus = "=";
	_server->sendMsg(RPL_NAMREPLY(channelStatus, _name, joined.str()), socket);
}

size_t Channel::getNumberOfUsers(void)
{
	return _clientsOnChannel.size();
}

std::vector<Client*>			Channel::getAllClients(void)const
{
	std::vector<Client*> allClients;
	for (std::map<std::string, Client*>::const_iterator it = _clientsOnChannel.begin(); it!= _clientsOnChannel.end(); it++)
	{
		allClients.push_back(it->second);
	}

	return (allClients);
}

void Channel::changeNickname(std::string oldNickname, std::string newNickname)
{
	std::map<std::string, Client*>::iterator it = _clientsOnChannel.find(oldNickname);
	if (it != _clientsOnChannel.end())
	{
		std::swap(_clientsOnChannel[newNickname], it->second);
		_clientsOnChannel.erase(it);
	}
	it = _clientsOperator.find(oldNickname);
	if (it != _clientsOperator.end())
	{
		std::swap(_clientsOperator[newNickname], it->second);
		_clientsOperator.erase(it);
	}
	it = _clientsBanned.find(oldNickname);
	if (it != _clientsBanned.end())
	{
		std::swap(_clientsBanned[newNickname], it->second);
		_clientsBanned.erase(it);
	}
}
