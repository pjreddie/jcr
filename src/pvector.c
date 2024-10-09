#include "jcr.h"

#include <stdlib.h>
#include <string.h>

PVector *make_pvector_(size_t capacity, size_t size, void** data)
{
    PVector *v = calloc(1, sizeof(PVector));
    if(capacity < size) capacity = size;
    v->capacity = capacity;
    v->size = size;
    v->data = calloc(capacity, sizeof(void*));
    if(data) memcpy(v->data, data, size*sizeof(void*));
    return v;
}

PVector *make_pvector()
{
    return make_pvector_(16,0,0);
}

int compare_pvector(const PVector *v1, const PVector *v2)
{
    if(v1->size != v2->size) return 0;
    size_t i;
    for(i = 0; i < v1->size; ++i){
        if (v1->data[i] != v2->data[i]) return 0;
    }
    return 1;
}

PVector *copy_pvector(PVector *v)
{
    return make_pvector_(v->capacity, v->size, v->data);
}

void free_pvector(PVector *v)
{
    free(v->data);
    free(v);
}

void *get_pvector(PVector *v, size_t i)
{
    if (i >= 0 && i < v->size) return v->data[i];
    else error("PVector access out of bounds: %ld size, %ld requested\n", v->size, i);
    return 0;
}

void set_pvector(PVector *v, size_t i, void *val)
{
    if (i >= 0 && i < v->size) v->data[i] = val;
    else error("PVector access out of bounds: %ld size, %ld requested\n", v->size, i);
}

void append_pvector(PVector *v, void *val)
{
    if (v->size == v->capacity){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity*sizeof(size_t));
    }
    
    v->data[v->size] = val;
    ++v->size;
}

