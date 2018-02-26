#pragma once
#ifndef _SEGMENTTREE_H
#include "SegmentTreeImpl.h"

//SegmentTree Template
template<class _Ty> class SegmentTree :public SegmentTreeType<_Ty>
{

	using PImpl = typename std::tr1::shared_ptr<SegmentTreeImpl<_Ty> >;

public:

	SegmentTree(const vector<_Ty>& Vec, Func func, _Ty Identity_Element)
		:_pImpl(make_shared<SegmentTreeImpl<_Ty> >(Vec, func, Identity_Element)) {}


	//��ѯ������ʱ�临�Ӷ�O(logn)
	_Ty query(int start, int end) const
	{
		return _pImpl->query(start, end);
	}


	//�޸Ĳ�����ʱ�临�Ӷ�O(logn)
	void modify(int index, _Ty&& value)
	{
		_pImpl->modify(index, _STD forward<_Ty>(value));
	}


	SegmentTree(const SegmentTree&) = delete;

	SegmentTree& operator=(const SegmentTree&) = delete;

	SegmentTree(SegmentTree&&) = delete;

	SegmentTree& operator=(SegmentTree&&) = delete;

	~SegmentTree() = default;


private:

	PImpl _pImpl;

};

#endif // !_SEGMENTTREE_H
