#pragma once
#ifndef _SEGMENTTREETYPE_H
#define _SEGMENTTREETYPE_H
#ifndef _STD
#define _STD ::std::
#endif // !_STD
#include <functional>
#include <memory>
#include <type_traits>


namespace MySegmentTree
{

	template<class _Ty>
	class SegmentTreeNode;

	template<class _Ty>
	struct SegmentTreeType
	{

		using value_type = typename std::remove_reference<_Ty>::type;

		using reference_type = typename value_type&;

		using SegmentTreeNode_ptr = typename std::tr1::shared_ptr<SegmentTreeNode<_Ty> >;

		using Func = typename std::tr1::function<_Ty(const _Ty&, const _Ty&)>;

		using modify_func = typename std::tr1::function<_Ty(const _Ty&)>;

	};

}


#endif // !_SEGMENTTREETYPE_H
