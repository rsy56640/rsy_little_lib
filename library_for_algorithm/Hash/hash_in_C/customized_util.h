#ifndef _CUSTOMIZED_UTIL_H
#define _CUSTOMIZED_UTIL_H
#include <stdlib.h>
/*
 * Users are able to customize their own data structure in Pair,
 * which means "struct K" and "struct V" should be defined by users.
 * Also there are some functions left to be defined.
**/
/*********************     user customized     *********************/
struct K;                                                     //TO BE DEFINED
struct V;                                                     //TO BE DEFINED
struct Pair
{
	struct K* key;
	struct V* value;
};
/*************************** for  RBtree ***************************/
int rb_key_compare(const struct K* k1, const struct K* k2);    //TO BE DEFINED
void assign_V(struct V** src, const struct V** dest);          //TO BE DEFINED
void free_K(struct K*);                                        //TO BE DEFINED
void free_V(struct V*);                                        //TO BE DEFINED
/**************************** for  Hash ****************************/
size_t K2int(const struct K*);                                          //TO BE DEFINED
/*******************************************************************/


#endif // !_CUSTOMIZED_UTIL_H
