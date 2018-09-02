#pragma once
#ifndef _SKIPLISTIMPL_HPP
#define _SKIPLISTIMPL_HPP
#include <deque>
#include "SkipListType.h"
#include "SkipListNode.h"
#include "SkipListException.h"

namespace RSY_TOOL::SkipList
{

	template<typename Key, typename Value> class SkipListImpl :public SkipListType<Key, Value>
	{
		using key_type = typename SkipListType<Key, Value>::key_type;
		using value_type = typename SkipListType<Key, Value>::value_type;
		using base_ptr = typename SkipListType<Key, Value>::base_ptr;
		using node_type = typename SkipListType<Key, Value>::node_type;
		using Key_Compare = typename SkipListType<Key, Value>::Key_Compare;

	public:

		SkipListImpl(Key_Compare&& key_compare)
			:_head(nullptr), _key_compare(std::move(key_compare)) {}

	private:
		base_ptr _head;
		Key_Compare _key_compare;

	};//end template class SkipListImpl

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTIMPL_HPP
