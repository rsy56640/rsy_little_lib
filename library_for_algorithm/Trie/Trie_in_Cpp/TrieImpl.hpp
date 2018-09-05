#ifndef _TRIEIMPL_HPP
#define _TRIEIMPL_HPP
#include "TrieType.hpp"
#include <iterator>
#include <stack>

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
			std::is_convertible_v<std::forward_iterator_tag,
			typename std::iterator_traits<_InIt>::iterator_category> &&
			std::is_convertible_v<Key,
			std::decay_t<decltype(std::declval<_InIt>().operator*())>>
			>* = nullptr
			> bool find(_InIt first, _InIt last) const
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
			if (root->_isKey)return true;
			return false;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<
			std::is_convertible_v<std::forward_iterator_tag,
			typename std::iterator_traits<_InIt>::iterator_category> &&
			std::is_convertible_v<Key,
			std::decay_t<decltype(std::declval<_InIt>().operator*())>>
			>* = nullptr
			> void insert(_InIt first, _InIt last)
		{
			node_ptr root = _root;
			node_ptr next;
			bool not_match = false;
			while (first != last)
			{
				if (!not_match)
				{
					next = root.find(*first);
					if (next == nullptr) // make new node
					{
						not_match = true;
						next = make_node<Key>(*first);
						root = root.addKey(*first, next);
					}
					else root = next;
				}
				else // make new node
				{
					next = make_node<Key>(*first);
					root = root.addKey(*first, new_node);
				}
				++first;
			}
			root->_isKey = true;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<
			std::is_convertible_v<std::forward_iterator_tag,
			typename std::iterator_traits<_InIt>::iterator_category> &&
			std::is_convertible_v<Key,
			std::decay_t<decltype(std::declval<_InIt>().operator*())>>
			>* = nullptr
			> bool erase(_InIt first, _InIt last)
		{

			// TODO

		}


		void swap(TrieImpl& other)
		{
			std::swap(_root, other._root);
		}


		~TrieImpl()
		{
			if (_root == nullptr) return;
			std::stack<node_ptr> stk;
			stk.push(_root);
			while (!stk.empty())
			{
				node_ptr node = stk.top(); stk.pop();
				for (auto it : node->next)
					stk.push(it.second);
				erase_node(node);
			}
		}


	private:

		node_ptr _root;

	};//end class TrieImpl


}//end namespace RSY_TOOL::Trie

#endif // !_TRIEIMPL_HPP
