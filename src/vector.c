#include "jcr.h"

#include <stdlib.h>
#include <string.h>

int compare_vector(const vector *v1, const vector *v2)
{
    if(v1->size != v2->size) return 0;
    if(v1->dsize != v2->dsize) return 0;
    size_t i;
    for(i = 0; i < v1->size*v1->dsize; ++i){
        char b1 = ((char *)v1->data)[i];
        char b2 = ((char *)v2->data)[i];
        if (b1 != b2) return 0;
    }
    return 1;
}

vector *make_vector_dsize(size_t capacity, size_t dsize)
{
    if (!capacity) capacity = 16;
    if (!dsize) dsize = sizeof(void *);
    vector *v = calloc(1, sizeof(vector));
    v->data = calloc(capacity, dsize);
    v->capacity = capacity;
    v->dsize = dsize;
    v->size = 0;
    return v;
}

vector *make_vector(size_t capacity)
{
    return make_vector_dsize(capacity, sizeof(void *));
}

vector *copy_vector(const vector *v)
{
    vector *c = make_vector_dsize(v->capacity, v->dsize);
    c->size = v->size;
    memcpy(c->data, v->data, c->size*c->dsize);
    return c;
}

vector *concat_vectors(vector *a, vector *b)
{
    if ( a->dsize != b->dsize ) {
        error("Mismatch data type size in concat: %ld vs %ld", a->dsize, b->dsize);
    }
    vector *c = make_vector_dsize(a->size + b->size + 1, a->dsize);
    c->size = a->size + b->size;
    memcpy(c->data, a->data, a->size*a->dsize);
    memcpy(c->data + a->size*a->dsize, b->data, b->size*b->dsize);
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
    return v->data + i*v->dsize;
}

void set_vector(vector *v, const size_t i, void *p)
{
    if ( i < 0 || v->size <= i ) {
        error("Out of bounds vector get: %d, size %d", i, v->size);
    }

    memcpy(v->data + i*v->dsize, p, v->dsize);
}

void append_vector(vector *v,  void *p)
{
    if ( v->size == v->capacity ){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * v->dsize);
    }
    
    memcpy(v->data + v->size*v->dsize, p, v->dsize);
    ++v->size;
}

