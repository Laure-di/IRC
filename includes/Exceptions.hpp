#pragma once //TODO change to ifndef
class serverError: public std::exception
{
	private:
		std::string	_msg;
	public:
		serverError(const std::string type, const std::string error_msg) : _msg(type + ": " + error_msg){}
		~serverError()throw(){}
		virtual const char* what() const throw()
		{
			return (this->_msg.c_str());
		}
};
