#include "customized_util.h"
#include <stdlib.h>
#include <stdio.h>


int rb_key_compare(struct K* k1, struct K* k2)
{
	if (k1->key < k2->key)
		return 1;
	return 0;
}

void assign_V(struct V* src, const struct V* dest)
{
	free_V(src);
	src->value = dest->value;
}

void free_V(struct V* v)
{

}

/**************************************************************/

struct K* getK(int k)
{
	struct K* key = malloc(sizeof(struct K));
	key->key = k;
	return key;
}
struct V* getV(int v)
{
	struct V* value = malloc(sizeof(struct V));
	value->value = v;
	return value;
}
void freeK(struct K** key)
{
	free(*key);
}
void freeV(struct V** value)
{
	free(*value);
}


void init_K_in_Pair(struct K** key, int k)
{
	*key = malloc(sizeof(struct K));
	(*key)->key = k;
}

void init_V_in_Pair(struct V** value, int v)
{
	*value = malloc(sizeof(struct V));
	(*value)->value = v;
}
