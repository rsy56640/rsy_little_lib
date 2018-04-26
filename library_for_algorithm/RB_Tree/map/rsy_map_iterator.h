/********************************\
*           deprecated           *
\********************************/
#pragma once
#define _RSY_MAP_ITERATOR_H
#ifndef _RSY_MAP_ITERATOR_H
#define _RSY_MAP_ITERATOR_H
#include "RB_Tree.h"
#include "RB_Tree_Iterator.h"
#include "rsy_map_Exception.h"

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{


		template<class Key, class Value>
		class map_iterator : public map_type<Key, Value>
		{

			using key_type = typename map_type<Key, Value>::key_type;
			using mapped_type = typename map_type<Key, Value>::mapped_type;
			using value_type = typename map_type<Key, Value>::value_type;
			using size_type = typename map_type<Key, Value>::size_type;
			using difference_type = typename map_type<Key, Value>::difference_type;
			using key_Compare = typename map_type<Key, Value>::key_Compare;
			using const_reference = typename map_type<Key, Value>::const_reference;
			using iterator_type = typename map_type<Key, Value>::iterator_type;

			using RBT_iterator = typename RB_Tree_Iterator<_Ty>::self_type;

		public:

			map_iterator(const RBT_iterator& rb_it)
				:_rb_it(rb_it)
			{}

			map_iterator(const map_iterator&) = default;

			map_iterator& operator=(const map_iterator&) & = default;

			bool operator==(const map_iterator& other)
			{
				return (this->_rb_it == other._rb_it);
			}

			bool operator!=(const map_iterator& _rb_it)
			{
				return (this->_rb_it != other._rb_it);
			}



		private:

			RBT_iterator _rb_it;

		};//end class map_iterator


	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RSY_MAP_ITERATOR_H
