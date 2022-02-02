#include "jcr.h"

#include <stdlib.h>

list *make_list()
{
	list *l = calloc(1, sizeof(list));
	return l;
}

void *pop_list(list *l)
{
    if(!l->back) return 0;
    node *b = l->back;
    void *val = b->val;
    l->back = b->prev;
    if(l->back) l->back->next = 0;
    free(b);
    --l->size;
    
    return val;
}

list *push_list(list *l, void *val)
{
    if(!l) l = make_list();
	node *new = calloc(1, sizeof(node));
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

void free_node(node *n)
{
	node *next;
	while(n) {
		next = n->next;
		free(n);
		n = next;
	}
}

void free_list(list *l)
{
    if(l){
        free_node(l->front);
        free(l);
    }
}

void free_list_contents(list *l)
{
    if(l){
        node *n = l->front;
        while(n){
            free(n->val);
            n = n->next;
        }
    }
}

void **list_to_array(list *l)
{
    void **a = calloc(l->size, sizeof(void*));
    int count = 0;
    node *n = l->front;
    while(n){
        a[count++] = n->val;
        n = n->next;
    }
    return a;
}
