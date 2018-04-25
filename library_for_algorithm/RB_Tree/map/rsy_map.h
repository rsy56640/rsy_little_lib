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

			using key_type = typename map_type<Key, Value>::key_type;
			using mapped_type = typename map_type<Key, Value>::mapped_type;
			using value_type = typename map_type<Key, Value>::value_type;
			using size_type = typename map_type<Key, Value>::size_type;
			using difference_type = typename map_type<Key, Value>::difference_type;
			using key_Compare = typename map_type<Key, Value>::key_Compare;
			using const_reference = typename map_type<Key, Value>::const_reference;
			using iterator_type = typename map_type<Key, Value>::iterator_type;

			using RBTree_ptr = typename _STD shared_ptr<RB_Tree<value_type> >;


		protected:


			/*
			 * Proxy class for distinguishing reading and writing
			**/
			class ValueProxy
			{

			public:

				//for writing
				ValueProxy& operator=(const ValueProxy& other);

				ValueProxy& operator=(Value value);

				//for reading
				operator Value() const;


			private:


			};//end class ValueProxy


		public:

			using iterator = iterator_type;

			rsy_map(const key_Compare& key_compare = less<key_type>{});

			rsy_map(const rsy_map&) = default;

			rsy_map& operator=(const rsy_map&) & = default;

			rsy_map(rsy_map&&);

			rsy_map& operator=(rsy_map&&);

			iterator begin() const;

			iterator end() const;

			void insert(const std::pair<Key, Value>& data);

			void insert_assign(const std::pair<Key, Value>& data);

			void erase(const Key& key);


			ValueProxy operator[](const Key&);

			//const
			const mapped_type& operator[](const Key&)const;

			void swap(RBTree_ptr& other);



		private:

			RBTree_ptr _rbt;


		};//end class rsy_map


		/****
		 ****  functions of inner class ProxyValue
		*****/

		/*
		 * for writing
		**/
		template<class Key, class Value>
		inline typename rsy_map<Key, Value>::ValueProxy& rsy_map<Key, Value>::ValueProxy::operator=(const typename rsy_map<Key, Value>::ValueProxy& other)
		{
			;
		}


		/*
		 * for writing
		**/
		template<class Key, class Value>
		inline typename rsy_map<Key, Value>::ValueProxy& rsy_map<Key, Value>::ValueProxy::operator=(Value value)
		{

		}


		/*
		 *for reading
		**/
		template<class Key, class Value>
		inline rsy_map<Key, Value>::ValueProxy::operator Value() const
		{

		}


		/****
		 ****  functions of class rsy_map<Ket, Value>
		*****/


		/*
		 * default constructor
		**/
		template<class Key, class Value>
		rsy_map<Key, Value>::rsy_map(const key_Compare& key_compare)
			:_rbt
		{
			_STD make_shared<RB_Tree<value_type> >
			(
				[key_compare](const value_type& lhs, const value_type& rhs)->bool
		{return key_compare(lhs.first, rhs.first); }
				)
		}
		{}


		/*
		 * move constructor
		**/
		template<class Key, class Value>
		rsy_map<Key, Value>::rsy_map(rsy_map<Key, Value>&& other)
		{
			this->swap(other);
		}


		//move assignment
		template<class Key, class Value>
		inline rsy_map<Key, Value>& rsy_map<Key, Value>::operator=(rsy_map<Key, Value>&& other)
		{
			if (*this == other)return;
			this->swap(other);
		}


		//the begin iterator of the map
		template<class Key, class Value>
		inline typename rsy_map<Key, Value>::iterator rsy_map<Key, Value>::begin() const
		{
			return  _rbt->minimum();
		}


		//the end iterator of the map
		template<class Key, class Value>
		inline typename rsy_map<Key, Value>::iterator rsy_map<Key, Value>::end() const
		{
			return  ++(_rbt->maximum());
		}


		//insert a pair of K-V
		//will not substitute if key collides
		template<class Key, class Value>
		inline void rsy_map<Key, Value>::insert(const std::pair<Key, Value>& data)
		{
			_rbt->insert(data);
		}


		//insert a pair of K-V
		//will substitute if key collides
		template<class Key, class Value>
		inline void rsy_map<Key, Value>::insert_assign(const std::pair<Key, Value>& data)
		{
			_rbt->insert(data, _INSERT_ASSIGNMENT);
		}


		//erase 
		template<class Key, class Value>
		inline void rsy_map<Key, Value>::erase(const Key& key)
		{

		}

		template<class Key, class Value>
		inline typename rsy_map<Key, Value>::ValueProxy rsy_map<Key, Value>::operator[](const Key&)
		{

		}

		//const
		template<class Key, class Value>
		inline const Value& rsy_map<Key, Value>::operator[](const Key&) const
		{

		}

		template<class Key, class Value>
		inline void rsy_map<Key, Value>::swap(RBTree_ptr& other)
		{

		}

		template<class Key, class Value>
		void swap(rsy_map<Key, Value>& lhs, rsy_map<Key, Value>& rhs)
		{
			lhs.swap(rhs);
		}

	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL

#endif // !_RSY_MAP_H
