#ifndef LIST_H
#define LIST_H
#include "darknet.h"

list *make_list();
int list_find(list *l, void *val);
void *list_pop(list *l);
void list_insert(list *, void *);


void free_list_contents(list *l);

void free_list(list *l);

void **list_to_array(list *l);

#endif
