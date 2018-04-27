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
			using iterator_category = typename  _My_ITERATOR bidirectional_iterator_tag;
			using RBTreeNode_ptr = typename RB_Tree_Node<_Ty>::RBTreeNode_ptr;
			using RBTEx = RB_Tree_Exception;

			RBTreeNode_ptr node;


			/*
			 * iterator increment operation
			**/
			void increment()
			{

				//if node is T.NIL
				//NB: it is allowed that iterator is end(), which is T.NIL.
				if (isNIL(node))
					throw RBTEx("iterator out of range.", RBTEx::iterator_ex);

				//right child exits
				if (!isNIL(node->right))
				{
					node = node->right;
					node = minimum(node);
				}


				//no right child
				else {

					//NB: the case is considered when node is root, and temp is T.NIL.
					RBTreeNode_ptr temp = node->parent;

					//trace back until node is not a right child
					while (node == temp->right)
					{
						node = temp;
						temp = temp->parent;
					}

					//assign temp to node,
					//regardless of whether or not temp is T.NIL.
					node = temp;

				}

			}//end function increment();


			/*
			 * iterator decremnt operation
			**/
			void decrement()
			{

				//if node is T.NIL
				if (isNIL(node))
					//Since the implementation has been changed a little,
					//the statement below has the correct result.
					//2018-04-25
					node = maximum(node->parent);

				//left child exists
				if (!isNIL(node->left))
				{
					node = node->left;
					node = maximum(node);
				}

				//no left child
				else {

					//NB: the case is considered when node is root, and temp is T.NIL.
					RBTreeNode_ptr temp = node->parent;

					//trace back until node is not a left child
					while (node == temp->left)
					{
						node = temp;
						temp = temp->parent;
					}

					//temp is T.NIL, 
					//which means the node is left-most of the RB_Tree.
					//throw an exception
					if (isNIL(temp))
						throw RBTEx("iterator out of range.", RBTEx::iterator_ex);

					//assign temp to node
					node = temp;

				}

			}//end function decrement();


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


			//RB_Tree_Iterator() = default;

			explicit RB_Tree_Iterator(RBTreeNode_ptr _node) { node = _node; }

			RB_Tree_Iterator(const self_type&) = default;

			RB_Tree_Iterator& operator=(const self_type&) & = default;

			bool operator==(const iterator_type& other)
			{
				return (this->node == other.node);
			}

			bool operator!=(const iterator_type& other)
			{
				return (this->node != other.node);
			}

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
					std::cout << "preposition increment error." << std::endl;
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
					std::cout << "postposition increment error." << std::endl;
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
					std::cout << "preposition decrement error." << std::endl;
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
					std::cout << "postposition decrement error." << std::endl;
					std::cout << e << std::endl;
				}
				return temp;
			}


		};//end class RB_Tree_Iterator

	}//end namepsace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RB_TREE_ITERATOR_H
