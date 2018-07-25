#ifndef _CUSTOMIZED_UTIL_H
#define _CUSTOMIZED_UTIL_H

#define RAII(free_func) __attribute__((cleanup(free_func)))

struct K { int key; };
struct V { int value; };

int rb_key_compare(struct K* k1, struct K* k2);
void assign_V(struct V* src, const struct V* dest);
void free_V(struct V*);

/**************************************************************/

struct K* getK(int);
struct V* getV(int);
void freeK(struct K**);
void freeV(struct V**);

void init_K_in_Pair(struct K**, int);
void init_V_in_Pair(struct V**, int);


#endif // !_CUSTOMIZED_UTIL_H
