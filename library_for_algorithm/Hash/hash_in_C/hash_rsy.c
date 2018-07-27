#include "hash_rsy.h"
#include "rbtree_rsy.h"

#define MAXSIZE_HASH_RSY 5
const inline int magic_num = 3;
const inline int addition = 1;

struct hash
{
	struct rb_tree* bucket[MAXSIZE_HASH_RSY];
	unsigned int count;
};

static inline int map(int value)
{
	return ((value * magic_num + addition) + MAXSIZE_HASH_RSY) % MAXSIZE_HASH_RSY;
}

/********************    Implementation for header file    ********************/

struct hash* hash_get()
{
	struct hash* hs = malloc(sizeof(struct hash));
	hs->count = 0;
	for (int i = 0; i < MAXSIZE_HASH_RSY; i++)
		hs->bucket[i] = rb_get();
	return hs;
}

int hash_get_size(const struct hash* hs)
{
	return hs->count;
}

int hash_insert(struct hash* hs, const struct Pair pair)
{
	if (NULL == hs)
		return 0;
	int mapped_value = map(K2int(pair.key));
	struct rb_tree* rbt = hs->bucket[mapped_value];
	if (rb_insert(rbt, pair))
	{
		hs->count++;
		return 1;
	}
	return 0;
}

int hash_insert_assign(struct hash* hs, const struct Pair pair)
{
	if (NULL == hs)
		return 0;
	int mapped_value = map(K2int(pair.key));
	struct rb_tree* rbt = hs->bucket[mapped_value];
	int sig = rb_insert_assign(rbt, pair);
	if (sig == 1)
		hs->count++;
	return sig;
}

int hash_erase(struct hash* hs, const struct K* key)
{
	if (NULL == hs)
		return 0;
	int mapped_value = map(K2int(key));
	struct rb_tree* rbt = hs->bucket[mapped_value];
	if (rb_erase(rbt, key))
	{
		hs->count--;
		return 1;
	}
	return 0;
}

struct V* hash_find(const struct hash* hs, const struct K* key)
{
	if (NULL == hs)
		return 0;
	int mapped_value = map(K2int(key));
	struct rb_tree* rbt = hs->bucket[mapped_value];
	return rb_find(rbt, key);
}

void hash_delete(struct hash* hs)
{
	for (int i = 0; i < MAXSIZE_HASH_RSY; i++)
		rb_delete(hs->bucket[i]);
	free(hs);
}


#undef MAXSIZE_HASH_RSY
