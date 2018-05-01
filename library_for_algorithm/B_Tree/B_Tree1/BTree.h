#pragma once
#ifndef _B_TREE_H
#define _B_TREE_H
#include "B_TreeImpl.h"
#include "B_Tree_iterator.h"
#include "memory"

namespace RSY_TOOL
{

	namespace BTree
	{


		/*
		 * template class BTree
		**/
		template<class Key, class Value>
		class BTree :B_tree_type<Key, Value>
		{

			using key_type = typename Key;
			using mapped_type = typename Value;
			using data_type = typename B_tree_type<Key, Value>::data_type;

			using base_ptr = typename B_Tree_Node_Base::base_ptr;
			using node_type = typename B_Tree_Node<Key, Value>;
			using link_type = typename B_Tree_Node<Key, Value>*;
			using data_type = typename B_tree_type<Key, Value>::data_type;
			using data_ptr = data_type*;

			using Key_Comp = typename B_tree_type<Key, Value>::Key_Comp;
			using Data_Comp = typename B_tree_type<Key, Value>::Data_Comp;
			using Data_Key_Comp = typename B_tree_type<Key, Value>::Data_Key_Comp;

			using B_TreeImpl_ptr = typename std::shared_ptr<B_TreeImpl<Key, Value> >;


		public:


			/*
			 * default constructor
			 * degree is default, and default compare function is less<Key>().
			**/
			BTree(_STD size_t degree = default_degree, Key_Comp key_comp = _STD less<Key>());



		private:

			B_TreeImpl_ptr _btree;

		};//end class BTree


		/*
		 * default constructor
		 * degree is default, and default compare function is less<Key>().
		**/
		template<class Key, class Value>
		BTree<Key, Value>::BTree(_STD size_t degree, Key_Comp key_comp)
			:_btree(_STD make_shared<B_TreeImpl<Key, Value> >(degree, key_comp))
		{}


	}//end namespace BTree

}//end namespace RSY_TOOL

#endif // !_B_TREE_H
