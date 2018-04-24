#pragma once
#ifndef _RSY_MAP_H
#define _RSY_MAP_H
#include "RB_Tree.h"
#include "rsy_map_Exception.h"
#include "rsy_map_type.h"

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		template<class Key, class Value>
		class rsy_map :public map_type<Key, Value>
		{

			using key_type = Key;
			using mapped_type = Value;


			void insert(const std::pair<Key, Value>& data);

			void insert_assign(const std::pair<Key, Value>& data);

			void erase(const Key& key);

			//const


		};//end class rsy_map



	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RSY_MAP_H
