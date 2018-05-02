#include "B_Tree_Node.h"
#include "B_Tree_Exception.h"

namespace RSY_TOOL
{

	namespace BTree
	{


		const bool isLeaf(const int degree_isLeaf)
		{
			return degree_isLeaf & 1;
		}


		const _STD size_t degree(const int degree_isLeaf)
		{
			return degree_isLeaf >> 1;
		}


		void setLeaf(int& _src_degree_isLeaf, const int _dest_degree_isLeaf)
		{
			_src_degree_isLeaf = (_src_degree_isLeaf & ~1) | (_dest_degree_isLeaf & 1);
		}


		/*
		 * find the minimum key in the subtree pNode
		**/
		_STD pair<B_Tree_Node_Base*, int> minimum(B_Tree_Node_Base* pNode)
		{
			B_Tree_Node_Base* result = pNode;
			while (!result->leaf())
			{
				DISK_READ(result->branch[0]);
				auto temp = result;
				result = result->branch[0];
				if (temp != pNode)
					DISK_WRITE(temp, W_NODIRTY);
			}
			return _STD make_pair(result, 0);
		}


		/*
		 * find the maximum key in the subtree pNode
		**/
		_STD pair<B_Tree_Node_Base*, int> maximum(B_Tree_Node_Base* pNode)
		{
			B_Tree_Node_Base* result = pNode;
			while (!result->leaf())
			{
				DISK_READ(result->branch[result->_key_size]);
				auto temp = result;
				result = result->branch[result->_key_size];
				if (temp != pNode)
					DISK_WRITE(temp, W_NODIRTY);
			}
			return _STD make_pair(result, result->_key_size - 1);
		}


		/*
		 * decrement
		**/
		_STD pair<B_Tree_Node_Base*, int> decrement(B_Tree_Node_Base* _node, int _index)
		{

			//no precursor
			if (_node->leaf() && _index == 0)
			{
				//find root
				B_Tree_Node_Base* root = _node;
				while (root->parent != nullptr)
				{
					DISK_READ(root->parent);
					auto temp = root;
					root = root->parent;
					DISK_WRITE(temp, W_NODIRTY);
				}

				//if _node is left-most
				if (_node == minimum(root).first)
					throw BTreeException("no precursor");

			}

			//trivial case: leaf node
			if (_node->leaf() && _index > 0)
			{
				return _STD make_pair(_node, --_index);
			}

			//precursor is in the other node

			//the first key of the leaf
			if (_node->leaf())
			{
				B_Tree_Node_Base* result = _node;

				//index indicats the number of this node is in the parent.
				int index = 0;

				//backtrack until index > 0
				while (index == 0)
				{
					DISK_READ(result->parent);
					auto parent = result->parent;
					auto temp = result;
					index = 0;
					while (parent->branch[index] != result)
						index++;
					DISK_WRITE(temp, W_NODIRTY);
					result = parent;
				}

				//then key should be key_{i-1}
				return _STD make_pair(result, index - 1);

			}
			//internal node
			else
			{
				auto temp = _node;
				DISK_READ(_node->branch[_index]);
				DISK_WRITE(temp->branch[_index], W_NODIRTY);
				DISK_WRITE(temp, W_NODIRTY);
				return maximum(_node->branch[_index]);
			}

		}//end function decrement();


		/*
		 * increment
		**/
		_STD pair<B_Tree_Node_Base*, int> increment(B_Tree_Node_Base* _node, int _index)
		{

			//no successor
			if (_node->leaf() && _index == _node->_key_size - 1)
			{
				B_Tree_Node_Base* root = _node;
				//find root
				while (root->parent != nullptr)
				{
					DISK_READ(root->parent);
					auto temp = root;
					root = root->parent;
					DISK_WRITE(temp, W_NODIRTY);
				}
				//if _node is right-most
				if (_node == maximum(root).first)
					throw BTreeException("no successor");
			}

			//trivial case: leaf node
			if (_node->leaf() && _index < _node->_key_size - 1)
			{
				return _STD make_pair(_node, ++_index);
			}

			//successor is in the other node

			//the last key of the leaf
			if (_node->leaf())
			{
				B_Tree_Node_Base* result = _node;

				//index indicats the number of this node is in the parent.
				int index = result->_key_size - 1;

				//backtrack until index > 0
				while (index == result->_key_size - 1)
				{
					DISK_READ(result->parent);
					auto parent = result->parent;
					auto temp = result;
					index = parent->_key_size - 1;
					while (parent->branch[index] != result)
						index--;
					DISK_WRITE(temp, W_NODIRTY);
					result = parent;
				}

				//then key should be key_i
				return _STD make_pair(result, index);

			}
			//internal node
			else
			{
				auto temp = _node;
				DISK_READ(_node->branch[_index + 1]);
				DISK_WRITE(temp->branch[_index + 1], W_NODIRTY);
				DISK_WRITE(temp, W_NODIRTY);
				return minimum(_node->branch[_index + 1]);
			}

		}

		B_Tree_Node_Base::B_Tree_Node_Base(const _STD size_t degree)
			:degree_isLeaf((degree << 1) | 1), _key_size(0), branch(nullptr), parent(nullptr)
		{
			branch = new typename B_Tree_Node_Base::base_ptr[degree << 1];
		}


		B_Tree_Node_Base::~B_Tree_Node_Base()
		{
			delete[] branch;
		}


		const _STD size_t B_Tree_Node_Base::degree()const noexcept
		{
			return BTree::degree(this->degree_isLeaf);
		}


		const bool B_Tree_Node_Base::leaf()const noexcept
		{
			return isLeaf(this->degree_isLeaf);
		}


		void DISK_WRITE(B_Tree_Node_Base* pNode, bool _mode)
		{

			//...
			//write to the disk or some device
			//...

			//if (_mode);
			//delete pNode;

		}


		void DISK_READ(B_Tree_Node_Base* pNode)
		{

			//...
			//read information from the disk or some device
			//...

		}


	}//end namespace BTree

}//end namespace RSY_TOOL
