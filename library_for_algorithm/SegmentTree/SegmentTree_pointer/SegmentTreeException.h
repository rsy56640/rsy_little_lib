#pragma once
#ifndef _SEGMENTTREEEXCEPTION_H
#define _SEGMENTTREEEXCEPTION_H
#ifndef _STD
#define _STD ::std::
#endif // !_STD
#include <exception>
#include <string>

template<class _Ty> class SegmentTreeException :public exception
{

public:
	SegmentTreeException(std::string msg)
		:_msg(msg) {}

	const char* what() const noexcept
	{
		return "SegmentTree Exception";
	}

private:

	std::string _msg;


	friend
		std::ostream& operator<<(std::ostream& os, const SegmentTreeException<_Ty>& e)
	{
		os << e._msg;
		return os;
	}

};

#endif // !_SEGMENTTREEEXCEPTION_H
