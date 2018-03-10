#pragma once
#ifndef _BITIMPL_H
#define _BITIMPL_H
#include <iostream> 
#include <vector>
#include "BIT_Type.h"
#include "BIT_Exception.h"

template<typename _Ty> class BITImpl :public BIT_type<_Ty>
{

public:

	//@ Parameter List:
	//@ Vec: the vector to be processed.
	//@ func: form Abelian group for the type _Ty.
	//@ identity_element: identity element.
	//@ inverse_function: find one's inverse element.
	BITImpl(const _STD vector<_Ty>& Vec, Func func,
		_Ty identity_element, Inverse_Funciton inverse_function)
		:
		_Func(func),
		_identity_element(identity_element),
		_Inverse_Func(inverse_function)
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
	//@ Parameter LIst:
	//@ start:	the start index
	//@ end:	the end index
	const _Ty query(int start, int end) const
	{

		if (start > end || start < 0 || end >= static_cast<int>(_Vec.size()))
			throw BIT_Exception("The querying range is invalid!!");

		return _Func(doQuery(end + 1), _Inverse_Func(doQuery(start)));

	}


	//modify the index as val
	//@ Parameter List:
	//@ index:	the position to be modified.
	//@ val:	the modifying value.
	void modify(int index, _Ty val)
	{

		if (index < 0 || index >= static_cast<int>(_Vec.size()))
			throw BIT_Exception("The modify index is invalid!!");

		doModify(index, val);

	}



	//modify pos with the function
	//@ Parameter List:
	//@ pos: the pos to be modified.
	//@ func: modify function.
	void modify(int index, modify_func func)
	{

		if (index < 0 || index >= static_cast<int>(_Vec.size()))
			throw BIT_Exception("The modify index is invalid!!");

		doModify(index, func);

	}


	BITImpl(const BITImpl&) = delete;

	BITImpl(BITImpl&&) = delete;

	BITImpl& operator=(const BITImpl&) = delete;

	BITImpl& operator=(BITImpl&&) = delete;


protected:

	_STD vector<_Ty> _Vec;

	//_Func 对类型 _Ty 构成Abel群
	Func _Func;

	//求逆运算
	Inverse_Funciton _Inverse_Func;

	//幺元
	_Ty _identity_element;


private:


	const int low_bit(int x) const noexcept
	{
		return x & (-x);
	}


	//S_(n-1)= A0+...+A_(n-1)
	//S_(n-1)=C_(n-1) + S_(n - 1 - low_bit(n));
	//@ Parameter LIst:
	//@ index:	the previous n elements
	const _Ty doQuery(int index) const
	{
		_Ty result = _identity_element;
		index--;
		while (index > 0)
		{
			result = _STD forward<_Ty>(_Func(result, _Vec[index]));
			index -= low_bit(index + 1);
		}
		return result;
	}


	//modify the pos as val
	//modify C_n,
	//then n += low_bit(n+1);
	//@ Parameter List:
	//@ index:	the position to be modified.
	//@ val:	the modifying value.
	void doModify(int index, _Ty val)
	{
		int size = _Vec.size();
		const _Ty _pos_val = _Func(doQuery(index + 1), _Inverse_Func(doQuery(index)));
		const _Ty diff = _Func(_Inverse_Func(_pos_val), val);
		auto func = [this, &diff](_Ty& other)
		{other = _STD forward<_Ty>(_Func(other, diff)); };
		while (index < size)
		{
			func(_Vec[index]);
			index += low_bit(index + 1);
		}
	}


	//modify pos with the function
	//@ Parameter List:
	//@ index:	the pos to be modified.
	//@ func:	modify function.
	void doModify(int index, modify_func func)
	{
		int size = _Vec.size();
		while (index < size)
		{
			func(_Vec[index]);
			index += low_bit(index + 1);
		}
	}


};
#endif // !_BITIMPL_H
