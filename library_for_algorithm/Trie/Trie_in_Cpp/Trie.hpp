#ifndef _TRIE_HPP
#define _TRIE_HPP
#include "TrieImpl.hpp"
#include <memory>

namespace RSY_TOOL::Trie
{

	template<typename Key> class Trie :TrieType<Key>
	{

		using node_ptr = typename TrieType<Key>::node_ptr;
		using key_type = typename TrieType<Key>::key_type;


	public:

		explicit Trie() :_pImpl(std::make_unique<TrieImpl<Key>>()) {}


	private:

		std::unique_ptr<TrieImpl<Key>> _pImpl;

	};



}//end namespace RSY_TOOL::Trie

#endif // !_TRIE_HPP
