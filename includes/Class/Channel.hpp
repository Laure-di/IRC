#include <string>
#include <deque>
#include "User.hpp"

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		char _mode;
		std::deque<User*>	_usersOnChannel;
		std::deque<User*>	_usersBanned;

	public:
		std::string get_name();
		void set_name(std::string name);
		std::string get_topic();
		void set_topic(std::string topic);

};
