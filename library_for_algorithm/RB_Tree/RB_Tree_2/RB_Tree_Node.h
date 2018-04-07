#pragma once
#ifndef _RB_TREE_NODE_H
#define _RB_TREE_NODE_H

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		using _RB_Tree_color_type = bool;
		const _RB_Tree_color_type _RB_Tree_red = false;
		const _RB_Tree_color_type _RB_Tree_black = true;


		struct RB_Tree_Node_Base
		{
			using base_ptr = typename RB_Tree_Node_Base*;
			using color_type = typename _RB_Tree_color_type;

			color_type color;
			base_ptr parent;
			base_ptr left;
			base_ptr right;

			RB_Tree_Node_Base()
				:color(_RB_Tree_red), parent(nullptr), left(nullptr), right(nullptr)
			{}

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


		template<typename _Ty>
		struct RB_Tree_Node : public RB_Tree_Node_Base
		{
			using RBTreeNode_ptr = typename std::shared_ptr<RB_Tree_Node<_Ty> >;
			using link_type = typename RB_Tree_Node<_Ty>*;
			_Ty value_field;			//node value
			RB_Tree_Node(const _Ty& value)
				:RB_Tree_Node_Base(), value_field(value)
			{}
		};


	}

}

#endif // !_RB_TREE_NODE_H
