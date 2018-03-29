#pragma once
#ifndef _SEGMENTTREENODE_H
#define _SEGMENTTREENODE_H
#include "SegmentTreeType.h"

namespace RSY_TOOL
{

	namespace MySegmentTree
	{

		//SegmentTreeNode Template
		template<class _Ty> class SegmentTreeNode :public SegmentTreeType<_Ty>
		{

		public:

			SegmentTreeNode(int start, int end, _Ty val)
				:_start(start), _end(end), _val(val) {}


			const int start() const noexcept
			{
				return _start;
			}


			const int end() const noexcept
			{
				return _end;
			}


			//value_type
			_Ty val() const noexcept
			{
				return _val;
			}


			//const reference_type
			const _Ty& value() const noexcept
			{
				return _val;
			}


			void setValue(_Ty&& value)
			{
				_val = _STD forward<_Ty>(value);
			}



			SegmentTreeNode(const SegmentTreeNode&) = delete;

			SegmentTreeNode& operator=(const SegmentTreeNode&) = delete;

			SegmentTreeNode(SegmentTreeNode&&) = delete;

			SegmentTreeNode& operator=(SegmentTreeNode&&) = delete;

			~SegmentTreeNode() = default;


		private:

			const int _start, _end;

			_Ty _val;

		};

	}

}

#endif // !_SEGMENTTREENODE_H
