#pragma once
#include "rbtree_rsy.h"

struct rb_node
{
	unsigned long long  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	struct rb_node* rb_right;
	struct rb_node* rb_left;
	int value;
};

struct rb_tree
{
	struct rb_node* rb_NIL;
	struct rb_node* rb_root;
	unsigned int node_count;
};

static inline struct rb_node* rb_parent(struct rb_node* rb)
{
	return (struct rb_node *)((rb)->rb_parent_color & ~3);
}
static inline int rb_color(struct rb_node* rb)
{
	return ((rb)->rb_parent_color & 1);
}
static inline int rb_is_red(struct rb_node* rb)
{
	return !rb_color(rb);
}
static inline int rb_is_black(struct rb_node* rb)
{
	return rb_color(rb);
}
static inline void rb_set_red(struct rb_node* rb)
{
	(rb)->rb_parent_color &= ~1;
}
static inline void rb_set_black(struct rb_node* rb)
{
	(rb)->rb_parent_color |= 1;
}
static inline void rb_set_parent(struct rb_node* rb, struct rb_node* p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned long long)p;
}
static inline void rb_set_color(struct rb_node* rb, unsigned int color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

struct rb_node* left_most(struct rb_tree* rbt)
{
	struct rb_node* root = rbt->rb_root;
	while (root->rb_left != rbt->rb_NIL)
		root = root->rb_left;
	return root;
}

struct rb_node* right_most(struct rb_tree* rbt)
{
	struct rb_node* root = rbt->rb_root;
	while (root->rb_right != rbt->rb_NIL)
		root = root->rb_right;
	return root;
}

void left_rotate(struct rb_tree* rbt, struct rb_node* Pnode)
{
	//take Pnode's right child
	struct rb_node* right_child = Pnode->rb_right;
	//set right child's left child as Pnode's right child
	Pnode->rb_right = right_child->rb_left;
	//mutually
	if (right_child->rb_left != rbt->rb_NIL)
		rb_set_parent(right_child->rb_left, Pnode);
	//set right child's parent
	rb_set_parent(right_child, rb_parent(Pnode));
	//mutually
	if (rb_parent(Pnode) == rbt->rb_NIL)
		rbt->rb_root = right_child;
	else if (Pnode == rb_parent(Pnode)->rb_left)
		rb_parent(Pnode)->rb_left = right_child;
	else rb_parent(Pnode)->rb_right = right_child;
	//finally set the relation between them
	right_child->rb_left = Pnode;
	rb_set_parent(Pnode, right_child);
}//end function left_rotate(struct rb_tree*, struct rb_node*);

void right_rotate(struct rb_tree* rbt, struct rb_node* Pnode)
{
	//take Pnode's left child
	struct rb_node* left_child = Pnode->rb_left;
	//set left child's right child as Pnode's left child
	Pnode->rb_left = left_child->rb_right;
	//mutually
	if (left_child->rb_right != rbt->rb_NIL)
		rb_set_parent(left_child->rb_right, Pnode);
	//set left child's parent
	rb_set_parent(left_child, rb_parent(Pnode));
	//mutually
	if (rb_parent(Pnode) == rbt->rb_NIL)
		rbt->rb_root = left_child;
	else if (Pnode == rb_parent(Pnode)->rb_left)
		rb_parent(Pnode)->rb_left = left_child;
	else  rb_parent(Pnode)->rb_right = left_child;
	//finally set the relation between them
	left_child->rb_right = Pnode;
	rb_set_parent(Pnode, left_child);
}//end function right_rotate(struct rb_tree*, struct rb_node*);

struct rb_node* doFind(const struct rb_tree* rbt, const int val)
{
	struct rb_node* y = rbt->rb_NIL;
	struct rb_node* x = rbt->rb_root;
	while (x != rbt->rb_NIL)
	{
		y = x;
		if (val < x->value)
			x = x->rb_left;
		else if (x->value < val)
			x = x->rb_right;
		else
			return x;
	}
	return rbt->rb_NIL;
}

void RB_Insert_Fixup(struct rb_tree* rbt, struct rb_node* z)
{
	while (rb_is_red(rb_parent(z)))
	{
		//z.p is left child
		if (rb_parent(z) == rb_parent(rb_parent(z))->rb_left)
		{
			struct rb_node* y = rb_parent(rb_parent(z))->rb_right;
			//Case 1: z's uncle y is RED
			if (rb_is_red(y))
			{
				rb_set_black(rb_parent(z));
				rb_set_black(y);
				rb_set_red(rb_parent(rb_parent(z)));
				z = rb_parent(rb_parent(z));
			}//then continue while-loop

			else	//make 2 rotations and stop
			{
				//Case 2: z's uncle y is BLACK, and z is a right child.
				if (z == rb_parent(z)->rb_right)
				{
					z = rb_parent(z);
					left_rotate(rbt, z);
				}

				//Nota Bene: Case 2 leads to Case 3.
				//Case 3: z's uncle y is BLACK, and z is a left child.
				rb_set_black(rb_parent(z));
				rb_set_red(rb_parent(rb_parent(z)));
				right_rotate(rbt, rb_parent(rb_parent(z)));

			}//Case 2 -> Case 3 -> End of while-loop.

		}//then continue while-loop

		 //z.p is right child
		else
		{
			//y is z's uncle node
			struct rb_node* y = rb_parent(rb_parent(z))->rb_left;

			//Case 1: z's uncle y is RED
			if (rb_is_red(y))
			{
				rb_set_black(rb_parent(z));
				rb_set_black(y);
				rb_set_red(rb_parent(rb_parent(z)));
				z = rb_parent(rb_parent(z));
			}//then continue while-loop

			else	//make 2 rotations and stop
			{
				//Case 2: z's uncle y is BLACK, and z is a left child.
				if (z == rb_parent(z)->rb_left)
				{
					z = rb_parent(z);
					right_rotate(rbt, z);
				}

				//Nota Bene: Case 2 leads to Case 3.
				//Case 3: z's uncle y is BLACK, and z is a right child.
				rb_set_black(rb_parent(z));
				rb_set_red(rb_parent(rb_parent(z)));
				left_rotate(rbt, rb_parent(rb_parent(z)));

			}//Case 2 -> Case 3 -> End of while-loop.

		}//then continue while-loop

	}//end while

	//now z.p is BLACK
	//for the safety, set root color as black
	rb_set_black(rbt->rb_root);

}//end function RB_Insert_Fixup(struct rb_tree*, struct rb_node*);

void doRB_insert(struct rb_tree* rbt, struct rb_node* Pnode, struct rb_node* y, int left)
{
	//now y is the parent of Pnode
	rb_set_parent(Pnode, y);

	//judge whether the tree is empty
	if (y == rbt->rb_NIL)
		rbt->rb_root = Pnode;

	//not empty, then set the relation between y and Pnode
	else if (left)
		y->rb_left = Pnode;
	else y->rb_right = Pnode;

	//set Pnode appropriately, color has already been set RED.
	Pnode->rb_left = rbt->rb_NIL;
	Pnode->rb_right = rbt->rb_NIL;

	//fixup to maintain the RB_Tree property
	RB_Insert_Fixup(rbt, Pnode);

}//end function doRB_insert(struct rb_tree*, struct rb_node*, struct rb_node*, int);




