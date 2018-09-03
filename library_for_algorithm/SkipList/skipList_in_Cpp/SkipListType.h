#pragma once
#ifndef _SKIPLISTTYPE_H
#define _SKIPLISTTYPE_H
#include <functional>
#include "SkipListNode.h"

namespace RSY_TOOL::SkipList
{

	template<typename Key, typename Value> struct SkipListType
	{
		using key_type = Key;
		using value_type = Value;
		using base_ptr = typename SkipListNode<key_type, value_type>::base_ptr;
		using node_ptr = typename SkipListNode<key_type, value_type>::node_ptr;
		using Key_Compare = std::function<bool(const key_type&, const key_type&)>;
		enum class insert_type { insert, insert_or_assign };
	};

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTTYPE_H
