#include "jcr.h"

#include <stdlib.h>
#include <string.h>

//djb2 hash function
//taken from http://www.cse.yorku.ca/~oz/hash.html
size_t hash_vector(const vector *v)
{
    size_t hash = 5381;
    size_t c;

    int i = 0;
    for(i = 0; i < v->size; ++i){
        c = (size_t) v->data[i];
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
    }

    return hash;
}

inline static char *vector_to_string(const vector *v)
{
    char *s = calloc(v->size + 1, sizeof(char));
    int i;
    for(i = 0; i < v->size; ++i){
        s[i] = (int)(size_t)v->data[i];
    }
    return s;
}

inline static vector *string_to_vector(const char *s)
{
    int n = strlen(s);
    vector *v = make_vector(n);
    int i;
    for(i = 0; i < n; ++i){
        append_vector(v, (void *) (size_t) s[i]);
    }
    return v;
}

map *make_map()
{
    map *d = calloc(1, sizeof(map));
    d->capacity = 63;
    d->size = 0;
    d->data = calloc(d->capacity, sizeof(list*));
    return d;
}

kvp *kvp_list_find(list *l, const vector *key)
{
    if(!l) return 0;
    node *n = l->front;
    while(n){
        kvp *pair = (kvp*) n->val;
        if(compare_vector(key, pair->key)){
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
                size_t h = hash_vector(pair->key)%d->capacity;
                d->data[h] = push_list(d->data[h], pair);
                n = n->next;
            }
        }
        free_list(old_data[i]);
    }
    free(old_data);
}

void *set_map(map *d, const vector *key, void *val)
{
    void *old = 0;
    if((double)d->size / d->capacity > .7) expand_map(d);

    size_t h = hash_vector(key) % d->capacity;
    list *l = d->data[h];
    kvp *pair = kvp_list_find(l, key);
    if(pair){
        old = pair->val;
        pair->val = val;
    }else{
        pair = calloc(1, sizeof(kvp));
        pair->key = copy_vector(key);
        pair->val = val;
        d->data[h] = push_list(d->data[h], pair);
        ++d->size;
    }
    return old;
}

void *set_map_s(map *d, const char *key, void *val)
{
    vector *v = string_to_vector(key);
    void *old = set_map(d, v, val);
    free_vector(v);
    return old;
}

void *get_map(map *d, const vector *key, void *def)
{
    size_t h = hash_vector(key) % d->capacity;
    list *l = d->data[h];
    kvp *pair = kvp_list_find(l, key);
    if(pair){
        return pair->val;
    }else{
        return def;
    }
    return def;
}

void *get_map_s(map *d, const char *key, void *def)
{
    vector *v = string_to_vector(key);
    void *val =  get_map(d, v, def);
    free_vector(v);
    return val;
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
                free_vector(pair->key);
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
    for(i = 0; i < d->size; ++i){
        list *l = d->data[i];
        if(l){
            node *n = l->front;
            while(n){
                kvp *pair = (kvp *) n->val;
                char *key = vector_to_string(pair->key);
                printf("%s: %p\n", key, pair->val);
                free(key);
                n = n->next;
            }
        }
    }
}

