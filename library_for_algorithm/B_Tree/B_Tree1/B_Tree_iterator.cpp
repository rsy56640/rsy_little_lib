#include "B_Tree_iterator.h"

namespace RSY_TOOL
{

	namespace BTree
	{


		/*
		 * find the precursor of the key
		 * if no precursor, an exception will be threw
		**/
		void B_Tree_iterator_base::decrement()
		{
			std::tie(_node, _index) = BTree::decrement(_node, _index);
		}


		/*
		 * find the successor of the key
		 * if no successor, an exception will be threw
		**/
		void B_Tree_iterator_base::increment()
		{
			std::tie(_node, _index) = BTree::increment(_node, _index);
		}


	}//end namespace BTree

}//end namespace RSY_TOOL
