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

map *make_map()
{
    map *d = calloc(1, sizeof(map));
    d->capacity = 63;
    d->size = 0;
    d->data = calloc(d->capacity, sizeof(list*));
    return d;
}

kvp *kvp_list_find(list *l, const char *key)
{
    if(!l) return 0;
    node *n = l->front;
    while(n){
        kvp *pair = (kvp*) n->val;
        if(0 == strcmp(key, pair->key)){
            return pair;
        }
        n = n->next;
    }
    return 0;
}

void expand_map(map *d)
{
    size_t i;
    size_t old_capacity = d->capacity;
    list **old_data = d->data;
    d->capacity = (d->capacity+1)*2 - 1;
    d->data = calloc(d->capacity, sizeof(list*));
    for(i = 0; i < old_capacity; ++i){
        list *l = old_data[i];
        if(l){
            node *n = l->front;
            while(n){
                kvp *pair = (kvp *) n->val;
                size_t h = djb2(pair->key)%d->capacity;
                d->data[h] = push_list(d->data[h], pair);
                n = n->next;
            }
        }
        free_list(old_data[i]);
    }
    free(old_data);
}

void *set_map(map *d, const char *key, void *val)
{
    void *old = 0;
    if((double)d->size / d->capacity > .7) expand_map(d);

    size_t h = djb2(key) % d->capacity;
    list *l = d->data[h];
    kvp *pair = kvp_list_find(l, key);
    if(pair){
        old = pair->val;
        pair->val = val;
    }else{
        pair = calloc(1, sizeof(kvp));
        pair->key = copy_string(key);
        pair->val = val;
        d->data[h] = push_list(d->data[h], pair);
        ++d->size;
    }
    return old;
}

int contains_map(map *d, const char *key)
{
    size_t h = djb2(key) % d->capacity;
    list *l = d->data[h];
    kvp *pair = kvp_list_find(l, key);
    if(pair){
        return 1;
    }
    return 0;
}

void *get_map(map *d, const char *key, void *def)
{
    size_t h = djb2(key) % d->capacity;
    list *l = d->data[h];
    kvp *pair = kvp_list_find(l, key);
    if(pair){
        return pair->val;
    }else{
        return def;
    }
    return def;
}

vector *keys_map(map *d)
{
    vector *v = make_vector(0);
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        list *l = d->data[i];
        if(l){
            node *n = l->front;
            while(n){
                kvp *pair = (kvp *) n->val;
                append_vector(v, &pair->key);
                n = n->next;
            }
        }
    }
    return v;
}

void free_map(map *d)
{
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        list *l = d->data[i];
        if(l){
            node *n = l->front;
            while(n){
                kvp *pair = (kvp *) n->val;
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

void print_map(map *d)
{
    size_t i;
    for(i = 0; i < d->capacity; ++i){
        list *l = d->data[i];
        if(l){
            node *n = l->front;
            while(n){
                kvp *pair = (kvp *) n->val;
                char *key = pair->key;
                printf("%s: %p\n", key, pair->val);
                n = n->next;
            }
        }
    }
}

