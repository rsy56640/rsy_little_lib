#pragma once
#ifndef _B_TREE_ITERATOR_H
#define _B_TREE_ITERATOR_H
#include "B_Tree_Node.h"
#include "B_Tree_Exception.h"

namespace RSY_TOOL
{

	namespace BTree
	{


		struct B_Tree_iterator_base
		{

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using base_ptr = B_Tree_Node_Base*;

			B_Tree_iterator_base(base_ptr node, int index)
				:_node(node), _index(index)
			{}

			base_ptr _node;
			int _index;

			/*
			 * find the minimum key in the subtree pNode
			**/
			_STD pair<base_ptr, int> B_Tree_minimum(base_ptr pNode);


			/*
			 * find the maximum key in the subtree pNode
			**/
			_STD pair<base_ptr, int> B_Tree_maximum(base_ptr pNode);


			/*
			 * find the precursor of the key
			**/
			_STD pair<base_ptr, int> decrement(base_ptr pNode, const int index);


			/*
			 * find the successor of the key
			**/
			_STD pair<base_ptr, int> increment(base_ptr pNode, const int index);

		};



		/*
		 * template struct for B_Tree_iterator
		**/
		template<class Key, class Value>
		struct B_Tree_iterator :public B_Tree_iterator_base
		{

			using iterator_category = typename B_Tree_iterator_base::iterator_category;
			using difference_type = typename B_Tree_iterator_base::difference_type;
			using base_ptr = typename B_Tree_iterator_base::base_ptr;
			using link_type = typename B_Tree_Node<Key, Value>*;
			using value_type = _STD pair<Key, Value>;
			using reference = value_type&;
			using pointer = value_type*;
			using self_type = B_Tree_iterator<Key, Value>;
			using iterator_type = self_type;
			using BTEx = BTreeException;


			B_Tree_iterator(base_ptr node, int index)
				:B_Tree_iterator_base(node, index)
			{}

			B_Tree_iterator(const B_Tree_iterator&) = default;

			B_Tree_iterator& operator=(const B_Tree_iterator&) & = default;

			bool operator==(const iterator_type& other)
			{
				return (this->_node == other._node && this->_index == other._index);
			}

			bool operator!=(const iterator_type& other)
			{
				return (this->_node != other._node || this->_index != other._index);
			}

			reference operator*() const noexcept
			{
				return static_cast<link_type>(_node)->data[_index]->second;
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
				catch (const BTEx& e) {
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
				catch (const BTEx& e) {
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
				catch (const BTEx& e) {
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
				catch (const BTEx& e) {
					std::cout << "postposition decrement error." << std::endl;
					std::cout << e << std::endl;
				}
				return temp;
			}


		};//end struct B_Tree_iterator


	}//end namespace BTree

}//end namespace RSY_TOOL




#endif // !_B_TREE_ITERATOR_H

