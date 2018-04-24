#pragma once
#ifndef _RSY_MAP_EXCEPTION_H
#define _RSY_MAP_EXCEPTION_H
#include <exception>
#include <string>
namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		class map_Exception : public _STD exception
		{

		public:

			map_Exception(const char* msg)
				:_msg(msg)
			{}

			map_Exception(const _STD string& msg)
				:_msg(msg.c_str())
			{}

			map_Exception(const map_Exception&) = default;

			map_Exception& operator=(const map_Exception&) & = default;


			char const* what()const noexcept
			{
				return _msg ? _msg : "Unknown exception";
			}


		private:

			const char* _msg;

		};//end class map_Exception

	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RSY_MAP_EXCEPTION_H
