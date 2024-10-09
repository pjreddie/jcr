#include "jcr.h"

#include <stdlib.h>
#include <string.h>

//djb2 hash function
//taken from http://www.cse.yorku.ca/~oz/hash.html
size_t djb2(const char *s)
{
    size_t hash = 5381;
    size_t c = 0;
    while((c = *s++)){
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
    }
    return hash;
}

Map *make_map()
{
    Map *d = calloc(1, sizeof(Map));
    d->capacity = 63;
    d->size = 0;
    d->data = calloc(d->capacity, sizeof(List*));
    return d;
}

KVP *kvp_list_find(List *l, const char *key)
{
    if(!l) return 0;
    Node *n = l->front;
    while(n){
        KVP *pair = (KVP*) n->val;
        if(0 == strcmp(key, pair->key)){
            return pair;
        }
        n = n->next;
    }
    return 0;
}

void expand_map(Map *d)
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
                KVP *pair = (KVP *) n->val;
                size_t h = djb2(pair->key)%d->capacity;
                d->data[h] = push_list(d->data[h], pair);
                n = n->next;
            }
        }
        free_list(old_data[i]);
    }
    free(old_data);
}

void *set_map(Map *d, const char *key, void *val)
{
    void *old = 0;
    if((double)d->size / d->capacity > .7) expand_map(d);

    size_t h = djb2(key) % d->capacity;
    List *l = d->data[h];
    KVP *pair = kvp_list_find(l, key);
    if(pair){
        old = pair->val;
        pair->val = val;
    }else{
        pair = calloc(1, sizeof(KVP));
        pair->key = copy_string(key);
        pair->val = val;
        d->data[h] = push_list(d->data[h], pair);
        ++d->size;
    }
    return old;
}

int contains_map(Map *d, const char *key)
{
    size_t h = djb2(key) % d->capacity;
    List *l = d->data[h];
    KVP *pair = kvp_list_find(l, key);
    if(pair){
        return 1;
    }
    return 0;
}

void *get_map(Map *d, const char *key, void *def)
{
    size_t h = djb2(key) % d->capacity;
    List *l = d->data[h];
    KVP *pair = kvp_list_find(l, key);
    if(pair){
        return pair->val;
    }else{
        return def;
    }
    return def;
}

Vector *keys_map(Map *d)
{
    Vector *v = make_vector(0);
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        List *l = d->data[i];
        if(l){
            Node *n = l->front;
            while(n){
                KVP *pair = (KVP *) n->val;
                append_vector(v, &pair->key);
                n = n->next;
            }
        }
    }
    return v;
}

void free_map(Map *d)
{
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        List *l = d->data[i];
        if(l){
            Node *n = l->front;
            while(n){
                KVP *pair = (KVP *) n->val;
                free(pair->key);
                free(pair);
                n = n->next;
            }
            free_list(l);
        }
    }
    free(d->data);
    free(d);
}

void print_map(Map *d)
{
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        List *l = d->data[i];
        if(l){
            Node *n = l->front;
            while(n){
                KVP *pair = (KVP *) n->val;
                char *key = pair->key;
                printf("%s: %p\n", key, pair->val);
                n = n->next;
            }
        }
    }
}

