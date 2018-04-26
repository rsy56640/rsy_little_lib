#pragma once
#ifndef _RB_TREE_H
#define _RB_TREE_H
#include "RB_TreeImpl.h"

namespace RSY_TOOL
{


	namespace MY_RB_Tree
	{

		//template class for RB_Tree
		template<class _Ty> class RB_Tree
		{

		public:

			using node_type = typename RB_Tree_Node<_Ty>::node_type;

			using link_type = typename RB_Tree_Node<_Ty>::link_type;

			using iterator_type = typename RB_Tree_Iterator<_Ty>::iterator_type;

			using Comp = typename std::function<bool(const _Ty&, const _Ty&)>;

			using PImpl = std::shared_ptr<RB_TreeImpl<_Ty> >;

			PImpl _pImpl;

		public:

			RB_Tree(const Comp& comp)
				:_pImpl(std::make_shared<RB_TreeImpl<_Ty> >(comp))
			{}


			void swap(RB_Tree& other)
			{
				std::swap(_pImpl, other._pImpl);
			}


			_STD size_t size() const
			{
				return _pImpl->size();
			}


			iterator_type begin()
			{
				return static_cast<iterator_type>(_pImpl->begin());
			}


			iterator_type end()
			{
				return static_cast<iterator_type>(_pImpl->end());
			}


			iterator_type find(const _Ty& value)
			{
				return _pImpl->find(value);
			}


			void insert(const _Ty& value, INSERT_ARG _arg = _INSERT_NOASSIGNMENT)
			{
				_pImpl->RB_Insert(value, _arg);
			}


			void erase(const _Ty& value)
			{
				_pImpl->RB_Delete(value);
			}


			iterator_type minimum() const
			{
				return _pImpl->minimum();
			}


			iterator_type maximum() const
			{
				return _pImpl->maximum();
			}


		};//end class RB_Tree


		template<class _Ty>
		void swap(RB_Tree<_Ty>& lhs, RB_Tree<_Ty>& rhs)
		{
			lhs.swap(rhs);
		}


	}//end namespace MY_RB_Tree

}//end namespace RSY_TOOL


 /*
  * non-member swap function
  * specialization for namespace std.
 **/
namespace std
{
	using RSY_TOOL::MY_RB_Tree::RB_Tree;
	template<class _Ty>
	void swap(RB_Tree<_Ty>& lhs, RB_Tree<_Ty>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif // !_RB_TREE_H
