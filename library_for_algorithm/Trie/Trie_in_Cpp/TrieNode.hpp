#ifndef _TRIENODE_HPP
#define _TRIENODE_HPP
#include <optional>
#include <type_traits>
#include <unordered_map>

namespace RSY_TOOL::Trie
{

	template<typename Key> struct TrieNode
	{
		using node_ptr = typename TrieNode<Key>*;
		using key_type = std::optional<Key>;
		key_type _key;
		bool _isKey = false;
		std::unordered_map<Key, node_ptr> next;
		template<
			typename Key_t,
			std::enable_if_t<
			std::is_convertible_v<Key, std::decay_t<Key_t>>>
		> explicit TrieNode(Key_t&& key) : _key(std::forward<Key_t>(key)) {}
		explicit TrieNode() :_key(std::nullopt) {}
		TrieNode(const TrieNode&) = delete;
		TrieNode& operator=(const TrieNode&) = delete;

		node_ptr find(const Key& key)
		{
			try {
				return next.at(key);
			}
			catch (std::out_of_range&)
			{
				return nullptr;
			}
		}

		node_ptr addKey(const Key& key, node_ptr new_node)
		{
			return next.insert(std::make_pair(key, new_node)).first->second;
		}

	};


	template<typename Key, typename Key_t> decltype(auto) make_node(Key_t&& key)
	{
		return new TrieNode<Key>{ std::forward<Key_t>(key) };
	}

	template<typename Key> decltype(auto) make_root()
	{
		return new TrieNode<Key>();
	}

	template<typename Key> void erase_node(TrieNode<Key>* node)
	{
		delete node;
	}

}//end namespace RSY_TOOL::Trie

#endif // !_TRIENODE_HPP
