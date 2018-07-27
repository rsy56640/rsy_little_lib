#ifndef _CUSTOMIZED_UTIL_H
#define _CUSTOMIZED_UTIL_H

/*********************     user customized     *********************/
struct K;
struct V;
struct Pair
{
	struct K* key;
	struct V* value;
};
/*************************** for  RBtree ***************************/
int rb_key_compare(struct K* k1, struct K* k2);
void assign_V(struct V** src, const struct V** dest);
void free_K(struct K*);
void free_V(struct V*);
/**************************** for  Hash ****************************/
struct hash;
int K2int(struct K*);

/*******************************************************************/


#endif // !_CUSTOMIZED_UTIL_H