#pragma once
#ifndef _RB_TREE_H
#define _RB_TREE_H
#include "RB_TreeImpl.h"
#include <memory>
namespace RSY_TOOL
{


	namespace MY_RB_Tree
	{

		//template class for RB_Tree
		template<class _Ty> class RB_Tree
		{

		private:

			using link_type = typename RB_Tree_Node<_Ty>::link_type;

			using PImpl = std::shared_ptr<RB_TreeImpl<_Ty> >;

			PImpl pImpl;

		public:

			RB_Tree() {}



			void swap(RB_Tree& other)
			{
				std::swap(pImpl, other.pImpl);
			}



		};


		template<class _Ty>
		void swap(RB_Tree<_Ty>& lhs, RB_Tree<_Ty>& rhs)
		{
			lhs.swap(rhs);
		}

	}

}
#endif // !_RB_TREE_H
