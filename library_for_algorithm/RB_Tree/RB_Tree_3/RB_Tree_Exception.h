#pragma once
#ifndef _RB_TREE_EXCEPTION_H
#define _RB_TREE_EXCEPTION_H
#include <string>
#include <iostream>

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		class RB_Tree_Exception
		{

		public:

			RB_Tree_Exception(const std::string& msg)
				:_msg(msg)
			{}

		private:

			std::string _msg;

			friend std::ostream& operator<<(std::ostream&, const RB_Tree_Exception&);

		};

		std::ostream& operator<<(std::ostream& out, const RB_Tree_Exception& e)
		{
			out << e._msg;
			return out;
		}


	}

}

#endif // !_RB_TREE_EXCEPTION_H
