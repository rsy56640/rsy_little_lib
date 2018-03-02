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
		: _size(1),
		_Func(func),
		_Identity_Element(Identity_Element),
		_checked(false)
	{
		//异常检查
		if (Vec.empty())
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		//初始化线段树，空间复杂度O(n)，时间复杂度O(n)
		find_size(Vec.size());
		ST.resize(2 * _size - 1);
		build(0, 0, _size - 1, Vec);
		_checked = true;

	}


	_Ty query(int start, int end) const
	{

		if (!_checked)
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		if (start > end)
			throw SegmentTreeException<_Ty>("The querying range is invalid!!");


		return doQuery(0, start, end);

	}


	void modify(int index, _Ty&& value)
	{

		if (!_checked)
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		if (index < ST[0]->start() || index > ST[0]->end())
			throw SegmentTreeException<_Ty>("The Index is invalid!!");


		doModify(0, index, _STD forward<_Ty>(value));

	}


	void modify(int index, modify_func func)
	{

		modify(index, func(query(index, index)));

	}



	SegmentTreeImpl(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl& operator=(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl(SegmentTreeImpl&&) = delete;

	SegmentTreeImpl& operator=(SegmentTreeImpl&&) = delete;

	~SegmentTreeImpl() = default;


protected:

	//线段树最底层长度，总长度 (2 * _size - 1)
	int _size;

	vector<SegmentTreeNode_ptr> ST;

	//_Func是一个_Ty上的二元代数运算符，满足结合律，有幺元，_Ty对_Func构成一个幺半群
	Func _Func;

	//幺元
	const _Ty _Identity_Element;

	//check out if SegmentTree exists
	bool _checked;

private:

	//find_size
	void find_size(int size)
	{
		while (_size < size)
		{
			_size <<= 1;
		}
	}


	//SegmentTree Initialization
	void build(int index, int start, int end, const vector<_Ty>& Vec)
	{

		//leaf node
		if (start == end)
		{
			ST[index] = make_shared<SegmentTreeNode<_Ty> >
				(start, end, (start < Vec.size()) ? Vec[start] : _Identity_Element);
			return;
		}

		//internal node (non-leaf)
		int mid = (start + end) / 2;


		//construct this node with initial val(_Identity_Element)
		ST[index] = make_shared<SegmentTreeNode<_Ty> >(start, end, _Identity_Element);


		//construct left and right subTree (recursion)
		build((index << 1) + 1, start, mid, Vec);
		build((index << 1) + 2, mid + 1, end, Vec);


		//set value
		ST[index]->setValue(
			_STD forward<_Ty>
			(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));

	}


	//index: cur_node
	_Ty doQuery(int index, int start, int end) const
	{

		//no segment union
		if (start > ST[index]->end() || end < ST[index]->start())
			return _Identity_Element;


		//querying segment includes root segment
		if (start <= ST[index]->start() && ST[index]->end() <= end)
			return ST[index]->val();


		//partially coincide
		return _Func(doQuery((index << 1) + 1, start, end),
			doQuery((index << 1) + 2, start, end));

	}


	//index: cur_node		//_index: modified_node
	void doModify(int index, int _index, _Ty&& value)
	{

		//leaf node found
		if (ST[index]->start() == ST[index]->end() && ST[index]->start() == _index)
		{
			ST[index]->setValue(_STD forward<_Ty>(value));
			return;
		}

		//not found
		int mid = (ST[index]->start() + ST[index]->end()) / 2;

		//left subTree
		if (_index <= mid)
		{
			doModify((index << 1) + 1, _index, _STD forward<_Ty>(value));
			ST[index]->setValue(
				_STD forward<_Ty>
				(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));
		}

		//right subTree
		else
		{
			doModify((index << 1) + 2, _index, _STD forward<_Ty>(value));
			ST[index]->setValue(
				_STD forward<_Ty>
				(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));
		}

	}


};

#endif // !_SEGMENTTREEIMPL_H
