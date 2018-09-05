#ifndef _TRIEIMPL_HPP
#define _TRIEIMPL_HPP
#include "TrieType.hpp"

namespace RSY_TOOL::Trie
{

	template<typename Key> class TrieImpl :public TrieType<Key>
	{
		using node_ptr = typename TrieType<Key>::node_ptr;
		using key_type = typename TrieType<Key>::key_type;


	public:

		explicit TrieImpl() :_root(make_root<Key>()) {}
		TrieImpl(const TrieImpl&) = delete;
		TrieImpl& operator=(const TrieImpl&) = delete;
		TrieImpl(TrieImpl&& other) :_root(other._root) { other._root = nullptr; }
		TrieImpl& operator=(TrieImpl&& other)
		{
			_root = other._root;
			other._root = nullptr;
			return *this;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<
			std::is_convertible_v<Key,
			std::decay_t<decltype(std::declval<_InIt>().operator*())>>
			>* = nullptr
			> bool find(_InIt first, _InIt last)
		{
			node_ptr root = _root;
			node_ptr next;
			while (first != last)
			{
				next = root.find(*first);
				if (next == nullptr) return false;
				root = next;
				++first;
			}
			return true;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<
			std::is_convertible_v<Key,
			std::decay_t<decltype(std::declval<_InIt>().operator*())>>
			>* = nullptr
			> void insert(_InIt first, _InIt last)
		{
			node_ptr root = _root;
			node_ptr next;
			while (first != last)
			{


			}
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<
			std::is_convertible_v<Key,
			std::decay_t<decltype(std::declval<_InIt>().operator*())>>
			>* = nullptr
			> bool erase(_InIt first, _InIt last)
		{



		}


		void swap(TrieImpl& other)
		{
			std::swap(_root, other._root);
		}


		~TrieImpl()
		{
			if (_root == nullptr) return;

			// TODO

		}

	private:

		node_ptr _root;

	};//end class Trie


}//end namespace RSY_TOOL::Trie

#endif // !_TRIEIMPL_HPP
