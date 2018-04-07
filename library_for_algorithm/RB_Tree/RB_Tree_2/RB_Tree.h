#pragma once
#ifndef _RB_TREE_H
#define _RB_TREE_H
#include "RB_TreeImpl.h"
#include <memory>
#include <algorithm>
namespace RSY_TOOL
{


	namespace MY_RB_Tree
	{

		//template class for RB_Tree
		template<class _Ty> class RB_Tree
		{

		private:

			using link_type = typename RB_Tree_Node<_Ty>::link_type;

			using Comp = typename std::function<bool(const _Ty&, const _Ty&)>;

			using PImpl = std::shared_ptr<RB_TreeImpl<_Ty> >;

			PImpl _pImpl;

		public:

			RB_Tree(const Comp& comp = less<_Ty>{})
				:_pImpl(std::make_shared<RB_TreeImpl<_Ty> >(comp))
			{}



			void swap(RB_Tree& other)
			{
				std::swap(_pImpl, other._pImpl);
			}


			void RB_Insert(const _Ty& value, INSERT_ARG _arg = _INSERT_NOASSIGNMENT)
			{
				_pImpl->RB_Insert(value, _arg);
			}


		};//end class


		template<class _Ty>
		void swap(RB_Tree<_Ty>& lhs, RB_Tree<_Ty>& rhs)
		{
			lhs.swap(rhs);
		}


	}

}
#endif // !_RB_TREE_H
