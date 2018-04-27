#pragma once
#ifndef _RSY_MAP_TPE_H
#define _RSY_MAP_TPE_H
#include <utility>
#include <functional>
#include "RB_Tree.h"

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		/*
		 * Type class for the rsy_map class
		**/
		template<class Key, class Value>
		struct map_type
		{
			using key_type = Key;
			using mapped_type = Value;
			using value_type = _STD pair<const Key, Value>;
			using size_type = _STD size_t;
			using difference_type = _STD ptrdiff_t;
			using key_Compare = _STD function<bool(const key_type&, const key_type&)>;
			using const_reference = const value_type&;
			using iterator_type = typename RB_Tree<_STD pair<const Key, Value> >::iterator_type;
			using iterator_category = typename RB_Tree<_STD pair<const Key, Value> >::iterator_category;
		};

	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL


#endif // !_RSY_MAP_TPE_H
