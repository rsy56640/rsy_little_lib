#pragma once
#ifndef _SEGMENTTREE_H
#define _SEGMENTTREE_H
#include "SegmentTreeImpl.h"


namespace RSY_TOOL
{

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


			_Ty query(int start, int end) const
			{
				return _pImpl->query(start, end);
			}


			//修改操作，时间复杂度O(logn)

			//单点修改
			void modify(int index, const _Ty& value)
			{
				_pImpl->modify(index, index, value);
			}


			void augment(int index, const _Ty& aug_value)
			{
				_pImpl->modify_augment(index, index, aug_value);
			}


			//NoCaucy
			void modify(int index, modify_func func, NoCauchy = {})
			{
				_pImpl->modify(index, index, func);
			}


			/**************************************\
			* Cauchy function not supported so far *
			\**************************************/
			//Cauchy
			/*
			void modify(int index, modify_func func, Cauchy)
			{
				_pImpl->modify(index, index, func, __CAUCHY_FUNC_);
			}
			*/



			//区间修改

			void modify(int start, int end, const _Ty& value)
			{
				_pImpl->modify(start, end, value);
			}


			void augment(int start, int end, const _Ty& aug_value)
			{
				_pImpl->modify_augment(start, end, aug_value);
			}


			//NoCauchy
			void modify(int start, int end, const modify_func& func, NoCauchy = {})
			{
				_pImpl->modify(start, end, func);
			}



			//Optimization for Cauchy Function
			//Cauchy
			/**************************************\
			* Cauchy function not supported so far *
			\**************************************/
			/*
			void modify(int start, int end, const modify_func& func, Cauchy)
			{
				_pImpl->modify(start, end, func, __CAUCHY_FUNC_);
			}
			*/



			SegmentTree(const SegmentTree&) = delete;

			SegmentTree& operator=(const SegmentTree&) = delete;

			SegmentTree(SegmentTree&&) = delete;

			SegmentTree& operator=(SegmentTree&&) = delete;

			~SegmentTree() = default;


		private:

			PImpl _pImpl;

		};

	}

}

#endif // !_SEGMENTTREE_H
