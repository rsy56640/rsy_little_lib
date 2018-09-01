#pragma once
#ifndef _SKIPLISTEXCEPTION_H
#define _SKIPLISTEXCEPTION_H
#include <exception>
#include <string>

namespace RSY_TOOL::SkipList
{

	class SkipListException :std::exception
	{
	public:
		SkipListException(std::string message) :_msg(message) {}
		virtual char const* what() const { return _msg.c_str(); }
	private:
		std::string _msg;
	};

}//end namespace RSY_TOOL::SkipList


#endif // !_SKIPLISTEXCEPTION_H
