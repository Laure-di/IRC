#include "../includes/Class/Channel.hpp"

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

