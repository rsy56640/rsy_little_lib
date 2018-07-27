#include "rbtree_rsy.h"
#ifndef true
#define true 1
#endif // !true
#ifndef false
#define false 0
#endif // !false

struct rb_node
{
	unsigned long long  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	struct rb_node* rb_right;
	struct rb_node* rb_left;
	struct Pair pair;
};

struct rb_tree
{
	struct rb_node* rb_NIL;
	struct rb_node* rb_root;
	unsigned int node_count;
};

struct rb_node_it
{
	struct rb_node* node;
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
	(rb)->rb_parent_color |= (unsigned long long) 1;
}
static inline void rb_set_parent(struct rb_node* rb, struct rb_node* p)
{
	rb->rb_parent_color = (rb->rb_parent_color & (unsigned long long)3) | (unsigned long long)p;
}
static inline void rb_set_color(struct rb_node* rb, unsigned int color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}
static inline void rb_set_color_with_node(struct rb_node* rb, const struct rb_node* target)
{
	rb_set_color(rb, rb_color(target));
}

struct rb_node* create_node(struct Pair pair)
{
	struct rb_node* node = malloc(sizeof(struct rb_node));
	rb_set_color(node, RB_RED);
	rb_set_parent(node, NULL);
	node->rb_left = NULL;
	node->rb_right = NULL;
	node->pair = pair;
	return node;
}

void free_node(struct rb_node* node)
{
	free_Pair(&node->pair);
	free(node);
}

void free_Pair(struct Pair* pair)
{
	free_K(pair->key);
	free_V(pair->value);
}

void init(struct rb_tree* rbt)
{
	rbt->node_count = 0;
	struct Pair pair = { NULL, NULL };
	rbt->rb_NIL = create_node(pair);
	rbt->rb_root = rbt->rb_NIL;
	rb_set_black(rbt->rb_NIL);
	rb_set_parent(rbt->rb_NIL, rbt->rb_root); //to implement iterator decrement.
}

int isNIL(const struct rb_node* node)
{
	return node->rb_left == NULL;
}

struct rb_node* left_most(struct rb_tree* rbt)
{
	if (rbt->node_count == 0)return rbt->rb_NIL;
	struct rb_node* node = rbt->rb_root;
	while (!isNIL(node->rb_left))
		node = node->rb_left;
	return node;
}

struct rb_node* right_most(struct rb_tree* rbt)
{
	if (rbt->node_count == 0)return rbt->rb_NIL;
	struct rb_node* node = rbt->rb_root;
	while (!isNIL(node->rb_right))
		node = node->rb_right;
	return node;
}

void left_rotate(struct rb_tree* rbt, struct rb_node* Pnode)
{
	//take Pnode's right child
	struct rb_node* right_child = Pnode->rb_right;
	//set right child's left child as Pnode's right child
	Pnode->rb_right = right_child->rb_left;
	//mutually
	if (!isNIL(right_child->rb_left))
		rb_set_parent(right_child->rb_left, Pnode);
	//set right child's parent
	rb_set_parent(right_child, rb_parent(Pnode));
	//mutually
	if (isNIL(rb_parent(Pnode)))
	{
		rbt->rb_root = right_child;
		rb_set_parent(rbt->rb_NIL, rbt->rb_root); //to implement iterator decrement.
	}
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
	if (!isNIL(left_child->rb_right))
		rb_set_parent(left_child->rb_right, Pnode);
	//set left child's parent
	rb_set_parent(left_child, rb_parent(Pnode));
	//mutually
	if (isNIL(rb_parent(Pnode)))
	{
		rbt->rb_root = left_child;
		rb_set_parent(rbt->rb_NIL, rbt->rb_root); //to implement iterator decrement.
	}
	else if (Pnode == rb_parent(Pnode)->rb_left)
		rb_parent(Pnode)->rb_left = left_child;
	else  rb_parent(Pnode)->rb_right = left_child;
	//finally set the relation between them
	left_child->rb_right = Pnode;
	rb_set_parent(Pnode, left_child);
}//end function right_rotate(struct rb_tree*, struct rb_node*);

/*
 * return the node with value which equals val,
 * return rbt->rb_NIL if no such node.
**/
struct rb_node* doFind(const struct rb_tree* rbt, const struct K* key)
{
	struct rb_node* y = rbt->rb_NIL;
	struct rb_node* x = rbt->rb_root;
	while (!isNIL(x))
	{
		y = x;
		if (rb_key_compare(key, x->pair.key))
			x = x->rb_left;
		else if (rb_key_compare(x->pair.key, key))
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
	if (isNIL(y))
	{
		rbt->rb_root = Pnode;
		rb_set_parent(rbt->rb_NIL, rbt->rb_root); //to implement iterator decrement.
	}

	//not empty, then set the relation between y and Pnode
	else if (left)
		y->rb_left = Pnode;
	else y->rb_right = Pnode;

	//set Pnode appropriately, color has already been set RED.
	Pnode->rb_left = rbt->rb_NIL;
	Pnode->rb_right = rbt->rb_NIL;

	//fixup to maintain the RB_Pairree property
	RB_Insert_Fixup(rbt, Pnode);

}//end function doRB_insert(struct rb_tree*, struct rb_node*, struct rb_node*, int);

/*
 *replace Psrc with Pdest, in terms of Psrc-parent
**/
void RB_Transplant(struct rb_tree* rbt, struct rb_node* Psrc, struct rb_node* Pdest)
{

	//if Psrc is root
	if (isNIL(rb_parent(Psrc)))
	{
		rbt->rb_root = Pdest;
		rb_set_parent(rbt->rb_NIL, rbt->rb_root); //to implement iterator decrement.
	}

	//if Psrc is left child
	else if (Psrc == rb_parent(Psrc)->rb_left)
		rb_parent(Psrc)->rb_left = Pdest;

	//else Psrc is right child
	else rb_parent(Psrc)->rb_right = Pdest;

	//set Pdest's parent
	rb_set_parent(Pdest, rb_parent(Psrc));

}

void doRB_Delete(struct rb_tree* rbt, struct rb_node* Pnode)
{
	//the count minus by 1
	rbt->node_count--;

	struct rb_node* y = Pnode;
	struct rb_node* x = NULL;
	int y_original_color = rb_color(y);

	//find the successor of Pnode
	if (isNIL(Pnode->rb_left))					//left child is NIL
	{
		x = Pnode->rb_right;
		RB_Transplant(rbt, Pnode, Pnode->rb_right);
	}
	else if (isNIL(Pnode->rb_right))			//right child is NIL
	{
		x = Pnode->rb_left;
		RB_Transplant(rbt, Pnode, Pnode->rb_left);
	}

	//left and right isn't either NIL,
	//find the successor of Pnode,
	//then substitute Pnode with its successor.
	else
	{
		//find successor
		y = left_most(Pnode->rb_right);
		y_original_color = rb_color(y);
		//to fill in the y's position
		x = y->rb_right;
		//if Pnode's successor is its right child
		if (rb_parent(y) == Pnode)
			rb_set_parent(x, y);
		else
		{
			//use x to substitute y
			RB_Transplant(rbt, y, y->rb_right);
			//steal Pnode's right subtree
			y->rb_right = Pnode->rb_right;
			rb_set_parent(y->rb_right, y);
		}
		//use y to substitute Pnode
		RB_Transplant(rbt, Pnode, y);
		//steal Pnode's left subtree
		y->rb_left = Pnode->rb_left;
		rb_set_parent(y->rb_left, y);
		rb_set_color_with_node(y, Pnode);
	}

	free_node(Pnode);

	//the position has been set appropriately,
	//then fixup the color change

	//if original color is BLACK,
	//then push down to the x.
	//so x has 2 level BLACK,
	//fixup
	if (y_original_color == RB_BLACK)
		RB_Delete_Fixup(rbt, x);
}//end doRB_Delete(struct rb_tree*, struct rb_node*);

void RB_Delete_Fixup(struct rb_tree* rbt, struct rb_node* Pnode)
{
	//if Pnode is BLACK(BLACK) and non-root
	while (Pnode != rbt->rb_root && rb_color(Pnode) == RB_BLACK)
	{
		//Pnode is left child
		if (Pnode == rb_parent(Pnode)->rb_left)
		{
			//w is Pnode's brother
			struct rb_node* w = rb_parent(Pnode)->rb_right;

			//Case 1: brother w is RED
			if (rb_color(w) == RB_RED)
			{
				//swap w and parent color,
				//then left rotate
				rb_set_black(w);
				rb_set_red(rb_parent(Pnode));
				left_rotate(rbt, rb_parent(Pnode));
				//reset w as Pnode's brother
				w = rb_parent(Pnode)->rb_right;
			}//Nota Bene: Case 1 leads to End
			 //1->2->End, 1->4->End, 1->3->4->End.

			 //Case 2: brother w is BLACK, and w's 2 childs are BLACK.
			 //remove BLACK of both Pnode and w,
			 //then compensate "a BLACK" to parent.
			 //tips: no check for w's color
			 //		because Case 1 check the color as RED
			 //		and end up with color as BLACK.
			if (rb_color(w->rb_right) == RB_BLACK
				&&
				rb_color(w->rb_left) == RB_BLACK)
			{
				//remove w's BLACK
				rb_set_red(w);
				//remove Pnode's BLACK, and compensate parent with "a BLACK".
				//because "a BLACK" is with Pnode, so set Pnode is its parent.
				Pnode = rb_parent(Pnode);
			}//2->(1|2|3|4|END)

			 //Case 3: brother w is RED,
			 //		  w's left child is RED, w's right child is BLACK
			else
			{
				//Case 3
				//tips: w's left child is definitely RED
				//		because previous "if" indicates at least one child is RED.
				if (rb_color(w->rb_right) == RB_BLACK)
				{
					//swap two node's color
					//and right rotate
					rb_set_black(w->rb_left);
					rb_set_red(w);
					right_rotate(rbt, w);
					//reset w
					w = rb_parent(Pnode)->rb_right;
				}

				//Case 4: brother w is RED, w's right child is RED
				//change 3 node's color and left rotate
				rb_set_color_with_node(w, rb_parent(Pnode));
				rb_set_black(rb_parent(Pnode));
				//set RED right child as BLACK
				rb_set_black(w->rb_right);
				//left rotate
				left_rotate(rbt, rb_parent(Pnode));
				//a trick: root's color might be changed, so for safety.
				Pnode = rbt->rb_root;
			}//3->4->END, 4->END
		}//end if(Pnode is left child)

		 //Pnode is right child
		else
		{
			//w is Pnode's brother
			struct rb_node* w = rb_parent(Pnode)->rb_left;

			//Case 1: brother w is RED
			if (rb_color(w) == RB_RED)
			{
				//swap w and parent color,
				//then right rotate
				rb_set_black(w);
				rb_set_red(rb_parent(Pnode));
				right_rotate(rbt, rb_parent(Pnode));
				//reset w as Pnode's brother
				w = rb_parent(Pnode)->rb_left;
			}//Nota Bene: Case 1 leads to End
			 //1->2->End, 1->4->End, 1->3->4->End.

			 //Case 2: brother w is BLACK, and w's 2 childs are BLACK.
			 //remove BLACK of both Pnode and w,
			 //then compensate "a BLACK" to parent.
			 //tips: no check for w's color
			 //		because Case 1 check the color as RED
			 //		and end up with color as BLACK.
			if (rb_color(w->rb_right) == RB_BLACK
				&&
				rb_color(w->rb_left) == RB_BLACK)
			{
				//remove w's BLACK
				rb_set_red(w);
				//remove Pnode's BLACK, and compensate parent with "a BLACK".
				//because "a BLACK" is with Pnode, so set Pnode is its parent.
				Pnode = rb_parent(Pnode);
			}//2->(1|2|3|4|END)

			 //Case 3: brother w is RED,
			 //		  w's right child is RED, w's left child is BLACK
			 //Case 4:
			else
			{
				//Case 3
				//tips: w's right child is definitely RED
				//		because previous "if" indicates at least one child is RED.
				if (rb_color(w->rb_left) == RB_BLACK)
				{
					//swap two node's color
					//and left rotate
					rb_set_black(w->rb_right);
					rb_set_red(w);
					left_rotate(rbt, w);
					//reset w
					w = rb_parent(Pnode)->rb_left;
				}
				//Case 4: brother w is RED, w's left child is RED
				//change 3 node's color and right rotate
				rb_set_color_with_node(w, rb_parent(Pnode));
				rb_set_black(rb_parent(Pnode));
				//set RED left child as BLACK
				rb_set_black(w->rb_left);
				//right rotate
				right_rotate(rbt, rb_parent(Pnode));
				//a trick: root's color might be changed, so for safety.
				Pnode = rbt->rb_root;
			}//3->4->END, 4->END
		}//end if Pnode is right child
	}//end while-loop
	 //quit loop, set Pnode(Proot) color as BLACK for safety.
	rb_set_black(Pnode);
}//end function RB_Delete_Fixup(RBNode_ptr)

int RB_Insert(struct rb_tree* rbt, const struct Pair pair, int _mode_t)
{
#define insert 0x01234567
#define insert_or_assign 0x12345678

	struct rb_node* Pnode = create_node(pair);
	rbt->node_count++;

	struct rb_node* y = rbt->rb_NIL;
	struct rb_node* x = rbt->rb_root;
	int left = true;

	//find the position to insert,
	//according to the property of BSPair.
	while (!isNIL(x))
	{
		y = x;
		if (rb_key_compare(Pnode->pair.key, x->pair.key))         //Pnode.key < x.key
		{
			x = x->rb_left;
			left = true;
		}
		else if (rb_key_compare(x->pair.key, Pnode->pair.key))    //x.key < Pnode.key
		{
			x = x->rb_right;
			left = false;
		}
		else                                //key collision
		{
			rbt->node_count--;
			//if the argument indicates the replacement.
			if (_mode_t == insert_or_assign)
			{
				//2018-04-26
				//Since the std::pair::operator= is so fuck,
				//this statement should be modifyed.

				//free_V operation has been set in "assign_V".
				assign_V(&x->pair.value, &Pnode->pair.value);
				free_K(Pnode->pair.key);
				free(Pnode);

				return 2;
			}
			else
			{
				free_node(Pnode);
				return 0;
			}
		}
	}
	doRB_insert(rbt, Pnode, y, left);
	return 1;
}//end RB_Insert(struct rb_tree*, const struct Pair, int);

//for iterator
struct rb_node* minimum(const struct rb_node* root)
{
	struct rb_node* node = root;
	while (!isNIL(node->rb_left))
		node = node->rb_left;
	return node;
}

struct rb_node* maximum(const struct rb_node* root)
{
	struct rb_node* node = root;
	while (!isNIL(node->rb_right))
		node = node->rb_right;
	return node;
}


/********************    Implementation for header file    ********************/

struct rb_tree* rb_get()
{
	struct rb_tree* rbt = malloc(sizeof(struct rb_tree));
	init(rbt);
	return rbt;
}

struct Pair rb_get_min(const struct rb_tree* rbt)
{
	return left_most(rbt)->pair;
}

struct Pair rb_get_max(const struct rb_tree* rbt)
{
	return right_most(rbt)->pair;
}

int rb_get_size(const struct rb_tree* rbt)
{
	return rbt->node_count;
}

struct rb_node_it* rb_begin(const struct rb_tree* rbt)
{
	struct rb_node_it* it = malloc(sizeof(struct rb_node_it));
	it->node = left_most(rbt);
	return it;
}

struct rb_node_it* rb_end(const struct rb_tree* rbt)
{
	struct rb_node_it* it = malloc(sizeof(struct rb_node_it));
	it->node = rbt->rb_NIL;
	return it;
}

int rb_it_increment(const struct rb_tree* rbt, struct rb_node_it* node_it)
{
	struct rb_node** node = &(node_it->node);
	//if node is Pair.NIL
	//NB: it is allowed that iterator is end(), which is Pair.NIL.
	if (isNIL(*node))
		return 0;
	//right child exits
	if (!isNIL((*node)->rb_right))
	{
		*node = (*node)->rb_right;
		*node = minimum(*node);
	}
	//no right child
	else {
		//NB: the case is considered when node is root, and temp is Pair.NIL.
		struct rb_node* temp = rb_parent(*node);
		//trace back until node is not a right child
		while (*node == temp->rb_right)
		{
			*node = temp;
			temp = rb_parent(temp);
		}
		//assign temp to node,
		//regardless of whether or not temp is Pair.NIL.
		*node = temp;
		if (isNIL(*node))
			return 0;
	}
	return 1;
}//end rb_it_increment(const struct rb_tree* rbt, const struct rb_node_it* node_it)

int rb_it_decrement(const struct rb_tree* rbt, struct rb_node_it* node_it)
{
	struct rb_node** node = &(node_it->node);
	//if node is Pair.NIL
	if (isNIL(*node))
	{
		//Since the implementation has been changed a little,
		//the statement below has the correct result.
		//2018-04-25
		*node = maximum(rb_parent(*node));
		return 1;
	}
	//left child exists
	if (!isNIL((*node)->rb_left))
	{
		*node = (*node)->rb_left;
		*node = maximum(*node);
	}
	//no left child
	else {
		//NB: the case is considered when node is root, and temp is Pair.NIL.
		struct rb_node* temp = rb_parent(*node);
		//trace back until node is not a left child
		while (*node == temp->rb_left)
		{
			*node = temp;
			temp = rb_parent(temp);
		}
		//temp is Pair.NIL, 
		//which means the node is left-most of the RB_Pairree.
		//throw an exception
		if (isNIL(temp))
			return 0;
		//assign temp to node
		*node = temp;
		return 1;
	}
}//end rb_it_decrement(const struct rb_tree* rbt, const struct rb_node_it* node_it);

void rb_it_delete(struct rb_node_it* node_it)
{
	free(node_it);
}

struct Pair rb_it_get(struct rb_node_it* node_it)
{
	return node_it->node->pair;
}

int rb_insert(struct rb_tree* rbt, const struct Pair pair)
{
	if (NULL == rbt)
		return 0;
	return RB_Insert(rbt, pair, insert);
}

int rb_insert_assign(struct rb_tree* rbt, const struct Pair pair)
{
	if (NULL == rbt)
		return 0;
	return RB_Insert(rbt, pair, insert_or_assign);
}

int rb_erase(struct rb_tree* rbt, const struct K* key)
{
	//RB_Pairree is empty
	if (rbt->node_count == 0)return;
	//find the node according to the property of BSPair
	struct rb_node* x = rbt->rb_root;
	while (!isNIL(x))
	{
		struct rb_node* Pnode;
		if (rb_key_compare(key, x->pair.key))			//key < x.key
			x = x->rb_left;
		else if (rb_key_compare(x->pair.key, key))	    //x.key < key
			x = x->rb_right;
		else								//key collision
		{
			Pnode = x;
			doRB_Delete(rbt, Pnode);
			return 1;
		}
	}//end while
	return 0;
}//end rb_erase(struct rb_tree*, const struct K);

struct V* rb_find(const struct rb_tree* rbt, const struct K* key)
{
	if (NULL == rbt)
		return NULL;
	struct rb_node* node = doFind(rbt, key);
	if (!isNIL(node))
		return node->pair.value;
	return NULL;
}

void rb_delete(struct rb_tree* rbt)
{
	if (rbt == NULL)
		return;
	const int size = rbt->node_count;
	const int s_size = rbt->node_count / 2 + 1;
	struct rb_node** stack = (rb_node**)malloc(s_size * sizeof(rb_node*));
	int cur = 0, count = 0;
	stack[cur++] = rbt->rb_root;
	while (count < size)
	{
		struct rb_node* top = stack[cur - 1];
		struct rb_node* temp_left = top->rb_left;
		struct rb_node* temp_right = top->rb_right;
		free_node(top); count++; cur--;
		if (!isNIL(temp_left))
			stack[cur++] = temp_left;
		if (!isNIL(temp_right))
			stack[cur++] = temp_right;
	}
	free(stack);
	free(rbt->rb_NIL);
	free(rbt);
}


#undef insert
#undef insert_or_assign
