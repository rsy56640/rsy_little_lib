#pragma once
#ifndef _RB_TREE_H
#define _RB_TREE_H

#include "RB_Tree_Iterator.h"

namespace RSY_TOOL
{


	namespace MY_RB_Tree
	{

		//template class for RB_Tree
		template<class Key, class Value, class KeyOfValue,
			class Compare, class Alloc = alloc>		//alloc has not been finished
			class RB_Tree							//因为这个原因，先把这个版本搁置起来
		{

		protected:
			using void_ptr = void*;
			using base_ptr = typename RB_Tree_Node_Base::base_ptr;
			using RB_Tree_Node = RB_Tree_Node<Value>;
		//	using RB_Tree_Node_Allocator = simple_alloc<RB_Tree_Node, Alloc>;
			using color_type = _RB_Tree_color_type;
		public:






		protected:






		};



	}

}

#endif // !_RB_TREE_H
