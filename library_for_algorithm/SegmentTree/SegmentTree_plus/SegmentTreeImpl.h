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

	typedef struct {} Commutive;
	typedef struct {} NoCommutive;

public:

	//customized function constructor
	SegmentTreeImpl(const vector<_Ty>& Vec, const Func& func, const _Ty& Identity_Element)
		: _size(1),
		_Func(func),
		_Identity_Element(Identity_Element)
	{
		//�ռ��
		if (Vec.empty())
			throw SegmentTreeException<_Ty>("The Segment is empty!!");


		//��ʼ���߶������ռ临�Ӷ�O(n)��ʱ�临�Ӷ�O(n)
		find_size(Vec.size());
		ST.resize(2 * _size - 1);
		build(0, 0, _size - 1, Vec);
		aug.resize(_size - 1);
		std::for_each(aug.begin(), aug.end(),
			[&Identity_Element](_Ty& val) {val = Identity_Element; });
		value_cache.resize(_size - 1);
		std::for_each(value_cache.begin(), value_cache.end(),
			[&Identity_Element](_Ty& val) {val = Identity_Element; });

	}


	const _Ty query(const int start, const int end)
	{

		if (start > end)
			throw SegmentTreeException<_Ty>("The querying range is invalid!!");


		return doQuery(0, start, end);

	}


	void modify(const int index, _Ty&& aug_value)
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
	void modify_augment(const int start, const int end, const _Ty& aug_value)
	{

		if (start < ST[0]->start() || end > ST[0]->end() || start > end)
			throw SegmentTreeException<_Ty>("The modified range is invalid!!");


		doModify(0, start, end, aug_value);

	}


	//lazy evaluation with modify_function
	void modify(const int start, const int end, const modify_func& func)
	{

		if (start < ST[0]->start() || end > ST[0]->end() || start > end)
			throw SegmentTreeException<_Ty>("The modified range is invalid!!");


		/*
		doModify(0,start,end,
			_STD move
			(func(doQuery()))
			);
			*/

	}


	SegmentTreeImpl(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl& operator=(const SegmentTreeImpl&) = delete;

	SegmentTreeImpl(SegmentTreeImpl&&) = delete;

	SegmentTreeImpl& operator=(SegmentTreeImpl&&) = delete;

	~SegmentTreeImpl() = default;


protected:

	//�߶�����ײ㳤�ȣ��ܳ��� (2 * _size - 1)
	int _size;

	vector<SegmentTreeNode_ptr> ST;	//Segment_Tree with size of (2 * _size - 1)

	//accumulation for lazy evaluation with size of (_size - 1)
	//aug[index] represents the value that should be later
	//	applied to the any of the values below.
	vector<_Ty> aug;

	//value cache:
	//store the value that represents the any element in the range
	//it should be updated when necessary
	vector<_Ty> value_cache;

	//_Func��һ��_Ty�ϵĶ�Ԫ������������������ɡ������ɣ�����Ԫ��
	//_Ty��_Func����һ�� �ɽ����۰�Ⱥ
	const Func _Func;

	//��Ԫ
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


	//suppose that the upper level is satisfied. (including this level)!!!
	//push down the accumulation to the next level.
	//
	//@ Parameter list:
	//@		const int index: the interval num
	void pushDown(const int index)
	{

		if (index >= _size - 1)return;

		const int sub_interval_length = (ST[index]->end() - ST[index]->start() + 1) / 2;

		//if the value_cache store the value of the range,
		//then set the next 2 child appropriate.
		if (!(value_cache[index] == _Identity_Element))
		{
			const _Ty& value = value_cache[index];
			_Ty temp = _Identity_Element;
			for (int i = 0; i < sub_interval_length; ++i)
				temp = _STD move(_Func(temp, value));
			_Ty temp2{ temp };
			ST[(index << 1) + 1]->setValue(_STD move(temp));
			ST[(index << 1) + 2]->setValue(_STD move(temp2));
			if ((index << 1) + 1 < _size - 1)
			{
				value_cache[(index << 1) + 1] = value;
				value_cache[(index << 1) + 2] = value;
				aug[(index << 1) + 1] = _Identity_Element;
				aug[(index << 1) + 2] = _Identity_Element;
			}
			return;
		}


		//something to do
		if (!(aug[index] == _Identity_Element))
		{

			const _Ty& _value = aug[index];


			//accumulate funciton to update the the interval value of the next level.
			std::tr1::function<void(SegmentTreeNode_ptr&)> _func_accumulate =
				[this, sub_interval_length, &_value](SegmentTreeNode_ptr& STNode_ptr)->void
			{
				_Ty& value = const_cast<_Ty&>(STNode_ptr->value());
				for (int i = 0; i < sub_interval_length; ++i)
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
			if ((index << 1) + 1 < _size - 1)
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



	/**********************************************************\
	* modify the range with substitution of the specific value *
	\**********************************************************/
	//@ Parameter list:
	//@		index:
	//@		start:
	//@		end:
	//@		value
	void doModify(const int index, const int start, const int end, const _Ty& value)
	{


	}


	//range updating with a augment value
	//@ Parameter list:
	//@		const int index:	the index of the interval
	//@		const int start:	start of modifing intereval
	//@		const int end:		end of modifying interval
	//@		const _Ty& value:	modifying value
	//@ Promise:
	void doModify_augment
	(const int index, const int start, const int end, const _Ty& aug_value)
	{

		int left = ST[index]->start();
		int right = ST[index]->end();

		//no intersection
		if (start > right || end < left)return;

		//the current range is covered within the modifying range. 
		if (start <= left && end >= right)
		{

			const int _length = right - left + 1;

			//if the value of the range is not set appropriate,
			//reset the value_cache with the augment value.
			if (!(value_cache[index] == _Identity_Element))
			{
				aug[index] = _Identity_Element;
				value_cache[index] = _STD move(_Func(value_cache[index], aug_value));
				const _Ty& value = value_cache[index];
				_Ty temp = _Identity_Element;
				for (int i = 0; i < _length; ++i)
					temp = _STD move(_Func(temp, value));
				ST[index]->setValue(_STD move(temp));
				return;
			}


			//find the corresponding interval and update
			//and stop to update
			_Ty temp = ST[index]->value();
			for (int i = 0; i < _length; ++i)
				temp = _STD move(_Func(temp, aug_value));
			ST[index]->setValue(_STD move(temp));

			//set the augmentation field
			aug[index] = _STD move(_Func(aug[index], aug_value));

		}


		//some intersecton
		//so update to the next level
		else {
			pushDown(index);
			doModify((index << 1) + 1, start, end, aug_value);
			doModify((index << 1) + 2, start, end, aug_value);
			ST[index]->setValue(
				_STD move
				(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));
		}


	}



	/****************************************************************************/
	//2018-3-27
	//specialization for different type of function
	/******************************************************************************/
	/*************************************************************************\
	*@ Promise: func type should be commutive semi-group against the type _Ty *
	\*************************************************************************/
	//ranging updating with modifying function
	//@ Parameter list:
	//@
	//@
	//@
	//@		modify_func:
	void doModify(const int index, const int start, const int end, const modify_func& func)
	{

		int left = ST[index]->start();
		int right = ST[index]->end();

		//no intersection
		if (start > right || end < left)return;


		//the current range is covered within the modifying range. 
		if (start <= left && end >= right)
		{
			const int _length = right - left + 1;
			_Ty temp = _Identity_Element;
			for (int i = 0; i < _length; ++i)
			{
				temp = _STD move(func(temp));
			}
			ST[index]->setValue(_STD move(_Func(ST[index]->value(), temp)));

			//update augmentation field
		//	aug[index] = 
				//_STD move(_Func(aug[index],));

			return;
		}

		auto _func = [](_Ty& value)->decltype(auto)
		{

		};

		//some intersecton
		//so update to the next level
		else {


		}

	}


};

#endif // !_SEGMENTTREEIMPL_H
