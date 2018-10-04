/*
 * you should first look at "customized_util.h"
 * then complete necessary data structure and functions.
 * About Concurrency: define macro `RSY_CONCURRENCY_MAP` to take advantage of concurrency.
**/
#include "customized_util.h"

#pragma once
#ifndef _RBTREE_RSY_H
#define _RBTREE_RSY_H

typedef struct rb_tree rb_tree;
typedef struct rb_node rb_node;
typedef struct rb_node_it rb_node_it;

/*
 * initial an empty rbt, which later has to be deleted by rb_delete().
**/
extern struct rb_tree* rb_get();

/*
 * return the minimal value in rbt,
 *   if the rbt is empty,
 *   the return value will be rb_NULL.
**/
extern struct Pair rb_get_min(const struct rb_tree* rbt);

/*
 * return the maximal value in rbt,
 *   if the rbt is empty,
 *   the return value will be rb_NULL.
**/
extern struct Pair rb_get_max(const struct rb_tree* rbt);

/*
 * return the size of rbt.
**/
extern int rb_get_size(const struct rb_tree* rbt);

/*
 * return the begin iterator of rbt.
**/
extern struct rb_node_it* rb_begin(const struct rb_tree* rbt);

/*
 * return the end iterator of rbt.
**/
extern struct rb_node_it* rb_end(const struct rb_tree* rbt);

/* return 0 if there is no next node,
 *        1 if possible.
**/
extern int rb_it_increment(const struct rb_tree* rbt, struct rb_node_it* node_it);

/* return 0 if there is no previous node,
 *        1 if possible.
**/
extern int rb_it_decrement(const struct rb_tree* rbt, struct rb_node_it* node_it);

extern void rb_it_delete(struct rb_node_it* node_it);

extern struct Pair rb_it_get(struct rb_node_it* node_it);

/*
 * return the insertion status:
 *    0 when value does exist or rbt is NULL,
 *    1 when insertion is completed.
**/
extern int rb_insert(struct rb_tree* rbt, const struct Pair pair);

/*
 * return the insertion status:
 *    0 when insertion does nothing,
 *    1 when insertion is completed,
 *    2 when value is replaced.
**/
extern int rb_insert_assign(struct rb_tree* rbt, const struct Pair pair);

/*
 * return whether the erasion status:
 *    0 when erasion does nothing,
 *    1 when erasion is completed.
**/
extern int rb_erase(struct rb_tree* rbt, const struct K* key);

/*
 * return the amount of value appears in rbt
 *   specifically,
 *     0 when value does not exist,
 *     1 when value does exist.
**/
extern struct V* rb_find(const struct rb_tree* rbt, const struct K* key);

/*
 * delete the rbt previously initialed by rb_get().
 *   If rbt is NULL, does nothing.
 *   The behavior is undefined if rbt does not equal a pointer returned earlier by rb_get().
**/
extern void rb_delete(struct rb_tree* rbt);

#endif // !_RBTREE_RSY_H
