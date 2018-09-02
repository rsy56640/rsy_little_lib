#pragma once
#ifndef _SKIPLISTIMPL_HPP
#define _SKIPLISTIMPL_HPP
#include <list>
#include <stdexcept>
#include <random>
#include "SkipListType.h"

namespace RSY_TOOL::SkipList
{

	template<typename Key, typename Value> class SkipListImpl :public SkipListType<Key, Value>
	{
		using key_type = typename SkipListType<Key, Value>::key_type;
		using value_type = typename SkipListType<Key, Value>::value_type;
		using base_ptr = typename SkipListType<Key, Value>::base_ptr;
		using node_ptr = typename SkipListType<Key, Value>::node_ptr;
		using Key_Compare = typename SkipListType<Key, Value>::Key_Compare;
		using insert_type = typename SkipListType<Key, Value>::insert_type;

		using head_type = typename std::list<std::pair<base_ptr, std::size_t>>::iterator;

		constexpr static std::size_t fail = 0;
		constexpr static std::size_t success = 1;
#define RETURN_INSERT_FAIL return fail;
#define RETURN_INSERT_SUCCESS return success;
#define RETURN_ERASE_FAIL return fail;
#define RETURN_ERASE_SUCCESS return success;

	public:

		SkipListImpl(Key_Compare&& key_compare)
			:_heads(), _key_compare(std::move(key_compare))
		{
			_heads.push_back(std::make_pair(make_nil(), 0u));
		}


		bool empty() const
		{
			return _heads.front().second == 0u;
		}


		std::size_t size() const
		{
			return _heads.front().second;
		}


		/*
		 * Exception: if no such key, throw `std::out_of_range`.
		 */
		value_type& operator[](const key_type& key)
		{
			base_ptr prev = find(key);
			if (prev->right == nullptr)
				throw std::out_of_range{};
			if (!_key_compare(key, prev->right))
				return static_cast<node_ptr>(prev->right)._value;
			throw std::out_of_range{};
		}


		/*
		 * return number of element inserted.
		 */
		template<
			typename Key_t,
			typename Value_t,
			std::enable_if_t<
			std::is_convertible_v<Key, Key_t> &&
			std::is_convertible_v<Value, Value_t>
			>* = nullptr
		> std::size_t insert(Key_t&& key, Value_t&& value, insert_type type)
		{
			base_ptr prev = find(key);
			if (prev->right != nullptr && !_key_compare(key, prev->right)) { // key collides
				if (type == insert_type::insert_or_assign) {
					static_cast<node_ptr>(prev->right)->_value = std::forward<Value_t>(value);
					RETURN_INSERT_SUCCESS
				}
				else RETURN_INSERT_FAIL
			}
			else { // insert at the right
				const base_ptr new_node = make_node<Key, Value>
					(std::forward<Key_t>(key), std::forward<Value_t>(value));
				const std::size_t level = get_level();
				std::size_t cur_level = 0;
				base_ptr below_node;
				base_ptr cur_node = make_copy(new_node);
				base_ptr right_node = prev->right; // might be nullptr
				head_type head_it = _heads.begin();
				prev->right = cur_node;
				cur_node->left = prev;
				cur_node->right = right_node;
				if (right_node != nullptr) right_node->left = cur_node;
				below_node = cur_node;
				cur_level++;
				head_it->second++;
				++head_it;
				while (cur_level < level && head_it != _heads.end())
				{
					cur_node = make_copy(new_node);
					prev = find_in_row(head_it->first, static_cast<node_ptr>(new_node)._key);
					right_node = prev->right; // might be nullptr
					prev->right = cur_node;
					cur_node->left = prev;
					cur_node->right = right_node;
					if (right_node != nullptr) right_node->left = cur_node;
					below_node->up = cur_node;
					cur_node->down = below_node;
					below_node = cur_node;
					cur_level++;
					head_it->second++;
					++head_it;
				}
				if (cur_level == level) { // new node's level is below the table.
					erase_node(const_cast<base_ptr>(new_node));
					RETURN_INSERT_SUCCESS
				}
				else { // new node's level is above the table, just add one level.
					base_ptr node = const_cast<base_ptr>(new_node);

					// TODO


				}
			}
		}


		/*
		 * return number of elements removed.
		 */
		std::size_t erase(const key_type& key)
		{
			// TODO
			//从上向下搜索，不要调用find()，每一行单独写，until key collision，
			//向下删除所有node
		}


	private:

		std::list<std::pair<base_ptr, std::size_t>> _heads;

		Key_Compare _key_compare;


		/*
		 * get random integer uniformly from [Left, Right].
		 */
		std::size_t get_random()
		{
			return std::uniform_int_distribution<std::size_t>{0, 1}
			(std::default_random_engine{ std::random_device{}() });
		}


		/*
		 * get the random level for the new node.
		 */
		std::size_t get_level()
		{
			std::size_t level = 1u;
			while (get_random() == 0)
				level++;
			return level;
		}


		/*
		 * return the maximum-key node that is less than key at the bottom level.
		 */
		base_ptr find(const key_type& key)
		{
			base_ptr cur = _heads.back().first;
			cur = find_in_row(cur, key);
			while (cur->down != nullptr)
			{
				cur = cur->down;
				cur = find_in_row(cur, key);
			}
			return cur;
		}


		/*
		 * return the maximum node that is less than key
		 * at the specific level indicated by head.
		 * (head might not be the first but promise head.key < key or head is nil)
		 */
		base_ptr find_in_row(base_ptr head, const key_type& key)
		{
			base_ptr cur = head;
			while (cur->right != nullptr && _key_compare(cur->right->_key, key))
				cur = cur->right;
			return cur;
		}


		/*
		 * add a row above the table, and insert key,
		 * before doing this, the current levels should be in the right place.
		 */
		void add_row_and_insert(const key_type& key)
		{


		}


		/*
		 * erase the row that start with head_it
		 */
		void erase_row(head_type head_it)
		{


		}


		/*
		 *
		 */
		void recycle()
		{


		}


#undef RETURN_ERASE_SUCCESS 
#undef RETURN_ERASE_FAIL 
#undef RETURN_INSERT_SUCCESS 
#undef RETURN_INSERT_FAIL

	};//end template class SkipListImpl

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTIMPL_HPP
