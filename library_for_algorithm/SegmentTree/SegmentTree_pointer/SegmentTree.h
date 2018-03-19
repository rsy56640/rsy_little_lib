#pragma once
#ifndef _SEGMENTTREE_H
#define _SEGMENTTREE_H
#include "SegmentTreeImpl.h"

//SegmentTree Template
template<class _Ty> class SegmentTree :public SegmentTreeType<_Ty>
{

	using PImpl = typename std::tr1::shared_ptr<SegmentTreeImpl<_Ty> >;

public:

	SegmentTree(const vector<_Ty>& Vec, Func func, _Ty Identity_Element)
		:_pImpl(make_shared<SegmentTreeImpl<_Ty> >(Vec, func, Identity_Element)) {}


	//查询操作，时间复杂度O(logn)
	_Ty query(int start, int end) const
	{
		return _pImpl->query(start, end);
	}


	//修改操作，时间复杂度O(logn)
	void modify(int index, _Ty&& value)
	{
		_pImpl->modify(index, _STD move(value));
	}


	void modify(int index, modify_func func)
	{
		_pImpl->modify(index, func);
	}


private:

	PImpl _pImpl;

};

#endif // !_SEGMENTTREE_H
