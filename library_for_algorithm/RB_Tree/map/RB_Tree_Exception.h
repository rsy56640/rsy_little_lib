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

			static constexpr int erase_ex = 0;
			static constexpr int iterator_ex = 1;

			RB_Tree_Exception(const std::string& msg, int mode)
				:_msg(msg), _mode(mode)
			{}

			std::string message() const
			{
				return _msg;
			}

			int mode() const noexcept
			{
				return _mode;
			}

		private:

			std::string _msg;

			int _mode;

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
