#include "jcr.h"

#include <stdlib.h>

int compare_vector(const vector *v1, const vector *v2)
{
    if(v1->size != v2->size) return 0;
    int i;
    for(i = 0; i < v1->size; ++i){
        if (v1->data[i] != v2->data[i]) return 0;
    }
    return 1;
}

vector *make_vector(size_t capacity)
{
    if (!capacity) capacity = 16;
    vector *v = calloc(1, sizeof(vector));
    v->data = calloc(capacity, sizeof (void *));
    v->capacity = capacity;
    v->size = 0;
    return v;
}

vector *copy_vector(const vector *v)
{
    vector *c = make_vector(v->size + 1);
    c->size = v->size;
    int i;
    for(i = 0; i < v->size; ++i){
        c->data[i] = v->data[i];
    }
    return c;
}

vector *concat_vectors(vector *a, vector *b)
{
    int i;
    vector *c = make_vector(a->size + b->size + 1);
    c->size = a->size + b->size;
    for(i = 0; i < a->size; ++i){
        c->data[i] = a->data[i];
    }
    for(i = 0; i < b->size; ++i){
        c->data[a->size + i] = b->data[i];
    }
    return c;
}

void free_vector(vector *v)
{
    free(v->data);
    free(v);
}

void *get_vector(const vector *v, const size_t i)
{
    if ( i < 0 || v->size <= i ) {
        error("Out of bounds vector get: %d, size %d", i, v->size);
    }
    return v->data[i];
}

void set_vector(vector *v, const size_t i, void *p)
{
    if ( i < 0 || v->size <= i ) {
        error("Out of bounds vector get: %d, size %d", i, v->size);
    }

    v->data[i] = p;
}

void append_vector(vector *v,  void *p)
{
    if ( v->size == v->capacity ){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * sizeof(void *));
    }
    v->data[v->size] = p;
    ++v->size;
}

