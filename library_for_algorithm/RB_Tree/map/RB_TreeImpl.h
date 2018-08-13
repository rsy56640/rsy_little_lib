/*****************************************************\
*     To implement the iterator::decrement();         *
*     I make a little update, that is:                *
*       Let T.NIL->parent = Proot;                    *
*       in the functions:                             *
*         init();left_rotate();right_rotate;          *
*         doRB_Insert();RB_Transplant();              *
*                                        2018-04-25   *
*     To implement the map::insert_assign();          *
*     function RB_Insert() has been changed:          *
*       it is assumed that the _Ty is  std::pair;     *
*                                        2018-04-26   *
\*****************************************************/

#pragma once
#ifndef _RB_TREEIMPL_H
#define _RB_TREEIMPL_H
#include "RB_Tree_Exception.h"
#include "RB_Tree_Node.h"
#include <functional>
#include <typeinfo>

namespace RSY_TOOL
{

	namespace MY_RB_Tree
	{

		namespace
		{
			struct INSERT_ARGUMENT { virtual ~INSERT_ARGUMENT() = 0 {}; };
			typedef struct :public INSERT_ARGUMENT {}  Assignment;     //if key collides, replace value
			typedef struct :public INSERT_ARGUMENT {}  NoAssignment;   //no replace
			static const Assignment _Assignment;
			static const NoAssignment _NoAssignment;
		}
		using INSERT_ARG = const INSERT_ARGUMENT&;

#define _INSERT_ASSIGNMENT  _Assignment
#define _INSERT_NOASSIGNMENT _NoAssignment

		namespace
		{
			template <typename T, template <typename...> class Template>
			struct is_specialization_of : std::false_type {};

			template <template <typename...> class Template, typename... Args>
			struct is_specialization_of<Template<Args...>, Template>
				: std::true_type {};

			template<typename T> struct is_pair : is_specialization_of
				<typename std::decay<T>::type, std::pair> {};


			template<class Key, class Value>
			Value& insert_assign(_STD pair<const Key, Value>& data)
			{
				return data.second;
			}

		}


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

				//2018-04-25
				NIL->parent = Proot;//2018-04-25 to implement iterator decrement.
				//2018-04-25

			}


			RBTreeNode_ptr left_most(RBTreeNode_ptr root) const
			{
				if (isNIL(root))
					return NIL;
				while (!isNIL(root->left))
					root = root->left;
				return root;
			}


			RBTreeNode_ptr right_most(RBTreeNode_ptr root) const
			{
				while (!isNIL(root->right))
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
				if (!isNIL(right_child->left))right_child->left->parent = Pnode;

				//set right child's parent
				right_child->parent = Pnode->parent;

				//mutually
				if (isNIL(Pnode->parent))
				{
					Proot = right_child;
					NIL->parent = Proot;//2018-04-25 to implement iterator decrement.
				}
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
				if (!isNIL(left_child->right))left_child->right->parent = Pnode;

				//set left child's parent
				left_child->parent = Pnode->parent;

				//mutually
				if (isNIL(Pnode->parent))
				{
					Proot = left_child;
					NIL->parent = Proot;//2018-04-25 to implement iterator decrement.
				}
				else if (Pnode == Pnode->parent->left)Pnode->parent->left = left_child;
				else Pnode->parent->right = left_child;

				//finally set the relation between them
				left_child->right = Pnode;
				Pnode->parent = left_child;

			}


			/******************************************\
			*                  doFind                  *
			* help find the node with the specific key *
			\******************************************/
			RBTreeNode_ptr doFind(const _Ty& value) const
			{

				const RBNode_ptr Pnode{ std::make_shared<RB_Tree_Node<_Ty> >(value) };

				RBNode_ptr y = NIL;
				RBNode_ptr x = Proot;

				//find the position to insert,
				//according to the property of BST.
				while (!isNIL(x))
				{

					y = x;
					if (_rb_Key_comp(Pnode, x))         //Pnode.key < x.key
						x = x->left;

					else if (_rb_Key_comp(x, Pnode))    //x.key < Pnode.key
						x = x->right;

					else                                //key collision
						return x;

				}

				throw RBTEx("no such element.", RBTEx::iterator_ex);

			}//end function doFind(const _Ty&);


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
				if (isNIL(y))
				{
					Proot = Pnode;
					NIL->parent = Proot;//2018-04-25 to implement iterator decrement.
				}


				//not empty, then set the relation between y and Pnode
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
			void RB_Transplant(RBNode_ptr Psrc, RBNode_ptr Pdest)
			{

				//if Psrc is root
				if (isNIL(Psrc->parent))
				{
					Proot = Pdest;
					NIL->parent = Proot;//2018-04-25 to implement iterator decrement.
				}

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

				if (isNIL(Pnode->left))					//left child is NIL
				{
					x = Pnode->right;
					RB_Transplant(Pnode, Pnode->right);
				}

				else if (isNIL(Pnode->right))			//right child is NIL
				{
					x = Pnode->left;
					RB_Transplant(Pnode, Pnode->left);
				}


				//left and right isn't either NIL,
				//find the successor of Pnode,
				//then substitute Pnode with its successor.
				else
				{

					//find successor
					y = left_most(Pnode->right);

					y_original_color = y->color;

					//to fill in the y's position
					x = y->right;

					//if Pnode's successor is its right child
					if (y->parent == Pnode)
						x->parent = y;

					else
					{

						//use x to substitute y
						RB_Transplant(y, y->right);

						//steal Pnode's right subtree
						y->right = Pnode->right;
						y->right->parent = y;

					}

					//use y to substitute Pnode
					RB_Transplant(Pnode, y);

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


			RBTreeNode_ptr minimum() const
			{
				RBTreeNode_ptr root(Proot);
				while (!isNIL(root->left))
					root = root->left;
				return root;
			}

			RBTreeNode_ptr maximum() const
			{
				RBTreeNode_ptr root(Proot);
				while (!isNIL(root->right))
					root = root->right;
				return root;
			}

			_STD size_t size() const noexcept
			{
				return node_count;
			}

			const RBTreeNode_ptr begin() const
			{
				return minimum();
			}

			const RBTreeNode_ptr end() const
			{
				return NIL;
			}


			/*
			 * find the node with the specific key
			**/
			RBTreeNode_ptr find(const _Ty& value) const
			{
				try {
					return doFind(value);
				}
				catch (const RBTEx& e)
				{
					return NIL;
				}
			}


			/*
			 * returns a node pointing to the first element that is not less than key.
			**/
			RBTreeNode_ptr lower_bound(const _Ty& value) const
			{

				RBNode_ptr Pnode{ std::make_shared<RB_Tree_Node<_Ty> >(value) };

				RBNode_ptr y = NIL;
				RBNode_ptr x = Proot;

				while (!isNIL(x))
				{

					if (_rb_Key_comp(x, Pnode))          //x.key < Pnode.key
						x = x->right;

					else if (_rb_Key_comp(Pnode, x))     //x.key > Pnode.key
					{
						y = x;
						x = x->left;
					}

					else return x;                       //key collision

				}

				return y;

			}


			/*
			 * returns a node pointing to the first element that is greater than key.
			**/
			RBTreeNode_ptr upper_bound(const _Ty& value) const
			{

				RBNode_ptr Pnode{ std::make_shared<RB_Tree_Node<_Ty> >(value) };

				RBNode_ptr y = NIL;
				RBNode_ptr x = Proot;

				while (!isNIL(x))
				{

					if (!_rb_Key_comp(Pnode, x))         //x.key <= Pnode.key
						x = x->right;

					else if (_rb_Key_comp(Pnode, x))     //x.key > Pnode.key
					{
						y = x;
						x = x->left;
					}

				}

				return y;

			}


			/*
			 * insert a node with specific value with a flag argument
			**/
			void RB_Insert(const _Ty& value, INSERT_ARG _arg)
			{

				static_assert(is_pair<_Ty>::value, "std::pair needed.");

				RBNode_ptr Pnode{ std::make_shared<RB_Tree_Node<_Ty> >(value) };

				node_count++;

				RBNode_ptr y = NIL;
				RBNode_ptr x = Proot;
				bool left = true;


				//find the position to insert,
				//according to the property of BST.
				while (!isNIL(x))
				{

					y = x;
					if (_rb_Key_comp(Pnode, x))         //Pnode.key < x.key
					{
						x = x->left;
						left = true;
					}
					else if (_rb_Key_comp(x, Pnode))    //x.key < Pnode.key
					{
						x = x->right;
						left = false;
					}
					else                                //key collision
					{
						node_count--;

						//if the argument indicates the replacement.
						if (typeid(_arg) == typeid(Assignment))
						{
							//2018-04-26
							//Since the std::pair::operator= is so fuck,
							//this statement should be modifyed.
							insert_assign(x->value_field) = _STD forward<typename _Ty::second_type>
								(insert_assign(Pnode->value_field));
						}
						return;
					}
				}

				doRB_Insert(Pnode, y, left);

			}//end function RB_Insert(const _Ty&, INSERT_ARG);


			/*
			 * delete specific key node,
			 * if no such node, do nothing.
			**/
			void RB_Delete(const _Ty& k_value)
			{

				//find whether there exists such node
				RBNode_ptr Pnode = std::make_shared<node_type>(k_value);


				//RB_Tree is empty
				if (node_count == 0)return;


				//find the node according to the property of BST
				RBNode_ptr x = Proot;

				while (!isNIL(x))
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

				throw RBTEx{ "There is no such key element.", RBTEx::erase_ex };

			}//end function RB_Delete(const _Ty&)


		};//end class RB_TreeImpl


	}//end namespace MY_RB_Tree


}//end namespace RSY_TOOL

#endif // !_RB_TREEIMPL_H
