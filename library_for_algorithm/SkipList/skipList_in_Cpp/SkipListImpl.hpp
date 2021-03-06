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
#define RETURN_INSERT_FAIL do{ call_time++; recycle(); return fail; }while (0);
#define RETURN_INSERT_SUCCESS do{ call_time++; recycle(); return success; }while (0);
#define RETURN_ERASE_FAIL do{ call_time++; recycle(); return fail; }while (0);
#define RETURN_ERASE_SUCCESS do{ call_time++; recycle(); return success; }while (0);


		/*
		 * interval times between per call of recycle().
		 */
		constexpr static std::size_t recycle_times = 2000;
		constexpr static double size_rate_tolerance = 0.72;


		/*
		 * Class ValueProxy is used for operator[] to write,
		 * since `S[key]=value` or `S[k1]=S[k2]` leads to the change of one line.
		 */
		template<typename, typename> friend class SkipList;
		class ValueProxy
		{
		public:
			ValueProxy(base_ptr below_node) :_below_node(below_node) {}
			ValueProxy(const ValueProxy&) = delete;

			/*
			 * for writing
			 */
			ValueProxy& operator=(const ValueProxy& other)
			{
				change_from_below_up(_below_node,
					static_cast<node_ptr>(other._below_node)->_value);
				return *this;
			}
			ValueProxy& operator=(const value_type& value)
			{
				change_from_below_up(_below_node, value);
				return *this;
			}

			/*
			 * for reading
			 */
			operator value_type() const
			{
				return static_cast<node_ptr>(_below_node)->_value;
			}

		private:
			base_ptr _below_node;

		};//end class ValueProxy


	public:

		SkipListImpl(Key_Compare key_compare)
			:_heads(), _key_compare(std::move(key_compare))
		{
			_heads.push_back(std::make_pair(make_nil(), 0u));
		}
		SkipListImpl(const SkipListImpl&) = delete;
		SkipListImpl& operator=(const SkipListImpl&) = delete;
		SkipListImpl(SkipListImpl&&) = default;
		SkipListImpl& operator=(SkipListImpl&&) = default;


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
		const value_type& at(const key_type& key) const
		{
			base_ptr prev = find(key);
			if (prev->right == nullptr)
				throw std::out_of_range{ "no such key" };
			if (!_key_node_compare(key, prev->right))
				return static_cast<node_ptr>(prev->right)->_value;
			throw std::out_of_range{ "no such key" };
		}
		value_type& at(const key_type& key)
		{
			return const_cast<value_type&>(static_cast<const SkipListImpl*>(this)->at(key));
		}


		/*
		 * if the key does not exist, insert the key.
		 * the ctor of Value is required.
		 */
		ValueProxy operator[](const key_type& key)
		{
			base_ptr prev = find(key);
			if (prev->right == nullptr || _key_node_compare(key, prev->right))
				insert(key, Value{}, insert_type::insert); // now prev->right = key.
			return ValueProxy{ prev->right };
		}


		/*
		 * return number of element inserted.
		 */
		template<
			typename Key_t,
			typename Value_t,
			std::enable_if_t<
			std::is_convertible_v<std::decay_t<Key_t>, Key> &&
			std::is_convertible_v<std::decay_t<Value_t>, Value>
			>* = nullptr
		> std::size_t insert(Key_t&& key, Value_t&& value, insert_type type)
		{
			base_ptr prev = find(key);
			if (prev->right != nullptr && !_key_node_compare(key, prev->right)) { // key collides
				if (type == insert_type::insert_or_assign) {
					const Value_t& _value = value;
					base_ptr cur_node = prev->right;
					while (cur_node != nullptr)
					{
						static_cast<node_ptr>(cur_node)->_value = _value;
						cur_node = cur_node->up;
					}
					RETURN_INSERT_SUCCESS
				}
				else RETURN_INSERT_FAIL
			}
			else { // insert at the right
				const base_ptr new_node = make_node<Key, Value>(std::forward<Key_t>(key), std::forward<Value_t>(value));
				const std::size_t level = get_level();
				std::size_t cur_level = 0;
				base_ptr below_node;
				base_ptr cur_node = make_copy(*static_cast<node_ptr>(new_node));
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
					cur_node = make_copy(*static_cast<node_ptr>(new_node));
					prev = find_in_row(head_it->first, static_cast<node_ptr>(new_node)->_key);
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
				while (cur_level < level) // new node's level is above the table.
				{
					below_node = add_row_and_insert(new_node, below_node);
					cur_level++;
				}
				erase_node<Key, Value>(const_cast<base_ptr>(new_node));
				RETURN_INSERT_SUCCESS
			}
		}


		/*
		 * return number of elements removed.
		 */
		std::size_t erase(const key_type& key)
		{
			typename std::list<std::pair<base_ptr, std::size_t>>::reverse_iterator head_it = _heads.rbegin();
			base_ptr cur = head_it->first;
			while (cur != nullptr)
			{
				cur = find_in_row(cur, key);
				if (cur->right != nullptr && !_key_node_compare(key, cur->right))
				{
					base_ptr _left;
					cur = cur->right;
					base_ptr _right;
					while (cur != nullptr)
					{
						_left = cur->left;
						_right = cur->right; // might be nullptr
						head_it->second--;
						_left->right = _right;
						if (_right != nullptr) _right->left = _left;
						base_ptr temp = cur;
						cur = cur->down;
						++head_it;
						erase_node<Key, Value>(temp);
					}
					RETURN_ERASE_SUCCESS
				}
				else
				{
					cur = cur->down;
					++head_it;
				}
			}
			RETURN_ERASE_FAIL
		}


		~SkipListImpl()
		{
			for (auto it : _heads)
			{
				base_ptr head = it.first;
				base_ptr cur_node = head->right;
				erase_nil(head);
				while (cur_node != nullptr)
				{
					base_ptr temp_node = cur_node;
					cur_node = cur_node->right;
					erase_node<Key, Value>(temp_node);
				}
			}
		}


	private:

		std::list<std::pair<base_ptr, std::size_t>> _heads;

		Key_Compare _key_compare;

		//times for call recycle().
		std::size_t call_time = 0u;


		inline bool _key_node_compare(const key_type& lhs, const base_ptr& rhs) const
		{
			return _key_compare(lhs, static_cast<node_ptr>(rhs)->_key);
		}


		/*
		 * get random integer uniformly from [Left, Right].
		 */
		static std::size_t get_random()
		{
			std::default_random_engine re{ std::random_device{}() };
			return std::uniform_int_distribution<std::size_t>{0, 1}(re);
		}


		/*
		 * get the random level for the new node.
		 */
		static std::size_t get_level()
		{
			std::size_t level = 1u;
			while (get_random() == 0)
				level++;
			//std::cout << "Level: " << level << std::endl;
			return level;
		}


		/*
		 * return the maximum-key node that is less than key at the bottom level.
		 */
		base_ptr find(const key_type& key) const
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
		base_ptr find_in_row(base_ptr head, const key_type& key) const
		{
			base_ptr cur = head;
			while (cur->right != nullptr
				&& _key_compare(static_cast<node_ptr>(cur->right)->_key, key))
				cur = cur->right;
			return cur;
		}


		/*
		 * below_node is at the bottom level,
		 * change the value of the line that below_node indicates.
		 */
		static void change_from_below_up(base_ptr below_node, const value_type& value)
		{
			while (below_node != nullptr)
			{
				static_cast<node_ptr>(below_node)->_value = value;
				below_node = below_node->up;
			}
		}


		/*
		 * add a row above the table, and insert key,
		 * before doing this, the current levels should be in the right place.
		 * return this new_node as below_node.
		 */
		base_ptr add_row_and_insert(const base_ptr& new_node, base_ptr below_node)
		{
			base_ptr cur_node = make_copy(*static_cast<node_ptr>(new_node));
			base_ptr new_head = make_nil();
			_heads.back().first->up = new_head; new_head->down = _heads.back().first;
			new_head->right = cur_node; cur_node->left = new_head;
			cur_node->down = below_node; below_node->up = cur_node;
			_heads.push_back(std::make_pair(new_head, 1u));
			return cur_node;
		}


		/*
		 * erase the row that start with head_it,
		 * the row would never be the bottom level.
		 */
		void erase_row(head_type head_it)
		{
			base_ptr cur_node = head_it->first->right;
			base_ptr up_head = head_it->first->up; // might be nullptr
			base_ptr down_head = head_it->first->down; // must not be nullptr
			down_head->up = up_head;
			if (up_head != nullptr) up_head->down = down_head;
			base_ptr temp = head_it->first;
			_heads.erase(head_it);
			erase_nil(temp);
			base_ptr up_node; // might be nullptr
			base_ptr down_node; // must not be nullptr
			while (cur_node != nullptr)
			{
				up_node = cur_node->up;
				down_node = cur_node->down;
				down_node->up = up_node;
				if (up_node != nullptr) up_node->down = down_node;
				temp = cur_node;
				cur_node = cur_node->right;
				erase_node<Key, Value>(temp);
			}
		}


		/*
		 * Recycle Strategy:
		 *     compare the size of the consecutive row,
		 *     if rate > `size_rate_tolerance`, erase the row,
		 *     the order if either from bottom to up or from up to bottom.
		 *         if up_size / down_size > size_rate_tolerance,
		 *         then erase up_row.
		 */
		void recycle()
		{
			static bool up2bottom = true; // the order to examine the rate. 
			constexpr static std::size_t min_level = 3;
			if (call_time == recycle_times)
			{
				call_time = 0u;
				if (_heads.size() < min_level) return;
				if (up2bottom) // up -> bottom
				{
					up2bottom = false;
					typename std::list<std::pair<base_ptr, std::size_t>>::reverse_iterator head_it = _heads.rbegin();
					std::size_t up_size = head_it->second;
					std::size_t down_size;
					++head_it;
					while (head_it != _heads.rend())
					{
						down_size = head_it->second;
						double rate = static_cast<double>(double(1.0) * up_size / down_size);
						if (rate > size_rate_tolerance)
						{
							erase_row(++head_type{ --(head_it.base()) }); // erase up_row
							return;
						}
						up_size = down_size;
						++head_it;
					}
				}
				else // bottom -> up
				{
					up2bottom = true;
					head_type head_it = _heads.begin();
					std::size_t down_size = head_it->second;
					std::size_t up_size;
					++head_it;
					while (head_it != _heads.end())
					{
						up_size = head_it->second;
						double rate = static_cast<double>(double(1.0) * up_size / down_size);
						if (rate > size_rate_tolerance)
						{
							erase_row(head_it);
							return;
						}
						down_size = up_size;
						++head_it;
					}
				}
			}
		}// end void recycle();


#undef RETURN_ERASE_SUCCESS 
#undef RETURN_ERASE_FAIL 
#undef RETURN_INSERT_SUCCESS 
#undef RETURN_INSERT_FAIL

	};//end template class SkipListImpl

}//end namespace RSY_TOOL::SkipList

#endif // !_SKIPLISTIMPL_HPP
