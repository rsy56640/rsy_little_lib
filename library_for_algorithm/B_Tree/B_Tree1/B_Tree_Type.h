#pragma once
#ifndef _B_TREE_TYPE_H
#define _B_TREE_TYPE_H
#include <functional>


namespace RSY_TOOL
{

	namespace BTree
	{

		/*
		 * type class for B_Tree
		**/
		template<class Key, class Value>
		struct B_tree_type
		{
			using key_type = typename Key;
			using mapped_type = typename Value;
			using data_type = typename std::pair<const Key, Value>;

			using Key_Comp = typename std::function<bool(const Key&, const Key&)>;
			using Data_Comp = typename std::function<bool(const data_type&, const data_type&)>;
			using Data_Key_Comp = typename std::function<bool(const data_type&, const Key&)>;
		};


	}//end namespace BTree

}//end namespace RSY_TOOL

#endif // !_B_TREE_TYPE_H
