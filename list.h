#ifndef SO_LIST_H
#define SO_LIST_H

#include <stdio.h>

typedef struct node *list;
typedef struct node *pos;

void create_list(list *L);
int isEmptyList(list l);

void InsertElement(list l, void *data);
void removeFirstElement(list l ,void(*delete_data)(void*));
void deleteAtPosition(list l, pos p, void(*delete_data)(void*));

pos first(list l);
pos next(list l, pos p);
int at_end(list l, pos p);
void *get(list l, pos p);

void deleteList(list l,  void(*delete_data)(void*));






#endif //SO_LIST_H
