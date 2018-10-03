/*
 * you should first look at "customized_util.h"
 * then complete necessary data structure and functions.
**/
#include "customized_util.h"

#pragma once
#ifndef _HASH_RSY_H
#define _HASH_RSY_H

typedef struct hash hash;

/*
 * initial an empty hash, which later has to be deleted by rb_delete().
**/
extern struct hash* hash_get();

/*
 * return the size of hs.
**/
extern int hash_get_size(const struct hash* hs);

/*
 * return the insertion status:
 *    0 when value does exist or hash is NULL,
 *    1 when insertion is completed.
**/
extern int hash_insert(struct hash* hs, const struct Pair pair);

/*
 * return the insertion status:
 *    0 when insertion does nothing,
 *    1 when insertion is completed,
 *    2 when value is replaced.
**/
extern int hash_insert_assign(struct hash* hs, const struct Pair pair);

/*
 * return whether the erasion status:
 *    0 when erasion does nothing,
 *    1 when erasion is completed.
**/
extern int hash_erase(struct hash* hs, const struct K* key);

/*
 * return the amount of value appears in hs
 *   specifically,
 *     0 when value does not exist,
 *     1 when value does exist.
**/
extern struct V* hash_find(const struct hash* hs, const struct K* key);

/*
 * delete the hs previously initialed by hash_get().
 *   If hs is NULL, does nothing.
 *   The behavior is undefined if hs does not equal a pointer returned earlier by hash_get().
**/
extern void hash_delete(struct hash* hs);

#endif // !_HASH_RSY_H
