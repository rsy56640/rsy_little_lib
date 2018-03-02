#pragma once
#ifndef _SEGMENTTREEIMPL_H
#include "SegmentTreeNode.h"
#include "SegmentTreeException.h"
#include <vector>
using std::vector;

//SegmentTreeImpl Template
template<class _Ty> class SegmentTreeImpl :public SegmentTreeType<_Ty>
{

public:

	//customized function constructor
	SegmentTreeImpl(const vector<_Ty>& Vec, Func func, _Ty Identity_Element)
		: _root(nullptr), _Func(func), _Identity_Element(Identity_Element), _checked(false)
	{
		//异常检查
		if (Vec.empty())
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		//初始化线段树，空间复杂度O(n)，时间复杂度O(n)
		_root = build(0, Vec.size() - 1, Vec);
		_checked = true;

	}


	_Ty query(int start, int end) const
	{

		if (!_checked)
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		if (start > end)
			throw SegmentTreeException<_Ty>("The querying range is invalid!!");


		return doQuery(_root, start, end);

	}


	void modify(int index, _Ty&& value)
	{

		if (!_checked)
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		if (index<_root->start() || index>_root->end())
			throw SegmentTreeException<_Ty>("The Index is invalid!!");


		doModify(_root, index, _STD forward<_Ty>(value));

	}


	void modify(int index, modify_func func)
	{

		if (!_checked)
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		if (index<_root->start() || index>_root->end())
			throw SegmentTreeException<_Ty>("The Index is invalid!!");


		doModify(_root, index,
			_STD forward<_Ty>
			(func(query(index, index)))
		);

	}



	SegmentTreeImpl(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl& operator=(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl(SegmentTreeImpl&&) = delete;

	SegmentTreeImpl& operator=(SegmentTreeImpl&&) = delete;

	~SegmentTreeImpl() = default;


protected:

	SegmentTreeNode_ptr _root;

	//_Func是一个_Ty上的二元代数运算符，满足结合律，有幺元，_Ty对_Func构成一个幺半群
	Func _Func;

	//幺元
	const _Ty _Identity_Element;

	//check out if SegmentTree exists
	bool _checked;


private:


	//SegmentTree Initialization
	SegmentTreeNode_ptr build(int start, int end, const vector<_Ty>& Vec)
	{

		//leaf node
		if (start == end)
			return make_shared<SegmentTreeNode<_Ty> >(start, end, Vec[start]);


		//internal node (non-leaf)
		int mid = (start + end) / 2;


		//construct this node with initial val(_Identity_Element)
		SegmentTreeNode_ptr node =
			make_shared<SegmentTreeNode<_Ty> >(start, end, _Identity_Element);


		//construct left and right subTree (recursion)
		node->left() = (build(start, mid, Vec));
		node->right() = (build(mid + 1, end, Vec));


		//set value
		node->setValue(
			_STD forward<_Ty>
			(_Func(node->left()->value(), node->right()->value())));

		return node;

	}


	//
	_Ty doQuery(SegmentTreeNode_ptr root, int start, int end) const
	{

		//no segment union
		if (start > root->end() || end < root->start())
			return _Identity_Element;


		//querying segment includes root segment
		if (start <= root->start() && root->end() <= end)
			return root->val();


		//partially coincide
		return _Func(doQuery(root->left(), start, end), doQuery(root->right(), start, end));

	}


	//
	void doModify(SegmentTreeNode_ptr root, int index, _Ty&& value)
	{

		//leaf node found
		if (root->start() == root->end() && root->start() == index)
		{
			root->setValue(_STD forward<_Ty>(value));
			return;
		}

		//not found
		int mid = (root->start() + root->end()) / 2;

		//left subTree
		if (index <= mid)
		{
			doModify(root->left(), index, _STD forward<_Ty>(value));
			root->setValue(
				_STD forward<_Ty>
				(_Func(root->left()->value(), root->right()->value())));
		}

		//right subTree
		else
		{
			doModify(root->right(), index, _STD forward<_Ty>(value));
			root->setValue(
				_STD forward<_Ty>
				(_Func(root->left()->value(), root->right()->value())));
		}

	}


};

#endif // !_SEGMENTTREEIMPL_H
