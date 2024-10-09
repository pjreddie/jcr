#include "jcr.h"

#include <stdlib.h>
#include <string.h>

IVector *make_ivector_(size_t capacity, size_t size, size_t* data)
{
    IVector *v = calloc(1, sizeof(IVector));
    if(capacity < size) capacity = size;
    v->capacity = capacity;
    v->size = size;
    v->data = calloc(capacity, sizeof(size_t));
    if(data) memcpy(v->data, data, size*sizeof(size_t));
    return v;
}

IVector *make_ivector()
{
    return make_ivector_(16,0,0);
}

int compare_ivector(const IVector *v1, const IVector *v2)
{
    if(v1->size != v2->size) return 0;
    size_t i;
    for(i = 0; i < v1->size; ++i){
        if (v1->data[i] != v2->data[i]) return 0;
    }
    return 1;
}

IVector *copy_ivector(IVector *v)
{
    return make_ivector_(v->capacity, v->size, v->data);
}

void free_ivector(IVector *v)
{
    free(v->data);
    free(v);
}

size_t get_ivector(IVector *v, size_t i)
{
    if (i >= 0 && i < v->size) return v->data[i];
    else error("IVector access out of bounds: %ld size, %ld requested\n", v->size, i);
    return 0;
}

void set_ivector(IVector *v, size_t i, size_t val)
{
    if (i >= 0 && i < v->size) v->data[i] = val;
    else error("IVector access out of bounds: %ld size, %ld requested\n", v->size, i);
}

void append_ivector(IVector *v, size_t val)
{
    if (v->size == v->capacity){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity*sizeof(size_t));
    }
    
    v->data[v->size] = val;
    ++v->size;
}

