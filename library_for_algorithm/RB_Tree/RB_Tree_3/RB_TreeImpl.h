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
			using RBTEx = typename RB_Tree_Exception;


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
			const RB_Comp _rb_Key_comp;


		protected:


			//initialize the Comparator with the customized comp function for _Ty type.
			void init()
			{
				NIL->color = _RB_Tree_black;
			}


			RBTreeNode_ptr minimum(RBTreeNode_ptr root)
			{
				while (root->left != NIL)
					root = root->left;
				return root;
			}


			RBTreeNode_ptr maximum(RBTreeNode_ptr root)
			{
				while (root->right != NIL)
					root = root->right;
				return root;
			}


			/***********************************\
			*            Left Rotate            *
			*  rotate Pnode and its right child *
			*     without any color changed     *
			\***********************************/
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
				else if (Pnode == Pnode->parent->left)Pnode->parent->left = right_child;
				else Pnode->parent->right = right_child;

				//finally set the relation between them
				right_child->left = Pnode;
				Pnode->parent = right_child;

			}


			/***********************************\
			*           Right Rotate           *
			*  rotate Pnode and its left child *
			*     without any color changed    *
			\***********************************/
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
				else if (Pnode == Pnode->parent->left)Pnode->parent->left = left_child;
				else Pnode->parent->right = left_child;

				//finally set the relation between them
				left_child->right = Pnode;
				Pnode->parent = left_child;

			}


			/*************************************\
			*  Insert a node with specific value  *
			\*************************************/
			//Pnode always has color RED.
			//@ Prameter list:
			//@		Pnode: the node to be inserted.
			//@		_arg : indicate whether or not to be substituted if key collides.
			void doRB_Insert(RBNode_ptr Pnode, RBNode_ptr y, bool left)
			{

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
				*           (static_cast<link_type>(Pnode.operator->()))->value_field;    *
				*       //= std::make_shared<RB_Tree_Node<_Ty> >(                         *
				*       //(static_cast<link_type>(Pnode.operator->())->value_field));     *
				*       //Proot->parent = NIL;                                            *
				*       //Pnode.reset(static_cast<base_type_ptr>(Proot.operator->()));    *
				*       Proot->parent = NIL;                                              *
				*       Proot->left = Pnode->left;                                        *
				*       Proot->right = Pnode->right;                                      *
				*       Pnode = std::static_pointer_cast<node_type>(Proot);               *
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
					if (z->parent == z->parent->parent->left)
					{

						//y is z's uncle node
						RBNode_ptr y = z->parent->parent->right;

						//Case 1: z's uncle y is RED
						if (y->color == _RB_Tree_red)
						{
							z->parent->color = _RB_Tree_black;
							y->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							z = z->parent->parent;
						}//then continue while-loop


						else	//make 2 rotations and stop
						{

							//Case 2: z's uncle y is BLACK, and z is a right child.
							if (z == z->parent->right)
							{
								z = z->parent;
								left_rotate(z);
							}

							//Nota Bene: Case 2 leads to Case 3.
							//Case 3: z's uncle y is BLACK, and z is a left child.
							z->parent->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							right_rotate(z->parent->parent);

						}//Case 2 -> Case 3 -> End of while-loop.

					}//then continue while-loop


					//z.p is right child
					else
					{

						//y is z's uncle node
						RBNode_ptr y = z->parent->parent->left;

						//Case 1: z's uncle y is RED
						if (y->color == _RB_Tree_red)
						{
							z->parent->color = _RB_Tree_black;
							y->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							z = z->parent->parent;
						}//then continue while-loop


						else	//make 2 rotations and stop
						{

							//Case 2: z's uncle y is BLACK, and z is a left child.
							if (z == z->parent->left)
							{
								z = z->parent;
								right_rotate(z);
							}

							//Nota Bene: Case 2 leads to Case 3.
							//Case 3: z's uncle y is BLACK, and z is a right child.
							z->parent->color = _RB_Tree_black;
							z->parent->parent->color = _RB_Tree_red;
							left_rotate(z->parent->parent);

						}//Case 2 -> Case 3 -> End of while-loop.

					}//then continue while-loop

				}//end while

				//now z.p is BLACK
				//for the safety, set root color as black
				Proot->color = _RB_Tree_black;

			}//end function RB_Insert_Fixup(RBNode_ptr)


			/****************************************************\
			*  replace Psrc with Pdest, in terms of Psrc-parent  *
			\****************************************************/
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


			/******************************\
			*    Delete a specific node    *
			\******************************/
			void doRB_Delete(RBNode_ptr Pnode)
			{

				//the count minus by 1
				node_count--;

				RBNode_ptr y = Pnode;
				RBNode_ptr x = nullptr;
				color_type y_original_color = y->color;


				//find the successor of Pnode

				if (Pnode->left == NIL)					//left child is NIL
				{
					x = Pnode->right;
					RB_Tranplant(Pnode, Pnode->right);
				}

				else if (Pnode->right == NIL)			//right child is NIL
				{
					x = Pnode->left;
					RB_Tranplant(Pnode, Pnode->left);
				}


				//left and right isn't either NIL,
				//find the successor of Pnode,
				//then substitute Pnode with its successor.
				else
				{

					//find successor
					y = minimum(Pnode->right);

					y_original_color = y->color;

					//to fill in the y's position
					x = y->right;

					//if Pnode's successor is its right child
					if (y->parent == Pnode)
						x->parent = y;

					else
					{

						//use x to substitute y
						RB_Tranplant(y, y->right);

						//steal Pnode's right subtree
						y->right = Pnode->right;
						y->right->parent = y;

					}

					//use y to substitute Pnode
					RB_Tranplant(Pnode, y);

					//steal Pnode's left subtree
					y->left = Pnode->left;
					y->left->parent = y;
					y->color = Pnode->color;

				}


				//the position has been set appropriately,
				//then fixup the color change


				//if original color is BLACK,
				//then push down to the x.
				//so x has 2 level BLACK,
				//fixup
				if (y_original_color == _RB_Tree_black)
					RB_Delete_Fixup(x);

			}



			/********************************************\
			* fixup "extra BLACK" to satisfy the RB_Tree *
			\********************************************/
			void RB_Delete_Fixup(RBNode_ptr Pnode)
			{

				//if Pnode is BLACK(BLACK) and non-root
				while (Pnode != Proot && Pnode->color == _RB_Tree_black)
				{

					//Pnode is left child
					if (Pnode == Pnode->parent->left)
					{

						//w is Pnode's brother
						RBNode_ptr w = Pnode->parent->right;


						//Case 1: brother w is RED
						if (w->color == _RB_Tree_red)
						{

							//swap w and parent color,
							//then left rotate
							w->color = _RB_Tree_black;
							Pnode->parent->color = _RB_Tree_red;
							left_rotate(Pnode->parent);

							//reset w as Pnode's brother
							w = Pnode->parent->right;

						}//Nota Bene: Case 1 leads to End
						//1->2->End, 1->4->End, 1->3->4->End.


						//Case 2: brother w is BLACK, and w's 2 childs are BLACK.
						//remove BLACK of both Pnode and w,
						//then compensate "a BLACK" to parent.
						//tips: no check for w's color
						//		because Case 1 check the color as RED
						//		and end up with color as BLACK.
						if (w->right->color == _RB_Tree_black
							&&
							w->left->color == _RB_Tree_black)
						{

							//remove w's BLACK
							w->color = _RB_Tree_red;

							//remove Pnode's BLACK, and compensate parent with "a BLACK".
							//because "a BLACK" is with Pnode, so set Pnode is its parent.
							Pnode = Pnode->parent;

						}//2->(1|2|3|4|END)


						//Case 3: brother w is RED,
						//		  w's left child is RED, w's right child is BLACK
						else
						{

							//Case 3
							//tips: w's left child is definitely RED
							//		because previous "if" indicates at least one child is RED.
							if (w->right->color == _RB_Tree_black)
							{

								//swap two node's color
								//and right rotate
								w->left->color = _RB_Tree_black;
								w->color = _RB_Tree_red;

								right_rotate(w);

								//reset w
								w = Pnode->parent->right;

							}


							//Case 4: brother w is RED, w's right child is RED
							//change 3 node's color and left rotate
							w->color = Pnode->parent->color;
							Pnode->parent->color = _RB_Tree_black;

							//set RED right child as BLACK
							w->right->color = _RB_Tree_black;

							//left rotate
							left_rotate(Pnode->parent);

							//a trick: root's color might be changed, so for safety.
							Pnode = Proot;

						}//3->4->END, 4->END

					}//end if(Pnode is left child)


					//Pnode is right child
					else
					{

						//w is Pnode's brother
						RBNode_ptr w = Pnode->parent->left;


						//Case 1: brother w is RED
						if (w->color == _RB_Tree_red)
						{

							//swap w and parent color,
							//then right rotate
							w->color = _RB_Tree_black;
							Pnode->parent->color = _RB_Tree_red;
							right_rotate(Pnode->parent);

							//reset w as Pnode's brother
							w = Pnode->parent->left;

						}//Nota Bene: Case 1 leads to End
						 //1->2->End, 1->4->End, 1->3->4->End.


						 //Case 2: brother w is BLACK, and w's 2 childs are BLACK.
						 //remove BLACK of both Pnode and w,
						 //then compensate "a BLACK" to parent.
						 //tips: no check for w's color
						 //		because Case 1 check the color as RED
						 //		and end up with color as BLACK.
						if (w->right->color == _RB_Tree_black
							&&
							w->left->color == _RB_Tree_black)
						{

							//remove w's BLACK
							w->color = _RB_Tree_red;

							//remove Pnode's BLACK, and compensate parent with "a BLACK".
							//because "a BLACK" is with Pnode, so set Pnode is its parent.
							Pnode = Pnode->parent;

						}//2->(1|2|3|4|END)


						 //Case 3: brother w is RED,
						 //		  w's right child is RED, w's left child is BLACK
						 //Case 4:
						else
						{

							//Case 3
							//tips: w's right child is definitely RED
							//		because previous "if" indicates at least one child is RED.
							if (w->left->color == _RB_Tree_black)
							{

								//swap two node's color
								//and left rotate
								w->right->color = _RB_Tree_black;
								w->color = _RB_Tree_red;

								left_rotate(w);

								//reset w
								w = Pnode->parent->left;

							}


							//Case 4: brother w is RED, w's left child is RED
							//change 3 node's color and right rotate
							w->color = Pnode->parent->color;
							Pnode->parent->color = _RB_Tree_black;

							//set RED left child as BLACK
							w->left->color = _RB_Tree_black;

							//right rotate
							right_rotate(Pnode->parent);

							//a trick: root's color might be changed, so for safety.
							Pnode = Proot;

						}//3->4->END, 4->END


					}


				}//end while-loop


				//quit loop, set Pnode(Proot) color as BLACK for safety.
				Pnode->color = _RB_Tree_black;

			}//end function RB_Delete_Fixup(RBNode_ptr)



		public:


			RB_TreeImpl(const Comp& comp)
				:node_count{ 0 },
				NIL{ std::make_shared<node_type>() },
				Proot{ NIL },
				_rb_Key_comp
			{
				[comp](const RBNode_ptr& lhs, const RBNode_ptr& rhs)->bool
			{
				return comp(lhs->value_field, rhs->value_field);
			}
			}
			{
				init();
			}


			//insert a node with specific value with a flag argument 
			void RB_Insert(const _Ty& value, INSERT_ARG _arg)
			{

				RBNode_ptr Pnode{ std::make_shared<RB_Tree_Node<_Ty> >(value) };

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

				doRB_Insert(Pnode, y, left);

			}


			//delete specific key node,
			//if no such node, do nothing.
			void RB_Delete(const _Ty& k_value)
			{

				//find whether there exists such node
				RBNode_ptr Pnode = std::make_shared<node_type>(k_value);


				//RB_Tree is empty
				if (node_count == 0)return;


				//find the node according to the property of BST
				RBNode_ptr x = Proot;

				while (x != NIL)
				{

					if (_rb_Key_comp(Pnode, x))			//Pnode.key < x.key
						x = x->left;

					else if (_rb_Key_comp(x, Pnode))	//x.key < Pnode.key
						x = x->right;

					else								//key collision
					{
						Pnode = x;
						doRB_Delete(Pnode);
						return;
					}

				}//end while

				throw RBTEx{ "There is no such key element." };

			}//end function RB_Delete(const _Ty&)


		};//end class RB_TreeImpl


	}//end namespace MY_RB_Tree


}//end namespace RSY_TOOL

#endif // !_RB_TREEIMPL_H
