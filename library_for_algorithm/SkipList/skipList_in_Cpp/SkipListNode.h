#pragma once
#ifndef _SKIPLISTNODE_H
#define _SKIPLISTNODE_H
#include <type_traits>

namespace RSY_TOOL::SkipList
{

	struct NodeBase
	{
		using base_ptr = NodeBase * ;
		base_ptr right = nullptr;
		base_ptr down = nullptr;
	};

	template<typename Key, typename Value> struct SkipListNode :NodeBase
	{
		using base_ptr = typename NodeBase::base_ptr;
		using node_type = typename SkipListNode<Key, Value>;
		Key _key;
		Value _value;
		template<bool> struct helper :std::false_type {};
		template<> struct helper<true> :std::true_type { using sfinae_type = void; };
		template<bool b> using helper_t = typename helper<b>::sfinae_type;
		template<
			typename Key_t,
			typename Value_t,
			typename = helper_t<
			std::is_same_v<Key, std::decay_t<Key_t>> &&
			std::is_same_v<Value, std::decay_t<Value_t>>
			>> SkipListNode(Key_t&& key, Value_t&& value)
			: _key(std::forward<Key>(key)), _value(std::forward<Value>(value)) {}
	};

	template<typename Key, typename Value> decltype(auto) make_node(Key&& key, Value&& value)
	{
		return new SkipListNode<Key, Value>
			(std::forward<Key>(key),
				std::forward<Value>(value));
	}

	template<typename Key, typename Value>
	void erase_node(SkipListNode<Key, Value>* node) { delete node; }

	NodeBase* make_nil() { return new NodeBase{}; }
	void erase_nil(NodeBase* nil) { delete nil; }

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTNODE_H