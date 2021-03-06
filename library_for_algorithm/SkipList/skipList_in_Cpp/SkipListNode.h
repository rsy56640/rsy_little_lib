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
		const Key _key;
		Value _value;
		template<
			typename Key_t,
			typename Value_t,
			std::enable_if_t<
			std::is_convertible_v<std::decay_t<Key_t>, Key> &&
			std::is_convertible_v<std::decay_t<Value_t>, Value>
			>* = nullptr
		> explicit SkipListNode(Key_t&& key, Value_t&& value)
			: _key(std::forward<Key_t>(key)), _value(std::forward<Value_t>(value)) {}
		SkipListNode(const SkipListNode& other)
			: _key(other._key), _value(other._value) {}
		SkipListNode& operator=(const SkipListNode&) = delete;
		SkipListNode(SkipListNode&&) = delete;
		SkipListNode& operator=(SkipListNode&&) = delete;
	};

	template<typename Key, typename Value, typename Key_t, typename Value_t>
	decltype(auto) make_node(Key_t&& key, Value_t&& value)
	{
		return new SkipListNode<Key, Value>(std::forward<Key_t>(key), std::forward<Value_t>(value));
	}

	template<typename Key, typename Value>
	decltype(auto) make_copy(const SkipListNode<Key, Value>& node)
	{
		return new SkipListNode<Key, Value>{ node };
	}

	template<typename Key, typename Value>
	void erase_node(NodeBase* node) { delete static_cast<SkipListNode<Key, Value>*>(node); }

	NodeBase* make_nil() { return new NodeBase{}; }
	void erase_nil(NodeBase* nil) { delete nil; }

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTNODE_H
