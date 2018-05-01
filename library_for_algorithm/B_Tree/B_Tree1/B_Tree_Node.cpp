#include "B_Tree_Node.h"

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


		B_Tree_Node_Base::B_Tree_Node_Base(const _STD size_t degree)
			:degree_isLeaf((degree << 1) | 1), _key_size(0), branch(nullptr)
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

			if (_mode);
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
