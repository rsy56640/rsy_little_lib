#ifndef _TRIETYPE_HPP
#define _TRIETYPE_HPP
#include "TrieNode.hpp"

namespace RSY_TOOL::Trie
{

	template<typename Key> struct TrieType
	{
		using node_ptr = typename TrieNode<Key>::node_ptr;
		using key_type = typename TrieNode<Key>::key_type;
	};

}//end namespace RSY_TOOL::Trie

#endif // !_TRIETYPE_HPP
