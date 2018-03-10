#pragma once
#ifndef _BINARY_INDEXED_TREE_H
#define _BINARY_INDEXED_TREE_H
#include <memory>
#include "BITImpl.h"

template<typename _Ty> class Binary_Indexed_Tree :public BIT_type<_Ty>
{

	using PImpl = typename std::tr1::shared_ptr<BITImpl<_Ty> >;

public:

	Binary_Indexed_Tree(const _STD vector<_Ty>& Vec, Func func,
		_Ty identity_element, Inverse_Funciton inverse_function)
		:_pImpl
		(_STD make_shared<BITImpl<_Ty> >(Vec, func, identity_element, inverse_function))
	{}


	const _Ty query(int start, int end) const
	{
		return _pImpl->query(start, end);
	}


	void modify(int index, _Ty val)
	{
		_pImpl->modify(index, val);
	}


	void modify(int index, modify_func func)
	{
		_pImpl->modify(index, func);
	}


private:

	PImpl _pImpl;

};
#endif // !_BINARY_INDEXED_TREE_H
