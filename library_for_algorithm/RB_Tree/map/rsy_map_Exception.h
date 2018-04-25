#pragma once
#ifndef _RSY_MAP_EXCEPTION_H
#define _RSY_MAP_EXCEPTION_H
#include <exception>
#include <string>
#include <iostream>
#include "RB_Tree_Exception.h"
namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		class map_Exception : public _STD exception
		{

			using RBTEx = RB_Tree_Exception;

		public:

			map_Exception(const char* msg)
				:_msg(msg), _mode(0)
			{}

			map_Exception(const _STD string& msg)
				:_msg(msg.c_str()), _mode(0)
			{}

			map_Exception(const RBTEx& e)
				:_msg(e.message().c_str()), _mode(e.mode())
			{}

			map_Exception(const map_Exception&) = default;

			map_Exception& operator=(const map_Exception&) & = default;


			char const* what()const noexcept
			{
				return _msg ? _msg : "Unknown exception";
			}

			friend _STD ostream& operator<<(_STD ostream& os, const map_Exception& e)
			{
				os << e._msg;
				return os;
			}

		private:

			const char* _msg;

			int _mode;


		};//end class map_Exception

	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RSY_MAP_EXCEPTION_H
