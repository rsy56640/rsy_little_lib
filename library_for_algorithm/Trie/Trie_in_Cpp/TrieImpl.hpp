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
			std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<_InIt>::iterator_category, std::forward_iterator_tag> &&
			std::is_convertible_v<std::decay_t<std::conditional_t<std::is_pointer_v<_InIt>, std::remove_pointer_t<_InIt>, decltype(std::declval<_InIt>().operator*())>>, Key>
			>* = nullptr
		> bool find(_InIt first, _InIt last) const
		{
			node_ptr root = _root;
			node_ptr next;
			while (first != last)
			{
				next = root->find(*first);
				if (next == nullptr) return false;
				root = next;
				++first;
			}
			return root->_isKey;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 */
		template<
			typename _InIt,
			std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<_InIt>::iterator_category, std::forward_iterator_tag> &&
			std::is_convertible_v<std::decay_t<std::conditional_t<std::is_pointer_v<_InIt>, std::remove_pointer_t<_InIt>, decltype(std::declval<_InIt>().operator*())>>, Key>
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
					next = root->find(*first);
					if (next == nullptr) // make new node
					{
						not_match = true;
						next = make_node<Key>(*first);
						root = root->addKey(*first, next);
					}
					else root = next;
				}
				else // make new node
				{
					next = make_node<Key>(*first);
					root = root->addKey(*first, next);
				}
				++first;
			}
			root->_isKey = true;
		}


		/*
		 * Parameter: [first, last) represents the forwardable stream.
		 * There are 3 cases of deletion:
		 *     1. no such key                          =>   no operation.
		 *     2. another key is longer than key       =>   unmark the node.
		 *     3. no longer key, but has prefix key    =>   delete node until prefix lastKey.
		 */
		template<
			typename _InIt,
			std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<_InIt>::iterator_category, std::forward_iterator_tag> &&
			std::is_convertible_v<std::decay_t<std::conditional_t<std::is_pointer_v<_InIt>, std::remove_pointer_t<_InIt>, decltype(std::declval<_InIt>().operator*())>>, Key>
			>* = nullptr
		> bool erase(_InIt first, _InIt last)
		{
			node_ptr root = _root;
			node_ptr lastKey = nullptr;
			std::stack<node_ptr> stk; // _root must not be put into stack.
			while (first != last)
			{
				root = root->find(*first);
				if (root == nullptr) return false; // case 1.
				stk.push(root);
				++first;
				if (root->_isKey && first != last) lastKey = root;
			}
			if (!root->_isKey) return false; // case 1.
			if (!root->next.empty()) // has longer key
			{
				root->_isKey = false; // case 2.
				return true;
			}
			else // no longer key, delete until prefix lastKey.
			{
				while (stk.top() != lastKey)
				{
					root = stk.top(); stk.pop();
					stk.top()->next.erase(root->_key.value());
					erase_node(root);
				}
				return true;
			}
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
