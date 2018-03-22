#pragma once
#ifndef _RB_TREE_ITERATOR_H
#define _RB_TREE_ITERATOR_H
#include "RB_Tree_Node.h"
#include <iterator>
#include <iostream>
#include <type_traits>
#include <memory>
#include <functional>


namespace MY_RB_Tree
{

	struct RB_Tree_Iterator_Base
	{
		using base_ptr = typename RB_Tree_Node_Base::base_ptr;
		using difference_type = long long;
		using  iterator_category = typename  std::bidirectional_iterator_tag;

		base_ptr node;

		void increment()
		{

		}

		void decrement()
		{

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
#endif // !_RB_TREE_ITERATOR_H
