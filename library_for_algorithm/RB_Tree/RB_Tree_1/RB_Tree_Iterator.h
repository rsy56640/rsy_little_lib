/*******************************************************************************************\
*@             explainations about _RB_Tree in sgi-stl-2.91.57                              *
*@  an implementation trick is showed here:                                                 *
*       It uses a "header" node, both of which and root are mutually parent to each other,  *
*       and its left node is T.beign()--(most)left, and its right node is T.(most)right.    *
*       The "header" is T.end(), the color of which is RED!!!                               *
*                                                                                           *
*                                                                                           *
*@ RSY 2018-3-26                                                                            *
\*******************************************************************************************/
#pragma once
#ifndef _RB_TREE_ITERATOR_H
#define _RB_TREE_ITERATOR_H
#include "RB_Tree_Node.h"
#include <iterator>
#ifndef _My_ITERATOR			//suppose I will implement the Iterator class in the future
#define _My_ITERATOR ::std::
#endif // !_My_ITERATOR
//#include <iostream>
//#include <type_traits>
//#include <memory>
//#include <functional>

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		struct RB_Tree_Iterator_Base
		{
			using base_ptr = typename RB_Tree_Node_Base::base_ptr;
			using difference_type = long long;
			using iterator_category = typename  _My_ITERATOR bidirectional_iterator_tag;

			base_ptr node;

			void increment()
			{
				//right child exits
				if (node->right != nullptr)
				{
					node = node->right;
					while (node->left != nullptr)
						node = node->left;
				}
				//no right child
				else {
					base_ptr temp = node->parent;
					//trace back until node is not a right child
					while (node == temp->right)
					{
						node = temp;
						temp = temp->parent;
					}
					//@special case: no next node, so increment() does nothing.
					//	if we gonna find the next node of root,
					//	and root has no right child.
					if (node->right != temp)
						node = temp;
				}
			}

			void decrement()
			{

				//case 1: when node is header, which is RED!!!!
				if (node->color == _RB_Tree_red && node->parent->parent == node)
				{
					node = node->right;	//node==header, the right of which is most-right
					return;
				}


				//case 2: when node has left child
				if (node->left != nullptr)
				{
					base_ptr temp = node->left;
					while (temp->right != nullptr)	//find the predecessor in the inorder-sequence
						temp = temp->right;
					node = temp;
					return;
				}


				//case 3: not the root, and no left child
				base_ptr temp = node->parent;	//check out the parent node
				while (node == temp->left)			//if the current is left child
				{									//continue to go up
					node = temp;					//until current is not left child
					temp = temp->parent;
				}
				node = temp;
				//NOTA BENE: if node is most-left, then the it turns to header(T.end())
			}

		};

		template<typename Value, typename Ref, typename Ptr>
		struct RB_Tree_Iterator :public RB_Tree_Iterator_Base
		{
			using value_type = Value;
			using reference_type = Ref;
			using pointer_type = Ptr;
			using iterator_type = typename RB_Tree_Iterator<Value, Value&, Value*>;
			using self_type = typename RB_Tree_Iterator<Value, Ref, Ptr>;
			using link_type = typename RB_Tree_Node<value_type>::link_type;

			RB_Tree_Iterator() = default;

			RB_Tree_Iterator(link_type _node) { node = _node; }

			RB_Tree_Iterator(const iterator_type& it) { node = it.node; }


			reference_type operator*() const noexcept
			{
				return static_cast<link_type>(node)->value_field;	//down-cast???????
			}

			pointer_type operator->()const  noexcept
			{
				return &(operator*());
			}

			self_type& operator++()
			{
				increment();
				return *this;
			}

			self_type operator++(int)
			{
				auto temp{ *this };
				increment();
				return temp;
			}

			self_type& operator--()
			{
				decrement();
				return *this;
			}

			self_type operator--(int)
			{
				auto temp{ *this };
				decrement();
				return temp;
			}

		};

	}

}

#endif // !_RB_TREE_ITERATOR_H
