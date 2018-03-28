#pragma once
#ifndef _SEGMENTTREE_H
#define _SEGMENTTREE_H
#include "SegmentTreeImpl.h"


namespace MySegmentTree
{


	//SegmentTree Template
	template<class _Ty> class SegmentTree :public SegmentTreeType<_Ty>
	{

		using PImpl = typename std::tr1::shared_ptr<SegmentTreeImpl<_Ty> >;

	public:

		//由于lazy evaluation，要求运算对该类型构成 可交换幺半群
		SegmentTree(const vector<_Ty>& Vec, const Func& func, const _Ty& Identity_Element)
			:_pImpl(make_shared<SegmentTreeImpl<_Ty> >(Vec, func, Identity_Element)) {}


		//查询操作，时间复杂度O(logn)
		_Ty query(int start, int end)
		{
			return _pImpl->query(start, end);
		}


		//修改操作，时间复杂度O(logn)

		//单点修改
		void modify(int index, const _Ty& value)
		{
			_pImpl->modify(index, index, value);
		}


		void modify_augment(int index, const _Ty& aug_value)
		{
			_pImpl->modify_augment(index, index, aug_value);
		}


		//NoCommutive
		void modify(int index, modify_func func, NoCommutive = {})
		{
			_pImpl->modify(index, index, func);
		}


		//Commutive
		void modify(int index, modify_func func, Commutive)
		{
			_pImpl->modify(index, index, func, Commutive{});
		}



		//区间修改

		void modify(int start, int end, const _Ty& value)
		{
			_pImpl->modify(start, end, value);
		}


		void modify_augment(int start, int end, const _Ty& aug_value)
		{
			_pImpl->modify_augment(start, end, aug_value);
		}


		//NoCommutive
		void modify(int start, int end, const modify_func& func, NoCommutive = {})
		{
			_pImpl->modify(start, end, func);
		}


		//Optimization for Commutive Function
		//Commutive
		void modify(int start, int end, const modify_func& func, Commutive)
		{
			_pImpl->modify(start, end, func, Commutive{});
		}


		SegmentTree(const SegmentTree&) = delete;

		SegmentTree& operator=(const SegmentTree&) = delete;

		SegmentTree(SegmentTree&&) = delete;

		SegmentTree& operator=(SegmentTree&&) = delete;

		~SegmentTree() = default;


	private:

		PImpl _pImpl;

	};

}

#endif // !_SEGMENTTREE_H
