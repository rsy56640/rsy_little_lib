#pragma once
#ifndef _B_TREE_NODE_H
#define _B_TREE_NODE_H
#include "B_Tree_Type.h"
#include <utility>

namespace RSY_TOOL
{

	namespace BTree
	{


		/*
		 * judge whether the node is leaf
		**/
		inline const bool isLeaf(const int degree_isLeaf);


		/*
		 * return the value of the node's degree
		**/
		inline const _STD size_t degree(const int degree_isLeaf);


		/*
		 * set the src leaf property according to the dest node
		**/
		inline void setLeaf(int& _src_degree_isLeaf, const int _dest__degree_isLeaf);


		/*
		 * struct B_Tree_Node_Base
		 *         only store the relation and size, no data.
		**/
		struct B_Tree_Node_Base
		{

			using INT_BOOL = int;
			using base_type = B_Tree_Node_Base;
			using base_ptr = B_Tree_Node_Base*;


			B_Tree_Node_Base(const _STD size_t degree);

			B_Tree_Node_Base(const B_Tree_Node_Base&) = delete;

			B_Tree_Node_Base& operator=(const B_Tree_Node_Base&) = delete;

			virtual ~B_Tree_Node_Base() = 0;

			const _STD size_t degree()const noexcept;

			const bool leaf()const noexcept;


			INT_BOOL degree_isLeaf;     //the last digit store the leaf property
			_STD size_t _key_size;
			base_ptr* branch;
			base_ptr parent;


		};//end struct B_Tree_Node_Base


		/*
		 * template struct B_Tree_Node
		 *            restore the data.
		**/
		template <class Key, class Value>
		struct B_Tree_Node : B_Tree_Node_Base, B_tree_type<Key, Value>
		{

			using base_ptr = typename B_Tree_Node_Base::base_ptr;
			using link_type = typename B_Tree_Node<Key, Value>*;
			using data_type = typename _STD pair<const Key, Value>;
			using data_ptr = typename data_type*;


			B_Tree_Node(const _STD size_t degree)
				:B_Tree_Node_Base(degree)
			{
				data = new data_ptr[(degree << 1) - 1];
			}

			B_Tree_Node(const B_Tree_Node&) = delete;

			B_Tree_Node& operator=(const B_Tree_Node&) = delete;

			~B_Tree_Node()
			{
				delete[] data;
			}


			data_ptr *data;

		};//end template struct B_Tree_Node


		/*
		 * write the node to the disk
		 * the operation is based on the argument _mode
		**/
		constexpr int W_ERASE = 0;     //write back and free memory
		constexpr int W_NOERASE = 1;   //onlt write back, no free memory operation
		constexpr int W_NODIRTY = 2;   //only free memory, no write operation
		void DISK_WRITE(B_Tree_Node_Base*, bool _mode = W_ERASE);


		/*
		 * read the node from the disk
		**/
		void DISK_READ(B_Tree_Node_Base*);


	}//end namespace BTree

}//end namespace RSY_TOOL

#endif // !_B_TREE_NODE_H
