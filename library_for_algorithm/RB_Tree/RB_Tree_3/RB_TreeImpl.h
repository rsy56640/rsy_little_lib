#pragma once
#ifndef _RB_TREEIMPL_H
#define _RB_TREEIMPL_H
#include "RB_Tree_Node.h"
#include "RB_Tree_Exception.h"
//#include "RB_Tree_Iterator.h"		//	[[deprecated]]
#include <functional>
#include <typeinfo>

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		namespace
		{
			struct INSERT_ARG { virtual ~INSERT_ARG() = default; };
			typedef struct :public INSERT_ARG {}  Assignment;	//if key collides, replace value
			typedef struct :public INSERT_ARG {}  NoAssignment;	//no replace
		}

#define _INSERT_ASSIGNMENT Assignment{}
#define _INSERT_NOASSIGNMENT NoAssignment{}



		//template class for RB_Tree
		template<class _Ty> class RB_TreeImpl
		{

			using size_type = int;
			using color_type = typename RB_Tree_Node<_Ty>::color_type;
			using node_type = typename RB_Tree_Node<_Ty>::node_type;
			using link_type = typename RB_Tree_Node<_Ty>::link_type;
			using RBTreeNode_ptr = typename RB_Tree_Node<_Ty>::RBTreeNode_ptr;
			using RBNode_ptr = RBTreeNode_ptr;
			using Comp = typename std::function<bool(const _Ty&, const _Ty&)>;
			using RB_Comp = typename std::function<bool(const RBNode_ptr&, const RBNode_ptr&)>;
			using Exception = typename RB_Tree_Exception;


		private:


			//size of the RB_Tree
			size_type node_count;


			//NIL node
			RBNode_ptr NIL;


			//RB_Tree root
			RBNode_ptr Proot;


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
				_rb_Key_comp = [&comp](const RBNode_ptr& lhs, const RBNode_ptr& rhs)->bool
				{
					return comp(lhs->value_field, rhs->value_field);
				};
			}


			//left rotate
			void left_rotate(RBNode_ptr Pnode)
			{

				//take Pnode's right child
				RBNode_ptr right_child = Pnode->right;

				//set right child's left child as Pnode's right child
				Pnode->right = right_child->left;

				//mutually
				if (right_child->left != NIL)right_child->left->parent = Pnode;

				//set right child's parent
				right_child->parent = Pnode->parent;

				//mutually
				if (Pnode->parent == NIL)
					Proot = right_child;
				else if (Pnode = Pnode->parent->left)Pnode->parent->left = right_child;
				else Pnode->parent->right = right_child;

				//finally set the relation between them
				right_child->left = Pnode;
				Pnode->parent = right_child;

			}


			//right rotate
			void right_rotate(RBNode_ptr Pnode)
			{

				//take Pnode's left child
				RBNode_ptr left_child = Pnode->left;

				//set left child's right child as Pnode's left child
				Pnode->left = left_child->right;

				//mutually
				if (left_child->right != NIL)left_child->right->parent = Pnode;

				//set left child's parent
				left_child->parent = Pnode->parent;

				//mutually
				if (Pnode->parent == NIL)
					Proot = left_child;
				else if (Pnode = Pnode->parent->left)Pnode->parent->left = left_child;
				else Pnode->parent->right = left_child;

				//finally set the relation between them
				left_child->right = Pnode;
				Pnode->parent = left_child;

			}


			//Pnode has color RED
			void doRB_Insert(RBNode_ptr Pnode, INSERT_ARG _arg)
			{

				node_count++;

				RBNode_ptr y = NIL;
				RBNode_ptr x = Proot;
				bool left = true;

				//find the position to insert,
				//according to the property of BST.
				while (x != NIL)
				{
					y = x;
					if (_rb_Key_comp(Pnode, x))			//Pnode.key < x.key
					{
						x = x->left;
						left = true;
					}
					else if (_rb_Key_comp(x, Pnode))	//x.key < Pnode.key
					{
						x = x->right;
						left = false;
					}
					else								//key collision
					{
						node_count--;

						//if the argument indicates the replacement.
						if (typeid(_arg) == typeid(Assignment))
						{
							x->value_field = Pnode->value_field;
						}
						return;

					}
				}

				//now y is the parent of Pnode
				Pnode->parent = y;


				//judge whether the tree is empty
				if (y == NIL)
					Proot = Pnode;

				/*************************************************************************\
				*                To commemorate my efforts on this issue.                 *
				*                             2018 - 04 - 07                              *
				*   if (y == NIL)                                                         *
				*   {                                                                     *
				*       Proot->value_field =                                              *
				* 		    (static_cast<link_type>(Pnode.operator->()))->value_field;    *
				* 	    //= std::make_shared<RB_Tree_Node<_Ty> >(                         *
				*	    //(static_cast<link_type>(Pnode.operator->())->value_field));     *
				*	    //Proot->parent = NIL;                                            *
				*	    //Pnode.reset(static_cast<base_type_ptr>(Proot.operator->()));    *
				*	    Proot->parent = NIL;                                              *
				*	    Proot->left = Pnode->left;                                        *
				*	    Proot->right = Pnode->right;                                      *
				*	    Pnode = std::static_pointer_cast<node_type>(Proot);               *
				*   }                                                                     *
				*                                                                         *
				\**************************************************************************/


				//not empty, then set the relation between y and Pnod
				else if (left)
					y->left = Pnode;
				else y->right = Pnode;


				//set Pnode appropriately, color has already been set RED.
				Pnode->left = NIL;
				Pnode->right = NIL;

				//fixup to maintain the RB_Tree property
				RB_Insert_Fixup(Pnode);

			}//end function doRB_Insert(RBNode_ptr, INSERT_ARG)


			/******************************\
			* fixup to satisfy the RB_Tree *
			\******************************/
			void RB_Insert_Fixup(RBNode_ptr z)
			{

				while (z->parent->color == _RB_Tree_red)
				{

					//z.p is left child
					if (z->parent = z->parent->parent->left)
					{

						//y is z's uncle node
						RBNode_ptr y = z->parent->parent->right;

						//case 1: z's uncle y is RED
						if (y->color == _RB_Tree_red)
						{
							z->parent->color = _RB_Tree_black;
							y->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							z = z->parent->parent;
						}//then continue while-loop


						else	//make 2 rotations and stop
						{

							//case 2: z's uncle y is BLACK, and z is a right child.
							if (z == z->parent->right)
							{
								z = z->parent;
								left_rotate(z);
							}

							//Nota Bene: case 2 leads to case 3.
							//case 3: z's uncle y is BLACK, and z is a left child.
							z->parent->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							right_rotate(z->parent->parent);

						}//case 2 -> case 3 -> End of while-loop.

					}//then continue while-loop


					//z.p is right child
					else
					{

						//y is z's uncle node
						RBNode_ptr y = z->parent->parent->left;

						//case 1: z's uncle y is RED
						if (y->color == _RB_Tree_red)
						{
							z->parent->color = _RB_Tree_black;
							y->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							z = z->parent->parent;
						}//then continue while-loop


						else	//make 2 rotations and stop
						{

							//case 2: z's uncle y is BLACK, and z is a left child.
							if (z == z->parent->left)
							{
								z = z->parent;
								right_rotate(z);
							}

							//Nota Bene: case 2 leads to case 3.
							//case 3: z's uncle y is BLACK, and z is a right child.
							z->parent->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							left_rotate(z->parent->parent);

						}//case 2 -> case 3 -> End of while-loop.

					}//then continue while-loop

				}//end while

				//now z.p is BLACK
				//for the safety, set root color as black
				Proot->color = _RB_Tree_black;

			}//end function RB_Insert_Fixup(RBNode_ptr)


			/**************************************************\
			* replace Psrc with Pdest, in terms of Psrc-parent *
			\**************************************************/
			void RB_Tranplant(RBNode_ptr Psrc, RBNode_ptr Pdest)
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


			//TODO
			void doRB_Delete(RBNode_ptr Pnode)
			{



			}



			//TODO
			void RB_Delete_Fixup(RBNode_ptr Pnode)
			{




			}



		public:


			RB_TreeImpl(const Comp& comp)
				:node_count{ 0 },
				NIL{ std::make_shared<node_type>() },
				Proot{ NIL }
			{
				init(comp);
			}


			//insert value with argument 
			void RB_Insert(const _Ty& value, INSERT_ARG _arg)
			{
				RBNode_ptr Pnode{ std::make_shared<RB_Tree_Node<_Ty> >(value) };
				doRB_Insert(Pnode, _arg);
			}


			//TODO
			void RB_Delete(const _Ty& value)
			{





			}


		};//end class RB_TreeImpl


	}//end namespace MY_RB_Tree


}//end namespace RSY_TOOL

#endif // !_RB_TREEIMPL_H
