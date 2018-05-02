#include "B_Tree_iterator.h"

namespace RSY_TOOL
{

	namespace BTree
	{

		/*
		* find the minimum key in the subtree pNode
		**/
		_STD pair<typename B_Tree_iterator_base::base_ptr, int> B_Tree_iterator_base::B_Tree_minimum(base_ptr pNode)
		{

		}


		/*
		 * find the maximum key in the subtree pNode
		**/
		_STD pair<typename B_Tree_iterator_base::base_ptr, int> B_Tree_iterator_base::B_Tree_maximum(base_ptr pNode)
		{


		}


		/*
		 * find the precursor of the key
		**/
		_STD pair<typename B_Tree_iterator_base::base_ptr, int> B_Tree_iterator_base::decrement(base_ptr pNode, const int index)
		{

			//trivial case
			if (index != 0)
				return _STD make_pair(pNode, index - 1);

			//precursor is in the other node




		}


		/*
		 * find the successor of the key
		**/
		_STD pair<typename B_Tree_iterator_base::base_ptr, int> B_Tree_iterator_base::increment(base_ptr pNode, const int index)
		{

			//trivial case
			if (index != pNode->_key_size - 1)
				return _STD make_pair(pNode, index + 1);

			//successor is in the other node


		}


	}//end namespace BTree

}//end namespace RSY_TOOL
