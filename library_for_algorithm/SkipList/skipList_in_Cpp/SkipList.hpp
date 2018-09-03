#pragma once
#ifndef _SKIPLIST_HPP
#define _SKIPLIST_HPP
#include "SkipListImpl.hpp"

namespace RSY_TOOL::SkipList
{

	template<typename Key, typename Value> class SkipList :public SkipListType<Key, Value>
	{
		using key_type = typename SkipListType<Key, Value>::key_type;
		using value_type = typename SkipListType<Key, Value>::value_type;
		using base_ptr = typename SkipListType<Key, Value>::base_ptr;
		using node_ptr = typename SkipListType<Key, Value>::node_ptr;
		using Key_Compare = typename SkipListType<Key, Value>::Key_Compare;
		using insert_type = typename SkipListType<Key, Value>::insert_type;

	public:
		explicit SkipList(Key_Compare _key_compare)
			: _pImpl(std::make_unique<SkipListImpl<Key, Value>>(_key_compare))
		{}
		SkipList(const SkipList&) = delete;
		SkipList& operator=(const SkipList&) = delete;
		SkipList(SkipList&&) = default;
		SkipList& operator=(SkipList&&) & = default;


		bool empty() const
		{
			return _pImpl->empty();
		}


		std::size_t size() const
		{
			return _pImpl->size();
		}


		/*
		 * Exception: if no such key, throw std::out_of_range
		 */
		value_type& operator[](const key_type& key)
		{
			return _pImpl->operator[](key);
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
		> std::size_t insert(Key_t&& key, Value_t value)
		{
			return _pImpl->insert(std::forward<Key_t>(key),
				std::forward<Value_t>(value), insert_type::insert);
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
		> std::size_t insert_or_assign(Key_t&& key, Value_t value)
		{
			return _pImpl->insert(std::forward<Key_t>(key),
				std::forward<Value_t>(value), insert_type::insert_or_assign);
		}


		/*
		 * return number of element erased.
		 */
		std::size_t erase(const key_type& key)
		{
			return _pImpl->erase(key);
		}


	private:

		std::unique_ptr<SkipListImpl<Key, Value>> _pImpl;

	};//end template class SkipList

}

#endif // !_SKIPLIST_HPP
