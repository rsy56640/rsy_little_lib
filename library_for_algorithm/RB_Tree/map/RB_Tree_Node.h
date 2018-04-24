#pragma once
#ifndef _RB_TREE_NODE_H
#define _RB_TREE_NODE_H
#include <memory>

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		using _RB_Tree_color_type = bool;
		const _RB_Tree_color_type _RB_Tree_red = false;
		const _RB_Tree_color_type _RB_Tree_black = true;


		template<typename _Ty>
		struct RB_Tree_Node
		{
			using color_type = typename _RB_Tree_color_type;
			using node_type = typename RB_Tree_Node<_Ty>;
			using link_type = typename RB_Tree_Node<_Ty>*;
			using RBTreeNode_ptr = typename std::shared_ptr<node_type>;

			color_type color;
			RBTreeNode_ptr parent;
			RBTreeNode_ptr left;
			RBTreeNode_ptr right;
			_Ty value_field;			//node value


			/*
			RB_Tree_Node()
				:color(_RB_Tree_red),
				parent(nullptr),
				left(nullptr),
				right(nullptr),
				value_field(_Ty{})	//WTF!!!
			{}			//shared_ptr cast failure leads to the refactor of the project,
						//so I use only node_type for every node,
						//but that means NIL has a value_field.
						//So it requests the _Ty has a default constructor.
			*/


			RB_Tree_Node(const _Ty& value = _Ty{})
				:color(_RB_Tree_red),
				parent(nullptr),
				left(nullptr),
				right(nullptr),
				value_field(value)
			{}



		};

	}

}

#endif // !_RB_TREE_NODE_H
