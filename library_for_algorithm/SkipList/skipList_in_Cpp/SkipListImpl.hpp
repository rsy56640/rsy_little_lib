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
		 * Exception: if no such key, throw `std::out_of_range`
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
		template<typename Key_t, typename Value_t>
		std::size_t insert(Key_t&& key, Value_t&& value, insert_type type)
		{

		}


		/*
		 * return number of elements removed.
		 */
		std::size_t erase(const key_type& key)
		{

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
		 * return the maximum node that is less than key at the bottom level.
		 */
		base_ptr find(const key_type& key)
		{

		}


		/*
		 * return the maximum node that is less than key
		 * at the specific level indicated by head.
		 */
		base_ptr find_in_row(base_ptr head, const key_type& key)
		{

		}


		/*
		 * add a row above prev_head, and insert key,
		 * before doing this, the current levels should be in the right place.
		 */
		void add_row_and_insert(head_type prev_head, const key_type& key)
		{


		}


		/*
		 *
		 */
		void erase_row()
		{


		}


		/*
		 *
		 */
		void recycle()
		{


		}


	};//end template class SkipListImpl

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTIMPL_HPP
