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
		Trie(const Trie&) = delete;
		Trie& operator=(const Trie&) = delete;
		Trie(Trie&&) = default;
		Trie& operator=(Trie&&) = default;

		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<typename _InIt>
		bool find(_InIt first, _InIt last) const
		{
			_pImpl->find(first, last);
		}

		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<typename _InIt>
		void insert(_InIt first, _InIt last)
		{
			_pImpl->insert(first, last);
		}

		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<typename _InIt>
		bool erase(_InIt first, _InIt last)
		{
			_pImpl->erase(first, last);
		}


	private:

		std::unique_ptr<TrieImpl<Key>> _pImpl;

	};//end class Trie

}//end namespace RSY_TOOL::Trie

#endif // !_TRIE_HPP
