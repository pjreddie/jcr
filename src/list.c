#include "jcr.h"

#include <stdlib.h>

List *make_list()
{
	List *l = calloc(1, sizeof(List));
	return l;
}

void *pop_list(List *l)
{
    if(!l->back) return 0;
    Node *b = l->back;
    void *val = b->val;
    l->back = b->prev;
    if(l->back) l->back->next = 0;
    free(b);
    --l->size;
    
    return val;
}

List *push_list(List *l, void *val)
{
    if(!l) l = make_list();
	Node *new = calloc(1, sizeof(Node));
	new->val = val;
	new->next = 0;

	if(!l->back){
		l->front = new;
		new->prev = 0;
	}else{
		l->back->next = new;
		new->prev = l->back;
	}
	l->back = new;
	++l->size;
    return l;
}

void free_node(Node *n)
{
	Node *next;
	while(n) {
		next = n->next;
		free(n);
		n = next;
	}
}

void free_list(List *l)
{
    if(l){
        free_node(l->front);
        free(l);
    }
}

void free_list_contents(List *l)
{
    if(l){
        Node *n = l->front;
        while(n){
            free(n->val);
            n = n->next;
        }
    }
}

void **list_to_array(List *l)
{
    void **a = calloc(l->size, sizeof(void*));
    int count = 0;
    Node *n = l->front;
    while(n){
        a[count++] = n->val;
        n = n->next;
    }
    return a;
}
