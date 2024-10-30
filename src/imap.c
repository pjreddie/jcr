#include "jcr.h"

#include <stdlib.h>
#include <string.h>

IMap *make_imap()
{
    IMap *d = calloc(1, sizeof(IMap));
    d->capacity = 63;
    d->size = 0;
    d->data = calloc(d->capacity, sizeof(List*));
    return d;
}

IVP *ivp_list_find(List *l, const uint64_t id)
{
    if(!l) return 0;
    Node *n = l->front;
    while(n){
        IVP *pair = (IVP*) n->val;
        if(id == pair->id){
            return pair;
        }
        n = n->next;
    }
    return 0;
}

void expand_imap(IMap *d)
{
    size_t i;
    size_t old_capacity = d->capacity;
    List **old_data = d->data;
    d->capacity = (d->capacity+1)*2 - 1;
    d->data = calloc(d->capacity, sizeof(List*));
    for(i = 0; i < old_capacity; ++i){
        List *l = old_data[i];
        if(l){
            Node *n = l->front;
            while(n){
                IVP *pair = (IVP *) n->val;
                size_t h = pair->id % d->capacity;
                d->data[h] = push_list(d->data[h], pair);
                n = n->next;
            }
        }
        free_list(old_data[i]);
    }
    free(old_data);
}

void *set_imap(IMap *d, const uint64_t id, void *val)
{
    void *old = 0;
    if((double)d->size / d->capacity > .7) expand_imap(d);

    size_t h = id % d->capacity;
    List *l = d->data[h];
    IVP *pair = ivp_list_find(l, id);
    if(pair){
        old = pair->val;
        pair->val = val;
    }else{
        pair = calloc(1, sizeof(IVP));
        pair->id = id;
        pair->val = val;
        d->data[h] = push_list(d->data[h], pair);
        ++d->size;
    }
    return old;
}

int contains_imap(IMap *d, const uint64_t id)
{
    size_t h = id % d->capacity;
    List *l = d->data[h];
    IVP *pair = ivp_list_find(l, id);
    if(pair){
        return 1;
    }
    return 0;
}

void *get_imap(IMap *d, const uint64_t id, void *def)
{
    size_t h = id % d->capacity;
    List *l = d->data[h];
    IVP *pair = ivp_list_find(l, id);
    if(pair){
        return pair->val;
    }else{
        return def;
    }
    return def;
}

void *remove_imap(IMap *d, const uint64_t id, void *def)
{
    size_t h = id % d->capacity;
    List *l = d->data[h];

    if(!l) return def;
    Node *n = l->front;

    while(n){
        IVP *pair = (IVP*) n->val;
        if(id == pair->id){
            void *v = pair->val;
            --l->size;
            if(l->front == n) l->front = n->next;
            if(l->back == n) l->back = n->prev;
            if(n->prev) n->prev->next = n->next;
            if(n->next) n->next->prev = n->prev;
            free(n);
            free(pair);
            --d->size;
            return v;
        }
        n = n->next;
    }
    return def;
}

IVector *keys_imap(IMap *d)
{
    IVector *v = make_ivector();
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        List *l = d->data[i];
        if(l){
            Node *n = l->front;
            while(n){
                IVP *pair = (IVP *) n->val;
                append_ivector(v, pair->id);
                n = n->next;
            }
        }
    }
    return v;
}

void free_imap(IMap *d)
{
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        List *l = d->data[i];
        if(l){
            Node *n = l->front;
            while(n){
                IVP *pair = (IVP *) n->val;
                free(pair);
                n = n->next;
            }
            free_list(l);
        }
    }
    free(d->data);
    free(d);
}

void print_imap(IMap *d)
{
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        List *l = d->data[i];
        if(l){
            Node *n = l->front;
            while(n){
                IVP *pair = (IVP *) n->val;
                printf("%llu: %p\n", pair->id, pair->val);
                n = n->next;
            }
        }
    }
}

