#pragma once
#ifndef _RB_TREE_NODE_H
#define _RB_TREE_NODE_H

namespace MY_RB_Tree
{

	using _RB_Tree_color_type = bool;
	const _RB_Tree_color_type _RB_Tree_red = false;
	const _RB_Tree_color_type black = true;


	struct RB_Tree_Node_Base
	{
		using base_ptr = typename RB_Tree_Node_Base*;
		using color_type = typename _RB_Tree_color_type;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minimum(base_ptr root)
		{
			while (root->left != nullptr)
				root = root->left;
			return root;
		}

		static base_ptr maximum(base_ptr root)
		{
			while (root->right != nullptr)
				root = root->right;
			return root;
		}

	};//struct RB_Tree_Node_Base


	template<typename Value>
	struct RB_Tree_Node : public RB_Tree_Node_Base
	{
		using link_type = typename RB_Tree_Node<Value>*;
		Value value_field;			//node value
	};


}
#endif // !_RB_TREE_NODE_H
