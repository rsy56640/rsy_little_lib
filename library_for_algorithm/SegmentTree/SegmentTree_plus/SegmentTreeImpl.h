#pragma once
#ifndef _SEGMENTTREEIMPL_H
#define _SEGMENTTREEIMPL_H
#include "SegmentTreeNode.h"
#include "SegmentTreeException.h"
#include <vector>
#include <algorithm>
using std::vector;

//SegmentTreeImpl Template
template<class _Ty> class SegmentTreeImpl :public SegmentTreeType<_Ty>
{

public:

	//customized function constructor
	SegmentTreeImpl(const vector<_Ty>& Vec, const Func& func, const _Ty& Identity_Element)
		: _size(1),
		_Func(func),
		_Identity_Element(Identity_Element)
	{
		//空检查
		if (Vec.empty())
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		//初始化线段树，空间复杂度O(n)，时间复杂度O(n)
		find_size(Vec.size());
		ST.resize(2 * _size - 1);
		build(0, 0, _size - 1, Vec);
		aug.resize(_size - 1);
		std::for_each(aug.begin(), aug.end(),
			[&Identity_Element](_Ty& val) {val = Identity_Element; });

	}


	const _Ty query(const int start, const int end)
	{

		if (start > end)
			throw SegmentTreeException<_Ty>("The querying range is invalid!!");


		return doQuery(0, start, end);

	}


	void modify(const int index, _Ty&& value)
	{

		if (index < ST[0]->start() || index > ST[0]->end())
			throw SegmentTreeException<_Ty>("The Index is invalid!!");


		modify(index, index, _STD move(value));

	}


	void modify(const int index, const modify_func& func)
	{

		if (index < ST[0]->start() || index > ST[0]->end())
			throw SegmentTreeException<_Ty>("The Index is invalid!!");


		modify(index, index, func);

	}


	//lazy evaluation with modify value
	void modify(const int start, const int end, const _Ty& value)
	{

		if (start < ST[0]->start() || end > ST[0]->end() || start > end)
			throw SegmentTreeException<_Ty>("The modified range is invalid!!");


		//doModify();

	}


	//lazy evaluation with modify_function
	void modify(const int start, const int end, const modify_func& func)
	{

		if (start < ST[0]->start() || end > ST[0]->end() || start > end)
			throw SegmentTreeException<_Ty>("The modified range is invalid!!");


		//doModify();

	}


	SegmentTreeImpl(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl& operator=(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl(SegmentTreeImpl&&) = delete;

	SegmentTreeImpl& operator=(SegmentTreeImpl&&) = delete;

	~SegmentTreeImpl() = default;


protected:

	//线段树最底层长度，总长度 (2 * _size - 1)
	int _size;

	vector<SegmentTreeNode_ptr> ST;	//Segment_Tree with size of (2 * _size - 1)

	//accumulation for lazy evaluation with size of (_size - 1)
	//aug[index] represents the value that should be later
	//	applied to the any of the values below.
	vector<_Ty> aug;

	//_Func是一个_Ty上的二元代数运算符，满足结合律、交换律，有幺元。
	//_Ty对_Func构成一个 可交换幺半群
	const Func _Func;

	//幺元
	const _Ty _Identity_Element;


private:

	//find_size
	void find_size(const int size)
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
			_STD move
			(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));

	}


	//querying the total value interval [start, end] applied by _Func
	//@	Parameter list:
	//@		int index:	current node
	//@		int start:	start of querying rang
	//@		int end:	end of querying range
	_Ty doQuery(int index, int start, int end)
	{

		int left = ST[index]->start();
		int right = ST[index]->end();

		//no segment union
		if (start > right || end < left)
			return _Identity_Element;


		//querying segment includes root segment
		if (start <= left && right <= end)
			return ST[index]->val();

		//update to the next level
		pushDown(index);

		//partially coincide
		return _Func(doQuery((index << 1) + 1, start, end),
			doQuery((index << 1) + 2, start, end));

	}



	//suppose that the upper level is satisfied. (including this level)
	//push down the accumulation to the next level.
	//@ Parameter list:
	//@		const int index: the interval num		
	void pushDown(const int index)
	{

		if (index >= _size - 1)return;

		//something to do
		if (!(aug[index] == _Identity_Element))
		{

			const _Ty& _value = aug[index];

			//accumulate funciton to update the the interval value of the next level.
			const int interval_length = (ST[index]->end() - ST[index]->start() + 1) / 2;
			std::function<void(SegmentTreeNode_ptr&)> _func_accumulate =
				[this, interval_length, &_value](SegmentTreeNode_ptr& STNode_ptr)->void
			{
				_Ty& value = const_cast<_Ty&>(STNode_ptr->value());
				for (int i = 0; i < interval_length; ++i)
				{
					value = _STD move(_Func(value, _value));
				}
				STNode_ptr->setValue(_STD move(value));
			};

			_func_accumulate(ST[(index << 1) + 1]);		//value update
			_func_accumulate(ST[(index << 1) + 2]);		//value update

			aug[index] = _Identity_Element;				//augmentation update


			//if index is not at the bottom level of the augmentation field,
			//then update the augmentation field of the next level.
			if (index * 2 + 1 < _size - 1)
			{
				auto _func = [this, &_value](_Ty& aug_value)->void
				{
					aug_value = _STD move(_Func(aug_value, _value));
				};

				//update the next level's augmentation field.
				_func(aug[(index << 1) + 1]);
				_func(aug[(index << 1) + 2]);
			}

		}//no augmentation

	}


	//ranging updating
	//@ Parameter list:
	//@		const int index:	the index of the interval
	//@		const int start:	start of modifing intereval
	//@		const int end:		end of modifying interval
	//@		const _Ty& value:	modifying value
	//@ Promise:
	void doModify(const int index, const int start, const int end, const _Ty& value)
	{

		int left = ST[index]->start();
		int right = ST[index]->end();

		//no intersection
		if (start > right || end < left)return;

		//the current range is covered within the modifying range. 
		if (start <= left && end >= right)
		{
			//find the corresponding interval and update
			//and stop to update
			const int _length = right - left + 1;
			_Ty& temp = ST[index]->value();
			for (int i = 0; i < _length; ++i)
				temp = _STD move(_Func(temp, value));
			ST[index]->setValue(_STD move(temp));

			//set the augmentation field
			aug[index] = _STD move(_Func(aug[index], value));
		}

		//some intersecton
		//so update to the next level
		else {
			pushDown(index);
			doModify((index << 1) + 1, start, end, value);
			doModify((index << 1) + 2, start, end, value);
			ST[index]->setValue(
				_STD move
				(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));
		}


	}



};

#endif // !_SEGMENTTREEIMPL_H
