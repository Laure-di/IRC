#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP 

#include "include.hpp"
class numericRepliesError: public std::exception{
	public:
		~numericRepliesError()throw(){}
		void sendReply(const NumericReplies _reply, const int _fd, Server *_server) const throw()
		{
			return (_server->sendMsg(_reply, _fd));
		}

};

#endif
