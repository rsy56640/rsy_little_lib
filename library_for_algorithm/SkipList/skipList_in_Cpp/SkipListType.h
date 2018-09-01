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
		using node_type = typename SkipListNode<key_type, value_type>::node_type;
		using Key_Compare = std::function<bool(const node_type&, const node_type&)>;
	};

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTTYPE_H
