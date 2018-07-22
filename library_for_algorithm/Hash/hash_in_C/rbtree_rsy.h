/*
 *
 *
 *
 *
**/
#pragma once
#ifndef _RBTREE_RSY_H
#define _RBTREE_RSY_H
#include <stdio.h>
#include <stdlib.h>

typedef struct rb_tree rb_tree;
typedef struct rb_node rb_node;
#define rb_NULL 0x80000000

/*
 * initial an empty rbt, which later has to be deleted by rb_delete().
**/
extern struct rb_tree* rb_get();

/*
 * return the minimal value in rbt,
 *   if the rbt is empty,
 *   the return value will be rb_NULL.
**/
extern int rb_get_min(const struct rb_tree* rbt);

/*
 * return the maximal value in rbt,
 *   if the rbt is empty,
 *   the return value will be rb_NULL.
**/
extern int rb_get_max(const struct rb_tree* rbt);

/*
 * return the insertion status:
 *   -1 when rbt is NULL,
 *    0 when value does exist,
 *    1 when insertion is completed.
 * Notice: the value shall not be rb_NULL(0x80000000).
**/
extern int rb_insert(struct rb_tree* rbt, const int value);

/*
 * return the insertion status:
 *   -1 when rbt is NULL,
 *    1 when insertion is completed.
 * Notice: the value shall not be rb_NULL(0x80000000).
**/
extern int rb_insert_assign(struct rb_tree* rbt, const int value);

/*
 * return whether the erasion status:
 *   -1 when rbt is NULL,
 *    0 when value does not exist,
 *    1 when erasion is completed.
**/
extern int rb_erase(struct rb_tree* rbt, const int value);

/*
 * return the amount of value appears in rbt
 *   specifically,
 *     0 when value does not exist,
 *     1 when value does exist.
**/
extern int rb_find(const struct rb_tree* rbt, const int value);

/*
 * delete the rbt previously initialed by rb_get().
 *   If rbt is NULL, does nothing.
 *   The behavior is undefined if rbt does not equal a pointer returned earlier by rb_get().
**/
extern void rb_delete(struct rb_tree* rbt);

#endif // !_RBTREE_RSY_H
