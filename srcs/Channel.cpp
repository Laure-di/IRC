#include "../includes/include.hpp"

Channel::Channel(Server *server, std::string name, Client *creator) : _server(server), _name(name), _mode(NONE)
{
	std::string nickname = creator->getNickname();
	_clients[nickname] = creator;
	_clientsMode[nickname] = CREATOR;
}

std::string Channel::getName(void)
{
	return _name;
}

std::string Channel::getChannelStatus(void)
{
	if (getMode() & SECRET)
		return "@";
	if (getMode() & PRIVATE)
		return "*";
	return "=";
}

std::vector<Client *> Channel::getAllClients(void) const
{
	std::vector<Client *>                           list;
	std::map<std::string, Client *>::const_iterator it;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		list.push_back(it->second);
	}
	return (list);
}

void Channel::setName(std::string name)
{
	_name = name;
}

std::string Channel::getTopic(void)
{
	return _topic;
}

std::map<std::string, Client *> Channel::getClients(void) const
{
	return _clients;
}

void Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void Channel::clearTopic(void)
{
	_topic.clear();
}

Client *Channel::getClientByNickname(const std::string nickname)
{
	std::map<std::string, Client *>::const_iterator cit;
	cit = _clients.find(nickname);
	if (cit != _clients.end())
		return cit->second;
	return 0;
}

bool Channel::checkOperatorByNickname(std::string nickname)
{
	std::map<std::string, Client *>::const_iterator cit;
	cit = _clients.find(nickname);
	if (cit == _clients.end())
		return false;
	return _clientsMode[nickname] & OPERATOR || _clientsMode[nickname] & CREATOR;
}

bool Channel::checkVoiceByNickname(std::string nickname)
{
	std::map<std::string, Client *>::const_iterator cit;
	cit = _clients.find(nickname);
	if (cit == _clients.end())
		return false;
	return _clientsMode[nickname] & OPERATOR || _clientsMode[nickname] & CREATOR || _clientsMode[nickname] & VOICE;
}

void Channel::addClient(int socket)
{
	Client     *client = _server->getClientByFd(socket);
	std::string nickname = client->getNickname();
	_clients[nickname] = client;
	_clientsMode[nickname] = 0;
}

void Channel::remClient(std::string nickname)
{
	_clients.erase(nickname);
	_clientsMode.erase(nickname);
}

void Channel::sendMsg(std::string message)
{
	std::map<std::string, Client *>::iterator clientIterator;
	for (clientIterator = _clients.begin(); clientIterator != _clients.end(); clientIterator++)
	{
		Client *client = clientIterator->second;
		if (client)
			_server->sendMsg(message, client->getFd());
	}
}

void Channel::sendMsg(std::string message, Client *sender)
{
	std::string nickname = sender->getNickname();
	int         fd = sender->getFd();
	if (((_mode & OUTSIDE) && !(getClientByNickname(nickname))) ||
		((_mode & MODERATED) && !(checkVoiceByNickname(nickname))))
		return _server->sendMsg(ERR_CANNOTSENDTOCHAN(nickname), fd);
	sendMsg(message, fd);
}

void Channel::sendMsg(std::string message, int sender)
{
	std::map<std::string, Client *>::iterator clientIterator;
	for (clientIterator = _clients.begin(); clientIterator != _clients.end(); clientIterator++)
	{
		Client *client = clientIterator->second;
		if (client)
		{
			int recipient = client->getFd();
			if (recipient != sender)
				_server->sendMsg(message, recipient);
		}
	}
}

void Channel::sendJoin(Client *client)
{
	sendMsg(":" + client->getFullIdentifier() + " JOIN " + _name + "\r\n");
}

void Channel::sendPart(Client *client, std::string leaveMessage)
{
	std::string msg;
	if (!leaveMessage.empty())
		msg = ":" + client->getFullIdentifier() + " PART " + _name + " :" + leaveMessage + "\r\n";
	else
		msg = ":" + client->getFullIdentifier() + " PART " + _name + " :" + client->getNickname() + "\r\n";
	sendMsg(msg);
}

void Channel::sendTopic(int socket)
{
	_server->sendMsg(RPL_TOPIC(_name, _topic), socket);
}

void Channel::sendListOfNames(const int socket)
{
	std::vector<std::string>                  clientNicknames;
	std::map<std::string, Client *>::iterator clientIterator;
	for (clientIterator = _clients.begin(); clientIterator != _clients.end(); clientIterator++)
		clientNicknames.push_back(clientIterator->second->getNicknameWithPrefix(this));
	std::string        delim = " ";
	std::ostringstream joinedClientNicknames;
	std::copy(clientNicknames.begin(), clientNicknames.end(),
			  std::ostream_iterator<std::string>(joinedClientNicknames, delim.c_str()));
	std::string channelStatus = getChannelStatus();
	_server->sendMsg(RPL_NAMREPLY(channelStatus, _name, joinedClientNicknames.str()), socket);
	_server->sendMsg(RPL_ENDOFNAMES(_name), socket);
}

size_t Channel::getNumberOfUsers(void)
{
	return _clients.size();
}

std::string Channel::getModeStr(void) const
{
	std::string res = "+";
	if (_mode & INVITATION)
		res += "i";
	if (_mode & KEY)
		res += "k";
	if (_mode & MODERATED)
		res += "m";
	if (_mode & OUTSIDE)
		res += "n";
	if (_mode & PRIVATE)
		res += "p";
	if (_mode & SECRET)
		res += "s";
	if (_mode & TOPIC)
		res += "t";
	if (_mode & LIMIT)
		res += "l";
	return res;
}

size_t Channel::getMaxLimitOfUsers(void)
{
	return _maxLimit;
}

void Channel::changeNickname(std::string oldNickname, std::string newNickname)
{
	std::map<std::string, Client *>::iterator it = _clients.find(oldNickname);
	if (it != _clients.end())
	{
		std::swap(_clients[newNickname], it->second);
		_clients.erase(it);
	}
	std::map<std::string, unsigned>::iterator it2 = _clientsMode.find(oldNickname);
	if (it2 != _clientsMode.end())
	{
		std::swap(_clientsMode[newNickname], it2->second);
		_clientsMode.erase(it2);
	}
}

void Channel::addMode(int mask)
{
	_mode |= mask;
}

void Channel::remMode(int mask)
{
	_mode &= ~mask;
}

void Channel::modMode(int mask, bool add)
{
	if (add)
		return addMode(mask);
	remMode(mask);
}

void Channel::modKey(bool add, std::string key)
{
	if (add)
		_keyHash = hasher(key.c_str());
	modMode(KEY, add);
}

void Channel::modLimit(bool add, std::string max)
{
	if (add)
		_maxLimit = toInt(max.c_str());
	modMode(LIMIT, add);
}

unsigned Channel::getMode()
{
	return _mode;
}

unsigned Channel::getMode(int socket)
{
	Client *client = _server->getClientByFd(socket);
	int     mode = _clientsMode[client->getNickname()];
	return mode;
}

void Channel::modClientMode(const int socket, std::string nickname, unsigned mask, bool add)
{
	if (!getClientByNickname(nickname))
		return _server->sendMsg(ERR_USERNOTINCHANNEL(nickname, _name), socket);
	if (add)
		_clientsMode[nickname] |= mask;
	else
		_clientsMode[nickname] &= ~mask;
}

void Channel::modClientMask(unsigned type, bool add, std::string mask)
{
	std::vector<std::string> *masksList;
	std::string               maskName;
	switch (type)
	{
	case 'b':
		maskName = "ban";
		masksList = &_banMasks;
		break;
	case 'e':
		maskName = "ban exception";
		masksList = &_banExceptionMasks;
		break;
	case 'I':
		maskName = "invitation";
		masksList = &_inviteMasks;
		break;
	default:
		return;
	}
	if (!mask.empty())
	{
		if (add)
			masksList->push_back(mask);
		else
			masksList->erase(std::remove(masksList->begin(), masksList->end(), mask), masksList->end());
	}
	std::string        delim = ",";
	std::ostringstream joinedMasksStream;
	std::copy(masksList->begin(), masksList->end(),
			std::ostream_iterator<std::string>(joinedMasksStream, delim.c_str()));
	std::string joinedMasks = joinedMasksStream.str();
	joinedMasks = joinedMasks.substr(0, joinedMasks.size() - 1);
	sendMsg("The " + maskName + " mask is now : " + joinedMasks + "\r\n");
}

bool Channel::isInvited(std::string nickname)
{
	std::vector<std::string>::const_iterator cit;
	for (cit = _inviteMasks.begin(); cit != _inviteMasks.end(); cit++)
	{
		if (strmatch(nickname, *cit))
			return true;
	}
	return false;
}

bool Channel::isBanned(std::string nickname)
{
	std::vector<std::string>::const_iterator cit;
	for (cit = _banMasks.begin(); cit != _banMasks.end(); cit++)
	{
		if (strmatch(nickname, *cit))
			return true;
	}
	return false;
}

bool Channel::isExcepted(std::string nickname)
{
	std::vector<std::string>::const_iterator cit;
	for (cit = _banExceptionMasks.begin(); cit != _banExceptionMasks.end(); cit++)
	{
		if (strmatch(nickname, *cit))
			return true;
	}
	return false;
}

bool Channel::checkPassword(std::string key)
{
	return (hasher(key.c_str()) == _keyHash);
}

void Channel::sendInfo(const int socket)
{
	_server->sendMsg(RPL_NAMREPLY(_name, toString(getNumberOfUsers()), _topic), socket);
}

std::string Channel::getClientPrefix(const Client *client)
{
	std::string                                     nickname = client->getNickname();
	std::map<std::string, Client *>::const_iterator cit;
	cit = _clients.find(nickname);
	if (cit == _clients.end())
		return ("");
	// IRSSI do not treat correctly "~" for creator
	if (_clientsMode[nickname] & CREATOR)
		return "@";
	if (_clientsMode[nickname] & OPERATOR)
		return "@";
	if (_clientsMode[nickname] & VOICE)
		return "+";
	return ("");
}
