#ifndef JCR_H
#define JCR_H
#include <stdio.h>
#include <stdint.h>

// Vector
typedef struct {
    void *data;
    size_t dsize;
    size_t size;
    size_t capacity;
} Vector;

#define get_vector_type(type, Vector, i) (*((type *) get_vector(Vector, i)))

Vector *make_vector_dsize(size_t capacity, size_t dsize);
Vector *make_vector(size_t capacity);
Vector *copy_vector(const Vector *v);
Vector *concat_vectors(Vector *a, Vector *b);
int compare_vector(const Vector *v1, const Vector *v2);
void free_vector(Vector *v);
void *get_vector(const Vector *v, const size_t i);
void set_vector(Vector *v, const size_t i, void * p);
void append_vector(Vector *v, void *p);

// IVector
typedef struct {
    size_t size;
    size_t capacity;
    size_t *data;
} IVector; // Vector for indices;

IVector *make_ivector();
IVector *copy_ivector(IVector *v);
void free_ivector(IVector *v);
size_t get_ivector(IVector *v, size_t i);
void set_ivector(IVector *v, size_t i, size_t val);
void append_ivector(IVector *v, size_t val);
int compare_ivector(const IVector *v1, const IVector *v2);

// PVector
typedef struct {
    size_t size;
    size_t capacity;
    void **data;
} PVector; // Vector for indices;

PVector *make_pvector();
PVector *copy_pvector(PVector *v);
void free_pvector(PVector *v);
void *get_pvector(PVector *v, size_t i);
void set_pvector(PVector *v, size_t i, void *val);
void append_pvector(PVector *v, void *val);
int compare_pvector(const PVector *v1, const PVector *v2);

// list
typedef struct Node{
    void *val;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List{
    size_t size;
    Node *front;
    Node *back;
} List;

List *make_list();

List *push_list(List *, void *);
void *pop_list(List *l);

void **list_to_array(List *l);

void free_list(List *l);
void free_list_contents(List *l);

// map
typedef struct{
    char *key;
    void *val;
} KVP;

typedef struct{
    size_t capacity;
    size_t size;
    List **data;
} Map;

Map *make_map();
Vector *keys_map(Map *d);
int contains_map(Map *d, const char *key);
void *set_map(Map *d, const char *key, void *val);
void *get_map(Map *d, const char *key, void *def);
void *remove_map(Map *d, const char *key, void *def);

void free_map(Map *d);
void print_map(Map *d);
void free_map(Map *d);

// args
int find_arg(int argc, char* argv[], char *arg);
int find_int_arg(int argc, char **argv, char *arg, int def);
double find_float_arg(int argc, char **argv, char *arg, double def);
char *find_char_arg(int argc, char **argv, char *arg, char *def);

// io
char *fgetl(FILE *fp);

// utils
uint64_t rand64();
uint32_t rand32();
float rand_unif();
double system_time();
void error(const char *s, ...);
void malloc_error();
char *copy_string(const char *s);

#endif
