#include "jcr.h"

#include <stdlib.h>
#include <string.h>

int compare_vector(const Vector *v1, const Vector *v2)
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

Vector *make_vector_dsize(size_t capacity, size_t dsize)
{
    if (!capacity) capacity = 16;
    if (!dsize) dsize = sizeof(void *);
    Vector *v = calloc(1, sizeof(Vector));
    v->data = calloc(capacity, dsize);
    v->capacity = capacity;
    v->dsize = dsize;
    v->size = 0;
    return v;
}

Vector *make_vector(size_t capacity)
{
    return make_vector_dsize(capacity, sizeof(void *));
}

Vector *copy_vector(const Vector *v)
{
    Vector *c = make_vector_dsize(v->capacity, v->dsize);
    c->size = v->size;
    memcpy(c->data, v->data, c->size*c->dsize);
    return c;
}

Vector *concat_vectors(Vector *a, Vector *b)
{
    if ( a->dsize != b->dsize ) {
        error("Mismatch data type size in concat: %ld vs %ld", a->dsize, b->dsize);
    }
    Vector *c = make_vector_dsize(a->size + b->size + 1, a->dsize);
    c->size = a->size + b->size;
    memcpy(c->data, a->data, a->size*a->dsize);
    memcpy(c->data + a->size*a->dsize, b->data, b->size*b->dsize);
    return c;
}

void free_vector(Vector *v)
{
    free(v->data);
    free(v);
}

void *get_vector(const Vector *v, const size_t i)
{
    if ( i < 0 || v->size <= i ) {
        error("Out of bounds vector get: %d, size %d", i, v->size);
    }
    return v->data + i*v->dsize;
}

void set_vector(Vector *v, const size_t i, void *p)
{
    if ( i < 0 || v->size <= i ) {
        error("Out of bounds vector get: %d, size %d", i, v->size);
    }

    memcpy(v->data + i*v->dsize, p, v->dsize);
}

void append_vector(Vector *v,  void *p)
{
    if ( v->size == v->capacity ){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * v->dsize);
    }
    
    memcpy(v->data + v->size*v->dsize, p, v->dsize);
    ++v->size;
}

