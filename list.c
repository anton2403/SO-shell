/*  
Carlos Martinez Rabunal , carlos.martinez4@udc.es
Anton Lopez Nunez , anton.lopez.nunez@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "list.h"



struct node {
    void *data;
    struct node *next;
};

typedef struct node * list;


void create_list(list *L){
    *L = malloc(sizeof(struct node));
    (*L)->next = NULL;
}

int isEmptyList(list l){
    return(l->next == NULL);
} 

void InsertElement(list l, void *data){
    struct node *pos = l;

    while(pos->next != NULL)
        pos = pos->next;
    pos->next = malloc(sizeof(struct node));
    pos->next->next = NULL;
    pos->next->data = data;
}

pos previous(list l, pos p){
    pos i;
    if(p == first(l)){
        return NULL;
    }
    else{
        for(i = first(l); i->next != p && i->next != NULL; i = next(l,i)); // previous p
        return i;
    }
}


void deleteAtPosition(list l, pos p, void(*delete_data)(void*)){
    pos i;
    if(p == first(l)){
        removeFirstElement(l, delete_data);
    }
    else{
        i = previous(l,p);
        i->next = p->next;
        delete_data(p->data);
        free(p);
    }

}

void removeFirstElement(list l, void (*free_data)(void *)){
    pos p, q;
    p = l->next;
    q = l->next->next;
    free_data(p->data);
    free(p);
    l->next = q;
}

pos first(list l){
    return l;
}
pos next(list l, pos p){
    if(p->next != NULL)
        return p->next;
    else
        return NULL;
}
int at_end(list l, pos p){
    return (p->next == NULL);
}
void* get(list l, pos p){
    if (p->next != NULL) {
        return p->next->data;
    }
    else
        return NULL;
}
void deleteList(list l, void (*free_data)(void *)){
    while(next(l,first(l)) != NULL){
        removeFirstElement(l, free_data);
    }
}
