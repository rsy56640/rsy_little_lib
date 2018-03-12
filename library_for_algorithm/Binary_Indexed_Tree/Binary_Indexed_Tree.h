#pragma once
#ifndef _BINARY_INDEXED_TREE_H
#define _BINARY_INDEXED_TREE_H
#include <memory>
#include "BITImpl.h"

template<typename _Ty> class Binary_Indexed_Tree :public BIT_type<_Ty>
{

	using PImpl = typename std::tr1::shared_ptr<BITImpl<_Ty> >;

public:


	//支持连续区间查询，及修改，复杂度均为O(log_n)
	//@ Parameter List:
	//@		Vec: 要操作的数组
	//@		func: 对类型 _Ty 构成Abel群
	//@		identity_element: 幺元
	//@		inverse_function: 求逆运算
	Binary_Indexed_Tree(const _STD vector<_Ty>& Vec, const Func& func,
		const _Ty& identity_element, const Inverse_Funciton& inverse_function)
		:_pImpl
		(_STD make_shared<BITImpl<_Ty> >(Vec, func, identity_element, inverse_function))
	{}


	const _Ty query(const int start, const int end) const
	{
		return _pImpl->query(start, end);
	}


	void modify(const int index, _Ty&& val)
	{
		_pImpl->modify(index, _STD forward<_Ty>(val));
	}


	void modify(const int index, const modify_func& func)
	{
		_pImpl->modify(index, func);
	}


private:

	PImpl _pImpl;

};
#endif // !_BINARY_INDEXED_TREE_H
