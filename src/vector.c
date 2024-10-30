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

Vector *make_vector_(size_t capacity, size_t size, size_t dsize, void **data)
{
    if(!capacity) capacity = 16;
    if(!dsize) dsize = sizeof(void *);

    Vector *v = calloc(1, sizeof(Vector));
    v->data = calloc(capacity, dsize);
    v->capacity = capacity;
    v->dsize = dsize;
    v->size = size;
    if(data && size) memcpy(v->data, data, size*dsize);
    return v;
}

Vector *make_vector_dsize(size_t dsize)
{
    return make_vector_(0,0,dsize,0);
}

Vector *make_vector()
{
    return make_vector_(0,0,0,0);
}

Vector *copy_vector(const Vector *v)
{
    return make_vector_(v->capacity, v->size, v->dsize, v->data);
}

Vector *concat_vectors(Vector *a, Vector *b)
{
    if ( a->dsize != b->dsize ) {
        error("Mismatch data type size in concat: %ld vs %ld", a->dsize, b->dsize);
    }
    Vector *c = make_vector_(a->capacity + b->capacity, a->size + b->size, a->dsize, 0);
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

