#pragma once
#ifndef _RB_TREE_ITERATOR_H
#define _RB_TREE_ITERATOR_H
#include "RB_Tree_Node.h"
#include "RB_Tree_Exception.h"
#include <iterator>
#ifndef _My_ITERATOR			//suppose I will implement the Iterator class in the future
#define _My_ITERATOR ::std::
#endif // !_My_ITERATOR


namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		template<class _Ty>
		struct RB_Tree_Iterator_Base
		{

			using color_type = typename RB_Tree_Node<_Ty>::color_type;
			using node_type = typename RB_Tree_Node<_Ty>::node_type;
			using link_type = typename RB_Tree_Node<_Ty>::link_type;
			using RBTreeNode_ptr = typename RB_Tree_Node<_Ty>::RBTreeNode_ptr;
			using size_type = _My_ITERATOR size_t;
			using value_type = _Ty;
			using difference_type = _My_ITERATOR ptrdiff_t;
			using reference = _Ty&;
			using pointer = _Ty*;
			//using iterator_category = typename  _My_ITERATOR bidirectional_iterator_tag;
			using iterator_category = typename   RB_Tree_Iterator_Base<_Ty>;
			using RBTreeNode_ptr = typename RB_Tree_Node<_Ty>::RBTreeNode_ptr;
			using RBTEx = RB_Tree_Exception;

			RBTreeNode_ptr node;


			/*
			 * iterator increment
			**/
			void increment()
			{
				if (isNIL(node))
					throw RBTEx("iterator out of range.", RBTEx::iterator_ex);
				//right child exits
				if (!isNIL(node->right))
				{
					node = node->right;
					while (!isNIL(node->left))
						node = node->left;
				}
				//no right child
				else {
					RBTreeNode_ptr temp = node->parent;
					//trace back until node is not a right child
					while (node == temp->right)
					{
						node = temp;
						temp = temp->parent;
					}
					//assign temp to node,
					//regardless of wether or not temp is NIL.
					node = temp;
				}
			}//end function increment()


			/*
			 * iterator decremnt
			**/
			void decrement()
			{
				//when node is T.NIL
				if (node->color == _RB_Tree_red && node->parent->parent == node)
					node = node->right;

				//
				if (node->color = )






			}

		};//end class RB_Tree_Iterator_Base

		template<typename _Ty>
		struct RB_Tree_Iterator :public RB_Tree_Iterator_Base<_Ty>
		{

			using self_type = RB_Tree_Iterator<_Ty>;
			using color_type = typename RB_Tree_Iterator_Base<_Ty>::color_type;
			using node_type = typename RB_Tree_Iterator_Base<_Ty>::node_type;
			using link_type = typename RB_Tree_Iterator_Base<_Ty>::link_type;
			using RBTreeNode_ptr = typename RB_Tree_Iterator_Base<_Ty>::RBTreeNode_ptr;
			using size_type = typename RB_Tree_Iterator_Base<_Ty>::size_type;
			using value_type = typename RB_Tree_Iterator_Base<_Ty>::value_type;
			using difference_type = typename RB_Tree_Iterator_Base<_Ty>::difference_type;
			using reference = typename RB_Tree_Iterator_Base<_Ty>::reference;
			using pointer = typename RB_Tree_Iterator_Base<_Ty>::pointer;
			using iterator_category = typename RB_Tree_Iterator_Base<_Ty>::iterator_category;
			using RBTreeNode_ptr = typename RB_Tree_Iterator_Base<_Ty>::RBTreeNode_ptr;
			using RBTEx = RB_Tree_Exception;


			using iterator_type = RB_Tree_Iterator<_Ty>;


			RB_Tree_Iterator() = default;

			RB_Tree_Iterator(RBTreeNode_ptr _node) { node = _node; }

			RB_Tree_Iterator(const self_type& it) { node = it.node; }


			reference operator*() const noexcept
			{
				return node->value_field;
			}

			pointer operator->()const  noexcept
			{
				return &(operator*());
			}

			self_type& operator++()
			{
				try {
					increment();
				}
				catch (const RBTEx& e) {
					std::cout << e << std::endl;
				}
				return *this;
			}

			self_type operator++(int)
			{
				auto temp{ *this };
				try {
					increment();
				}
				catch (const RBTEx& e) {
					std::cout << e << std::endl;
				}
				return temp;
			}

			self_type& operator--()
			{
				try {
					decrement();
				}
				catch (const RBTEx& e) {
					std::cout << e << std::endl;
				}
				return *this;
			}

			self_type operator--(int)
			{
				auto temp{ *this };
				try {
					decrement();
				}
				catch (const RBTEx& e) {
					std::cout << e << std::endl;
				}
				return temp;
			}


		};//end class RB_Tree_Iterator

	}//end namepsace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RB_TREE_ITERATOR_H
