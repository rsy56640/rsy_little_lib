#pragma once
#ifndef _B_TREE_EXCEPTION_H
#define _B_TREE_EXCEPTION_H
#include <iostream>
#include <exception>

namespace RSY_TOOL
{

	namespace BTree
	{

		class BTreeException :public std::exception
		{

		public:

			BTreeException(const char* msg)
				:_msg(msg)
			{}

			const char* what()
			{
				return _msg;
			}

		private:

			const char* _msg;

			friend std::ostream& operator<<(std::ostream& os, const BTreeException& e)
			{
				os << e._msg;
				return os;
			}

		};//end class BTreeException

	}//end namespace BTree

}//end namespace RSY_TOOL

#endif // !_B_TREE_EXCEPTION_H
