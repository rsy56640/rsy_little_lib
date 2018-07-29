#ifndef _USER_H
#define _USER_H
#include "customized_util.h"

/*********************     user customized     *********************/
struct K { int key; };
struct V { int value; };
struct Pair;
/*************************** for  RBtree ***************************/
int rb_key_compare(const struct K* k1, const struct K* k2);
void assign_V(struct V** src, const struct V** dest);
void free_K(struct K*);
void free_V(struct V*);
/**************************** for  Hash ****************************/
struct hash;
int K2int(const struct K*);

/*******************************************************************/

#define RAII(free_func) __attribute__((cleanup(free_func)))

struct K* getK(int);
struct V* getV(int);
void freeK(struct K**);
void freeV(struct V**);

void init_K_in_Pair(struct K**, int);
void init_V_in_Pair(struct V**, int);


#endif // !_USER_H
