#pragma once
#ifndef _BIT_EXCEPTION_H
#define _BIT_EXCEPTION_H
#include <exception>
#include <string>
#include <iostream>

class BIT_Exception
{

public:

	BIT_Exception(std::string msg)
		:_msg(msg) {}


private:

	_STD string _msg;


	friend
		std::ostream& operator<<(std::ostream& os, const BIT_Exception& e)
	{
		os << e._msg;
		return os;
	}


};
#endif // !_BIT_EXCEPTION_H
