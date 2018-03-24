#pragma once
#ifndef _BITIMPL_H
#define _BITIMPL_H
#include <vector>
#include "BIT_Type.h"
#include "BIT_Exception.h"

template<typename _Ty> class BITImpl :public BIT_type<_Ty>
{

public:

	//@ Parameter List:
	//@		Vec: the vector to be processed.
	//@		func: form Abelian group for the type _Ty.
	//@		identity_element: identity element.
	//@		inverse_function: find one's inverse element.
	BITImpl(const _STD vector<_Ty>& Vec, Func func,
		_Ty identity_element, Inverse_Funciton inverse_function)
		:
		_Func(_STD forward<Func>(func)),
		_identity_element(_STD forward<_Ty>(identity_element)),
		_Inverse_Func(_STD forward<Inverse_Funciton>(inverse_function))
	{

		if (Vec.empty())
			throw BIT_Exception("The Vector is empty!!");

		int size = Vec.size();
		_Vec.resize(size);
		for (int i = 0; i < size; ++i)
		{
			_Vec[i] = 0;
			int _low_bit = low_bit(i + 1);
			for (int j = i - _low_bit + 1; j <= i; ++j)
				_Vec[i] = _Func(_Vec[i], Vec[j]);
		}
	}


	//query te previous n elements value
	//@ Parameter List:
	//@		start:	the start index
	//@		end:	the end index
	const _Ty query(const int start, const  int end) const
	{

		if (start > end || start < 0 || end >= static_cast<int>(_Vec.size()))
			throw BIT_Exception(
				_STD string("The querying range: ")
				.append(_STD to_string(start))
				.append(" - ")
				.append(_STD to_string(end))
				.append(" is invalid!!")
			);

		return doQuery(start, end);

	}


	//modify the index as val
	//@ Parameter List:
	//@		index:	the position to be modified.
	//@		val:	the modifying value.
	void modify(const int index, _Ty&& val)
	{

		if (index < 0 || index >= static_cast<int>(_Vec.size()))
			throw BIT_Exception(
				_STD string("The modify index: ")
				.append(_STD to_string(index))
				.append(" is invalid!!")
			);

		doModify(index, _STD move(val));

	}



	//modify pos with the function
	//@ Parameter List:
	//@		pos: the pos to be modified.
	//@		func: modify function.
	void modify(const int index, const modify_func& func)
	{

		if (index < 0 || index >= static_cast<int>(_Vec.size()))
			throw BIT_Exception(
				_STD string("The modify index: ")
				.append(_STD to_string(index))
				.append(" is invalid!!")
			);

		doModify(index, func);

	}


	BITImpl(const BITImpl&) = delete;

	BITImpl(BITImpl&&) = delete;

	BITImpl& operator=(const BITImpl&) = delete;

	BITImpl& operator=(BITImpl&&) = delete;


protected:

	_STD vector<_Ty> _Vec;

	//_Func 对类型 _Ty 构成Abel群
	const Func _Func;

	//求逆运算
	const Inverse_Funciton _Inverse_Func;

	//幺元
	const _Ty _identity_element;


private:


	const int low_bit(const int x) const noexcept
	{
		return x & (-x);
	}


	const int high_bit(const int x) const noexcept
	{
		const static _STD vector<int> bit =
		{
			0, 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8,
			1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15, 1 << 16,
			1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24,
			1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30, 1 << 31,
		};

		//binary search to find the high bit
		int low = 0, high = 32, mid = 16;
		while (high - low > 1)
		{
			if (x >= bit[mid])
				low = mid;
			else high = mid;
			mid = (low + high) / 2;
		}
		return bit[low];
	}


	//find the "1" stream from high-bit to low-bit
	//ex:	n = 11 1011, m = 11 0110;	n = 11 1011, m = 01 0110;
	//then return 11 0000;					0;
	//@ Parameter List:
	//@		n >= m
	const int find_stop(const int m, const int n) const noexcept
	{
		const int highBit = high_bit(n);
		if (highBit > m)return 0;
		const int p = n & m;
		int p_highBit = high_bit(p), counter = 1;
		//find how many successive bits is 1
		while (p_highBit)
		{
			p_highBit >>= 1;
			if (p & p_highBit)counter++;
			else break;
		}
		//form the result according to counter
		int result = 0;
		for (int i = 0; i < counter; ++i)
		{
			p_highBit <<= 1;
			result += p_highBit;
		}
		return result;
	}


	//S_(n-1)= A0+...+A_(n-1)
	//S_(n-1)=C_(n-1) + S_(n - 1 - low_bit(n));
	//@ Parameter LIst:
	//@		start:	the start index
	//@		end:	the end index
	const _Ty doQuery(const int start, const int end) const
	{
		int _start = start, _end = end;
		_Ty result1 = _identity_element;
		_Ty result2 = _identity_element;

		//identifier "stop" represents the bottom of the sum
		//such as: [26, 28] = S_28 - S_25 = (S_28 - S_23) - (S_25 - S_23);
		//in which stop = 24;
		const int stop = find_stop(start, end + 1);

		while (_end >= stop)
		{
			result1 = _STD forward<_Ty>(_Func(result1, _Vec[_end]));
			_end -= low_bit(_end + 1);
		}

		_start--;
		while (_start >= stop)
		{
			result2 = _STD forward<_Ty>(_Func(result2, _Vec[_start]));
			_start -= low_bit(_start + 1);
		}

		return _Func(result1, _Inverse_Func(result2));

	}


	//modify the pos as val
	//modify C_n,
	//then n += low_bit(n+1);
	//@ Parameter List:
	//@		index:	the position to be modified.
	//@		val:	the modifying value.
	void doModify(const int index, _Ty&& val)
	{
		int size = _Vec.size();
		const _Ty _pos_val = doQuery(index, index);
		const _Ty diff = _Func(_Inverse_Func(_pos_val), val);	//augmentation
		auto _func = [this, &diff](_Ty& other)			//apply the augmentation
		{other = _STD forward<_Ty>(_Func(other, diff)); };
		for (int _index = index; _index < size;)
		{
			_func(_Vec[_index]);
			_index += low_bit(_index + 1);
		}
	}


	//modify pos with the function
	//@ Parameter List:
	//@		index:	the pos to be modified.
	//@		func:	modify function.
	void doModify(const int index, const modify_func& func)
	{
		int size = _Vec.size();
		const _Ty _pos_val = doQuery(index, index);
		const _Ty diff = _Func(_Inverse_Func(_pos_val), func(_pos_val));	//augmentation
		auto _func = [this, &diff](_Ty& other)				//apply the augmentation
		{other = _STD forward<_Ty>(_Func(other, diff)); };
		for (int _index = index; _index < size;)
		{
			_func(_Vec[_index]);
			_index += low_bit(_index + 1);
		}
	}


};
#endif // !_BITIMPL_H
