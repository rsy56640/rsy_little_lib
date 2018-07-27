#include <stdio.h>
#include <stdlib.h>
#include "rbtree_rsy.h"
#include "hash_rsy.h"
#include "user.h"

void test_rbt()
{
	printf("------ Test RBtree ------\n");

	rb_tree* rbt = rb_get();
	struct Pair pair;

	init_K_in_Pair(&pair.key, 3);
	init_V_in_Pair(&pair.value, 33);
	rb_insert(rbt, pair);

	init_K_in_Pair(&pair.key, 4);
	init_V_in_Pair(&pair.value, 44);
	rb_insert(rbt, pair);

	init_K_in_Pair(&pair.key, 5);
	init_V_in_Pair(&pair.value, 55);
	rb_insert(rbt, pair);

	RAII(freeK) struct K* k = getK(4);
	struct V* v = rb_find(rbt, k);
	printf("%d\n", v->value);

	//test insert_assign
	init_K_in_Pair(&pair.key, 4);
	init_V_in_Pair(&pair.value, 77);
	rb_insert_assign(rbt, pair);
	v = rb_find(rbt, k);
	printf("%d\n", v->value);

	init_K_in_Pair(&pair.key, 7);
	init_V_in_Pair(&pair.value, 77);
	rb_insert_assign(rbt, pair);

	init_K_in_Pair(&pair.key, 12);
	init_V_in_Pair(&pair.value, 23);
	rb_insert_assign(rbt, pair);


	rb_node_it* it1 = rb_begin(rbt);
	printf("----- test iterating ++ -----\n");
	printf("key: %d, value: %d\n", rb_it_get(it1).key->key, rb_it_get(it1).value->value);
	while (rb_it_increment(rbt, it1))
		printf("key: %d, value: %d\n", rb_it_get(it1).key->key, rb_it_get(it1).value->value);
	rb_it_delete(it1);


	printf("----- test iterating -- -----\n");
	rb_node_it* it2 = rb_end(rbt);
	while (rb_it_decrement(rbt, it2))
		printf("key: %d, value: %d\n", rb_it_get(it2).key->key, rb_it_get(it2).value->value);
	rb_it_delete(it2);


	rb_delete(rbt);

	printf("------ Test RBtree Finished ------\n");
}


void test_hash()
{
	printf("------ Test Hash ------\n");

	struct hash* hs = hash_get();
	struct Pair pair;

	init_K_in_Pair(&pair.key, 3);
	init_V_in_Pair(&pair.value, 33);
	hash_insert(hs, pair);

	init_K_in_Pair(&pair.key, 4);
	init_V_in_Pair(&pair.value, 44);
	hash_insert(hs, pair);

	init_K_in_Pair(&pair.key, 5);
	init_V_in_Pair(&pair.value, 12);
	hash_insert(hs, pair);

	printf("size of hash: %d\n", hash_get_size(hs));

	RAII(freeK) struct K* k = getK(4);
	struct V* v = hash_find(hs, k);
	printf("%d\n", v->value);


	//test insert_assign
	init_K_in_Pair(&pair.key, 4);
	init_V_in_Pair(&pair.value, 77);
	hash_insert_assign(hs, pair);

	v = hash_find(hs, k);
	printf("%d\n", v->value);

	hash_erase(hs, k);

	printf("size of hash: %d\n", hash_get_size(hs));

	hash_delete(hs);

	printf("------ Test Hash Finished ------\n");
}

int main()
{
	printf("hello from _hash_in_C!\n");

	test_rbt();
	test_hash();

	getchar();
	return 0;
}