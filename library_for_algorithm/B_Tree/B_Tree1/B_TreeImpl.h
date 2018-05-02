#pragma once
#ifndef _B_TREEIMPL_H
#define _B_TREEIMPL_H
#include "B_Tree_Node.h"
#include "B_Tree_Exception.h"

namespace RSY_TOOL
{

	namespace BTree
	{

		//default degree that the minimum amount of keys one B_Tree node has
		const _STD size_t default_degree = 10;


		namespace
		{
			struct INSERT_ARGUMENT { virtual ~INSERT_ARGUMENT() = 0 {}; };
			typedef struct :public INSERT_ARGUMENT {}  Assignment;     //if key collides, replace value
			typedef struct :public INSERT_ARGUMENT {}  NoAssignment;   //no replace
			static const Assignment _Assignment;
			static const NoAssignment _NoAssignment;
			using INSERT_ARG = const INSERT_ARGUMENT&;
		}

#define _INSERT_ASSIGNMENT  _Assignment
#define _INSERT_NOASSIGNMENT _NoAssignment


		/*
		 * template class B_TreeImpl
		**/
		template<class Key, class Value>
		class B_TreeImpl : B_tree_type<Key, Value>
		{

			using key_type = typename B_tree_type<Key, Value>::key_type;
			using mapped_type = typename B_tree_type<Key, Value>::mapped_type;

			using base_ptr = typename B_Tree_Node_Base::base_ptr;
			using node_type = typename B_Tree_Node<Key, Value>;
			using link_type = typename B_Tree_Node<Key, Value>*;
			using data_type = typename B_tree_type<Key, Value>::data_type;
			using data_ptr = data_type*;

			using Key_Comp = typename B_tree_type<Key, Value>::Key_Comp;
			using Data_Comp = typename B_tree_type<Key, Value>::Data_Comp;
			using Data_Key_Comp = typename B_tree_type<Key, Value>::Data_Key_Comp;

			using BTEx = BTreeException;

		public:


			/*
			 * default constructor
			 * degree is default, and default compare function is less<Key>().
			**/
			B_TreeImpl(const _STD size_t degree, Key_Comp key_comp);


			B_TreeImpl(const B_TreeImpl&) = delete;


			B_TreeImpl& operator=(const B_TreeImpl&) = delete;


			/*
			 * destructor, responsible for freeing the memory
			**/
			~B_TreeImpl();


			/*
			 * find the key in the subtree pNode.
			 * if there is no such key, return pair(nullptr, 0);
			**/
			_STD pair<base_ptr, int> B_Tree_find(base_ptr pNode, const Key& key);


			/*
			 * insert a pair of key and value
			 * if _arg equals Assignment, amd key collides, then substitute the value.
			**/
			void insert(const Key&, Value&&, INSERT_ARG);


			/*
			 * erase the data with the specific key,
			 * if the data does not exist, throw an exception
			 * then handle the exception in B_Tree.h
			 * responsible for freeing memory during its process
			**/
			void erase(const Key&);



		protected:    //auxiliary functions for implementation


			/*
			 * allocate a default node with degree.
			**/
			base_ptr allocate_node();


			/*
			 * create the root, intialize the tree
			**/
			void B_Tree_Create();


			/*
			 * free the root, destroy the tree
			**/
			void B_Tree_Delete();


			/*
			 * split the full child_node
			 * and promise that pNode is not full.
			 * pNode and its child is already in the memory
			 * no free operation for pNode and all its childs.
			**/
			void B_Tree_Split_child(base_ptr pNode, _STD size_t index);


			/*
			 * insert key into a non-full node.
			 * responsible for freeing the memory during its process.
			 * if _arg equals Assignment, amd key collides, then substitute the value.
			**/
			void B_Tree_Insert_NONFULL(base_ptr pNode, const Key& key, Value&& value, INSERT_ARG _arg);


			/*
			 * left rotate at position index of pNode.
			 * assume 3 nodes has already been read in the memory
			**/
			void B_Tree_Left_Rotate(base_ptr pNode, _STD size_t index);


			/*
			 * right rotate at position index of pNode.
			 * assume 3 nodes has already been read in the memory
			**/
			void B_Tree_Right_Rotate(base_ptr pNode, _STD size_t index);


			/*
			 * erase a key in the subtree pNode
			 * assume pNode is big enough,
			 * which means pNode has no less than _degree key_words.
			**/
			void B_Tree_erase_BIGENOUGH(base_ptr pNode, const Key& key);




		private:

			const _STD size_t _degree;
			Key_Comp _key_comp;
			Data_Comp _data_comp;
			base_ptr _root;

		};//end class B_TreeImpl


		/*
		 * default constructor
		 * degree is default, and default compare function is less<Key>().
		**/
		template<class Key, class Value>
		B_TreeImpl<Key, Value>::B_TreeImpl(const _STD size_t degree, typename B_TreeImpl<Key, Value>::Key_Comp key_comp)
			:_degree(degree),
			_key_comp(key_comp),
			_data_comp
			(
				[=](const typename B_TreeImpl<Key, Value>::data_type& lhs, const typename B_TreeImpl<Key, Value>::data_type& rhs)->bool
		{
			return _key_comp(lhs.first, rhs.first);
		}
			),
			_root(nullptr)
		{
			B_Tree_Create();
		}


		/*
		 * destructor, responsible for freeing the memory
		**/
		template<class Key, class Value>
		B_TreeImpl<Key, Value>::~B_TreeImpl()
		{
			B_Tree_Delete();
		}


		/*
		 * find the key in the subtree pNode.
		 * if there is no such key, return pair(nullptr, 0);
		**/
		template<class Key, class Value>
		_STD pair<typename B_TreeImpl<Key, Value>::base_ptr, int> B_TreeImpl<Key, Value>::B_Tree_find(base_ptr pNode, const Key& key)
		{

			//cast as a node_type.
			link_type p = static_cast<link_type>(pNode);

			//compare from left to right 
			_STD size_t index = 0;
			while (index < pNode->_key_size && _key_comp(p->data[index]->first, key))  //p.key_i < key
				index++;

			//now, index is _key_size
			//or key is in (p.key_{i-1}, p.key_i].

			//if p.key_i == key
			if (index < pNode->_key_size && !_key_comp(p->data[index]->first, key) && !_key_comp(key, p->data[index]->first))
				return _STD make_pair(pNode, index);

			//if p is leaf, return NIL
			else if (pNode->leaf())
				return _STD make_pair(nullptr, 0);

			//no key collision, and not leaf
			//read the child node from disk
			else DISK_READ(pNode->branch[index]);

			//find the result via recursive process
			return B_Tree_find(pNode->branch[index]);

		}


		/*
		 * insert a pair of key and value
		 * if _arg equals Assignment, amd key collides, then substitute the value.
		**/
		template<class Key, class Value>
		void B_TreeImpl<Key, Value>::insert(const Key& key, Value&& value, INSERT_ARG _arg)
		{

			//store a duplicate of _root.
			base_ptr r = _root;

			//root is full, then split it
			if (r->_key_size == (_degree << 1) - 1)
			{

				//create a null node as the new root.
				base_ptr root = allocate_node();
				_root = root;
				setLeaf(root, NULL);
				root->_key_size = 0;
				root->branch[0] = r;
				r->parent = root;

				//split full node r (root[0])
				B_Tree_Split_child(root, 0);

				//write back
				DISK_WRITE(_root, W_NOERASE);
				DISK_WRITE(_root->branch[0]);
				DISK_WRITE(_root->branch[1]);

			}

			//now root is not full
			B_Tree_Insert_NONFULL(_root, key, value, _arg);

		}


		/*
		 * erase the data with the specific key,
		 * if the data does not exist, throw an exception
		 * then handle the exception in B_Tree.h
		 * responsible for freeing memory during its process
		**/
		template<class Key, class Value>
		void B_TreeImpl<Key, Value>::erase(const Key& key)
		{


		}


		/*
		 * allocate a default node with degree.
		**/
		template<class Key, class Value>
		inline typename B_TreeImpl<Key, Value>::base_ptr B_TreeImpl<Key, Value>::allocate_node()
		{
			return static_cast<base_ptr>(new node_type(_degree));
		}


		/*
		 * create the root, intialize the tree
		**/
		template<class Key, class Value>
		inline void B_TreeImpl<Key, Value>::B_Tree_Create()
		{
			base_ptr _root = allocate_node();
			DISK_WRITE(_root, W_NOERASE);
		}


		/*
		 * free the root, destroy the tree
		**/
		template<class Key, class Value>
		inline void B_TreeImpl<Key, Value>::B_Tree_Delete()
		{
			DISK_WRITE(_root);
		}


		/*
		 * split the full child_node
		 * and promise that pNode is not full.
		 * pNode and its child is already in the memory
		 * no free operation for pNode and all its childs.
		**/
		template<class Key, class Value>
		inline void B_TreeImpl<Key, Value>::B_Tree_Split_child(base_ptr pNode, _STD size_t index)
		{

			//create a sibling, and reassign to sibling the latter part of pNode
			base_ptr sibling = allocate_node();
			sibling->parent = pNode;
			base_ptr p = pNode->branch[index];

			link_type left = static_cast<link_type>(p);
			link_type right = static_cast<link_type>(sibling);

			//reallocate the resource of the to sibling
			setLeaf(sibling, p);

			sibling->_key_size = _degree - 1;

			for (int i = 0; i < _degree - 1; ++i)
				right->data[i] = left->data[i + _degree];

			if (!p->leaf())
			{
				for (int i = 0; i < _degree; ++i)
					sibling->branch[i] = p->branch[i + _degree];
			}

			//truncate the node p to the size (_degree - 1),
			//and lift up the middle key.
			p->_key_size = _degree - 1;

			//right shift the child pointer, and the vacancy points to sibling.
			for (int i = pNode->_key_size; i > index; --i)
				pNode->branch[i + 1] = pNode->branch[i];
			pNode->branch[index + 1] = sibling;

			//shift right pNode.key and set the approptiate position for p.key(middle)
			link_type parent = static_cast<link_type>(pNode);

			for (int i = pNode->_key_size - 1; i >= index; --i)
				parent->data[i + 1] = parent->data[i];
			parent->data[index] = left->data[_degree - 1];

			pNode->_key_size++;

			DISK_WRITE(pNode, W_NOERASE);
			DISK_WRITE(p, W_NOERASE);
			DISK_WRITE(sibling, W_NOERASE);

		}


		/*
		 * insert key into a non-full node.
		 * responsible for freeing the memory during its process.
		**/
		template<class Key, class Value>
		void B_TreeImpl<Key, Value>::B_Tree_Insert_NONFULL(base_ptr pNode, const Key& key, Value&& value, INSERT_ARG _arg)
		{

			_STD size_t index = pNode->_key_size - 1;

			link_type p = static_cast<link_type>(pNode);

			//pNode is a leaf
			//find the appropriate position to insert
			if (pNode->leaf())
			{
				while (index >= 0 && _key_comp(key, p->data[index]->first))  //key < p.key_i
					index--;

				//now index < 0 and key < p.key_0
				//or key is in [p.key_i, p.key_{i+1})
				if (index < 0)
				{

					//shift right the key{0, ..., size-1}
					for (int i = pNode->_key_size - 1; i >= 0; --i)
						p->data[i + 1] = p->data[i];

					p->data[0] = new _STD pair<const Key, Value>(key, value);
					pNode->_key_size++;

					return;
				}


				//verify whether the key equals the p.key_i
				if (_key_comp(p->data[index]->first, key))
				{
					//if _arg indicats assignment, then modify the data
					if (typeid(_arg) == typeid(Assignment))
						p->data[index]->second = _STD foward<Value>(value);
				}
				//not equal
				else {

					//shift right the key{i+1, ..., size-1}
					for (int i = pNode->_key_size - 1; i > index; --i)
						p->data[i + 1] = p->data[i];

					p->data[index + 1] = new _STD pair<const Key, Value>(key, value);
					pNode->_key_size++;

				}

				//write back
				if (pNode == _root)
					DISK_WRITE(pNode, W_NOERASE);
				else DISK_WRITE(pNode);

			}//end if (pNode is leaf)


			//pNode is an internal node
			else
			{
				while (index >= 0 && _key_comp(key, p->data[index]->first))  //key < p.key_i
					index--;

				//now index < 0 and key < p.key_0
				//or key is in [p.key_i, p.key_{i+1})

				//verify whether the key equals to the p.key[index]
				if (index >= 0 && _key_comp(p->data[index]->first, key))
				{
					//if _arg indicats assignment, then modify the data
					if (typeid(_arg) == typeid(Assignment))
						p->data[index]->second = _STD foward<Value>(value);
					return;
				}

				//no key collision
				//key is in (-inf, p.key_0) or (p.key_i, p.key_{i+1})
				//insert the key via a recursive process
				index++;
				DISK_READ(pNode->branch[index]);

				base_ptr child = pNode->branch[index];

				//if child is full
				if (child->_key_size == (_degree << 1) - 1)
				{

					B_Tree_Split_child(pNode, index);

					if (_key_comp(p->data[index]->first, key))
					{
						DISK_WRITE(pNode->branch[index], W_NODIRTY);
						index++;
					}
					else {
						DISK_WRITE(pNode->branch[index + 1], W_NODIRTY);
					}

					child = pNode->branch[index];

					//free memory
					if (pNode == _root)
						DISK_WRITE(pNode, W_NOERASE);
					else DISK_WRITE(pNode);

				}
				//free memory
				else if (pNode != _root)
					DISK_WRITE(pNode);

				//insert key via recursve process
				B_Tree_Insert_NONFULL(child, key, value, _arg);

			}

		}//end function B_Tree_Insert_NONFULL();


		/*
		 * left rotate at position index of pNode.
		 * assume 3 nodes has already been read in the memory
		**/
		template<class Key, class Value>
		void B_TreeImpl<Key, Value>::B_Tree_Left_Rotate(base_ptr pNode, _STD size_t index)
		{
			base_ptr left = pNode->branch[index];
			base_ptr right = pNode->branch[index + 1];

			link_type p = static_cast<link_type>(pNode);
			link_type y = static_cast<link_type>(left);
			link_type z = static_cast<link_type>(rigt);

			y->data[left->_key_size++] = p->data[index];
			left->branch[left->_key_size] = right->branch[0];
			p->data[index] = z->data[0];

			right->_key_size--;
			for (int i = 0; i < right->_key_size; ++i)
			{
				z->data[i] = z->data[i + 1];
				right->branch[i] = right->branch[i + 1];
			}
			right->branch[right->_key_size] = right->branch[right->_key_size + 1];

			//write back
			DISK_WRITE(pNode, W_NOERASE);
			DISK_WRITE(left, W_NOERASE);
			DISK_WRITE(right, W_NOERASE);

		}


		/*
		 * right rotate at position index of pNode.
		 * assume 3 nodes has already been read in the memory
		**/
		template<class Key, class Value>
		void B_TreeImpl<Key, Value>::B_Tree_Right_Rotate(base_ptr pNode, _STD size_t index)
		{
			base_ptr left = pNode->branch[index - 1];
			base_ptr right = pNode->branch[index];

			link_type p = static_cast<link_type>(pNode);
			link_type y = static_cast<link_type>(left);
			link_type z = static_cast<link_type>(rigt);

			right->branch[right->_key_size + 1] = right->branch[right->_key_size];
			for (int i = right->_key_size; i > 0; --i)
			{
				z->data[i] = z->data[i - 1];
				right->branch[i] = right->branch[i - 1];
			}
			right->_key_size++;

			z->data[0] = p->data[index - 1];
			right->branch[0] = left->branch[left->_key_size];
			p->data[index - 1] = y->data[--left->_key_size];

			//write back
			DISK_WRITE(pNode, W_NOERASE);
			DISK_WRITE(left, W_NOERASE);
			DISK_WRITE(right, W_NOERASE);

		}


		/*
		 * erase a key in the subtree pNode
		 * assume pNode is big enough,
		 * which means pNode has no less than _degree key_words.
		**/
		template<class Key, class Value>
		void B_TreeImpl<Key, Value>::B_Tree_erase_BIGENOUGH(base_ptr pNode, const Key& key)
		{

			link_type p = static_cast<link_type>(pNode);

			//pNode is a leaf node
			if (pNode->leaf())
			{
				for (int i = 0; i < pNode->_key_size; ++i)
					if (!_key_comp(p->data[i]->first, key) && !_key_comp(key, p->data[i]->first))
					{
						for (int j = i; i < pNode->_key_size - 1; ++j)
							p->data[j] = p->data[j + 1];
						pNode->_key_size--;
						DISK_WRITE(pNode);
						return;
					}

				DISK_WRITE(pNode, W_NODIRTY);

				throw BTEx("There is no such key.");

			}//end if pNode is leaf

			//pNode is an internal node
			else {

				//find the appropriate position of the key
				int index = 0;
				while (index < pNode->_key_size && _key_comp(p->data[index]->first, key)) //p.key_i < key
					index++;
				//now index == size
				//or key is in (p.key_{i-1} / inf if index==0, p.key_i];


				//judge whether the key collides
				if (index != pNode->_key_size && !_key_comp(key, p->data[index]->first))
				{
					//process key collision in the internal node









				}//end if key collision in the intrenal node


				//no collision, index == size,
				//or key is in (key_{i-1}, key_i),
				//key might be in p->data[index].
				else {

					DISK_READ(pNode->branch[index]);

					//if child is not big enough
					//if neighbor sibling has enough, then steal one by rotating.
					if (pNode->branch[index]->_key_size == _degree - 1)
					{

						bool _OK = false;
						//right sibling
						if (index != pNode->_key_size)
						{
							//load right and rotate left
							DISK_READ(pNode->branch[index + 1]);
							if (pNode->branch[index + 1]->_key_size != _degree - 1)
								B_Tree_Left_Rotate(pNode, index);

							_OK = true;
							//free sibling
							DISK_WRITE(pNode->branch[index + 1], W_NODIRTY);

						}

						//left sibling
						if (index > 0 && !_OK)
						{
							//load left and rotate right
							DISK_READ(pNode->branch[index - 1]);
							if (pNode->branch[index - 1]->_key_size != _degree - 1)
								B_Tree_Right_Rotate(pNode, index);

							_OK = true;
							//free sibling
							DISK_WRITE(pNode->branch[index - 1], W_NODIRTY);

						}

						//not enough, merge with one neighbor
						if (!_OK)
						{

							//push down one key in pNode,
							//which serves as thee middle key.

							//merge with right sibling
							if (index != pNode->_key_size)
							{

								DISK_READ(pNode->branch[index + 1]);

								base_ptr left = pNode->branch[index];
								base_ptr right = pNode->branch[index + 1];

								link_type y = static_cast<link_type>(left);
								link_type z = static_cast<link_type>(right);

								y->data[_degree - 1] = p->data[index];
								for (int i = 0; i < _degree - 1; ++i)
								{
									y->data[_degree + i] = z->data[i];
									left->branch[_degree + i] = right->branch[i];
								}
								left->branch[(_degree << 1) - 1] = right->branch[_degree - 1];
								left->_key_size = (_degree << 1) - 1;

								//free, lest memory leak
								DISK_WRITE(right, W_NODIRTY);

								//shift key and data in pNode from index to size
								for (int i = index; i < pNode->_key_size - 1; ++i)
								{
									pNode->branch[i] = pNode->branch[i + 1];
									p->data[i + 1] = p->data[i + 2];
								}
								pNode->_key_size--;

								DISK_WRITE(pNode, W_NOERASE);
								DISK_WRITE(left, W_NOERASE);

							}
							//merge with left sibling
							//in fact at this time index == key_size !!!!
							else {

								DISK_READ(pNode->branch[index - 1]);

								base_ptr left = pNode->branch[index - 1];
								base_ptr right = pNode->branch[index];

								link_type y = static_cast<link_type>(left);
								link_type z = static_cast<link_type>(right);

								y->data[_degree - 1] = p->data[index - 1];
								for (int i = 0; i < _degree - 1; ++i)
								{
									y->data[_degree + i] = z->data[i];
									left->branch[_degree + i] = right->branch[i];
								}
								left->branch[(_degree << 1) - 1] = right->branch[_degree - 1];
								left->_key_size = (_degree << 1) - 1;

								//free, lest memory leak
								DISK_WRITE(right, W_NODIRTY);

								pNode->_key_size--;

								DISK_WRITE(pNode, W_NOERASE);
								DISK_WRITE(left, W_NOERASE);

							}


						}//end merge with ones neighbor

					}//end child is not big enough


					//big enough, do recursive process
					base_ptr child = pNode->branch[index];

					//free memory
					DISK_WRITE(pNode, W_NODIRTY);

					B_Tree_erase_BIGENOUGH(child, key);

				}//end if no key collision in this pNode


			}//end if pNode is an internal node


		}//end function B_Tree_erase_BIGENOUGH():


	}//end namespace BTree

}//end namespace RSY_TOOL

#endif // !_B_TREEIMPL_H
