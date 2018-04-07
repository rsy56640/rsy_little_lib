#pragma once
#ifndef _RB_TREEIMPL_H
#define _RB_TREEIMPL_H
#include <functional>
#include "RB_Tree_Node.h"
//#include "RB_Tree_Iterator.h"		//	[[deprecated]]
#include "RB_Tree_Exception.h"

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		//template class for RB_Tree
		template<class _Ty> class RB_TreeImpl
		{

			using size_type = int;
			using color_type = typename RB_Tree_Node_Base::color_type;
			using base_ptr = typename RB_Tree_Node_Base::base_ptr;
			using link_type = typename RB_Tree_Node<_Ty>::link_type;
			using RBTreeNode_ptr = typename std::shared_ptr<RB_Tree_Node<_Ty> >;
			using RBNode_ptr = RBTreeNode_ptr;
			using Comp = typename std::function<bool(const _Ty&, const _Ty&)>;
			using RB_Comp = typename std::function<bool(const base_ptr&, const base_ptr&)>;
			using Exception = typename RB_Tree_Exception;


		private:

			size_type node_count;

			//RB_Tree root
			RBNode_ptr Proot;

			//NIL node
			base_ptr NIL;

			/******************************\
			*  Comparator for two node     *
			*    if _rb_comp(p1, p2) > 0   *
			*    it means that p1 < p2     *
			\******************************/
			RB_Comp _rb_Key_comp;


		protected:


			//initialize the Comparator with the customized comp function for _Ty type.
			void init(const Comp& comp)
			{
				NIL = std::make_shared<RB_Tree_Node_Base>();
				NIL->color = _RB_Tree_black;
				NIL->parent = NIL;
				NIL->left = nullptr;
				NIL->right = nullptr;
				Proot.reset(static_cast<link_type>(NIL.operator->()));
				Proot->parent.reset(static_cast<link_type>(NIL.operator->()));
				_rb_Key_comp = [&comp](const base_ptr& lhs, const base_ptr& rhs)->bool
				{
					return comp(static_cast<link_type>(lhs.operator->())->value_field,
						static_cast<link_type>(rhs.operator->())->value_field);
				};
			}


			//left rotate
			void left_rotate(base_ptr Pnode)
			{
				//take Pnode's right child
				base_ptr right_child = Pnode->right;

				//set right child's left child as Pnode's right child
				Pnode->right = right_child->left;

				//mutually
				if (right_child->left != NIL)right_child->left->parent = Pnode;

				//set right child's parent
				right_child->parent = Pnode->parent;

				//mutually
				if (Pnode->parent == NIL)Proot = right_child;
				else if (Pnode = Pnode->parent->left)Pnode->parent->left = right_child;
				else Pnode->parent->right = right_child;

				//finally set the relation between them
				right_child->left = Pnode;
				Pnode->parent = right_child;

			}


			//right rotate
			void right_rotate(base_ptr Pnode)
			{
				//take Pnode's left child
				base_ptr left_child = Pnode->left;

				//set left child's right child as Pnode's left child
				Pnode->left = left_child->right;

				//mutually
				if (left_child->right != NIL)left_child->right->parent = Pnode;

				//set left child's parent
				left_child->parent = Pnode->parent;

				//mutually
				if (Pnode->parent == NIL)Proot = left_child;
				else if (Pnode = Pnode->parent->left)Pnode->parent->left = left_child;
				else Pnode->parent->right = left_child;

				//finally set the relation between them
				left_child->right = Pnode;
				Pnode->parent = left_child;

			}


			//Pnode has color RED
			void doRB_Insert(base_ptr Pnode)
			{
				base_ptr y = NIL;
				base_ptr x = static_cast<base_ptr>(Proot.operator->());



			}


			/******************************\
			* fixup to satisfy the RB_Tree *
			\******************************/
			void RB_Insert_Fixup(base_ptr Pnode)
			{

			}


			/**************************************************\
			* replace Psrc with Pdest, in terms of Psrc-parent *
			\**************************************************/
			void RB_Tranplant(base_ptr Psrc, base_ptr Pdest)
			{

				//if Psrc is root
				if (Psrc->parent == NIL)
					Proot = Pdest;

				//if Psrc is left child
				else if (Psrc == Psrc->parent->left)
					Psrc->parent->left = Pdest;

				//else Psrc is right child
				else Psrc->parent->right = Pdest;

				//set Pdest's parent
				Pdest->parent = Psrc->parent;

			}


			//
			void doRB_Delete(base_ptr Pnode)
			{



			}


			//
			void RB_Delete_Fixup(base_ptr Pnode)
			{




			}



		public:


			RB_TreeImpl(const Comp& comp = less<_Ty>())
				:node_count(0), Proot(nullptr), NIL(nullptr)
			{
				init(comp);
			}


			//
			void RB_Insert(const _Ty& value)
			{
				RBNode_ptr Pnode(std::make_shared<RB_Tree_Node<_Ty> >(value));
				doRB_Insert(static_cast<base_ptr>(Pnode.operator->()));
			}


			//
			void RB_Delete(const _Ty& value)
			{





			}


		};//end class RB_TreeImpl


	}//end namespace MY_RB_Tree


}//end namespace RSY_TOOL

#endif // !_RB_TREEIMPL_H
