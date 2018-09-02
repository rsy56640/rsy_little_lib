#pragma once
#ifndef _SKIPLISTNODE_H
#define _SKIPLISTNODE_H
#include <type_traits>

namespace RSY_TOOL::SkipList
{

	struct NodeBase
	{
		using base_ptr = NodeBase * ;
		base_ptr left = nullptr;
		base_ptr right = nullptr;
		base_ptr up = nullptr;
		base_ptr down = nullptr;
	};

	template<typename Key, typename Value> struct SkipListNode :NodeBase
	{
		using base_ptr = typename NodeBase::base_ptr;
		using node_ptr = typename SkipListNode<Key, Value>*;
		Key _key;
		Value _value;
		template<
			typename Key_t,
			typename Value_t,
			std::enable_if_t<
			std::is_convertible_v<Key, Key_t> &&
			std::is_convertible_v<Value, Value_t>
			>* = nullptr
		> explicit SkipListNode(Key_t&& key, Value_t&& value)
			: _key(std::forward<Key>(key)), _value(std::forward<Value>(value)) {}
		SkipListNode(const SkipListNode& other)
			: _key(other._key), _value(other._value) {}
		SkipListNode& operator=(const SkipListNode&) = delete;
		SkipListNode(SkipListNode&&) = delete;
		SkipListNode& operator=(SkipListNode&&) = delete;
	};

	template<typename Key, typename Value, typename Key_t, typename Value_t>
	decltype(auto) make_node(Key_t&& key, Value_t&& value)
	{
		return new SkipListNode<Key, Value>
			(std::forward<Key_t>(key),
				std::forward<Value_t>(value));
	}

	template<typename Key, typename Value>
	void erase_node(NodeBase* node) { delete static_cast<SkipListNode<Key, Value>*>(node); }

	NodeBase* make_nil() { return new NodeBase{}; }
	void erase_nil(NodeBase* nil) { delete nil; }

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTNODE_H
