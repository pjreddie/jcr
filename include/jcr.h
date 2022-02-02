#ifndef JCR_H
#define JCR_H
#include <stdio.h>

// vector
typedef struct {
    void **data;
    size_t size;
    size_t capacity;
} vector;


vector *make_vector(size_t capacity);
vector *copy_vector(const vector *v);
vector *concat_vectors(vector *a, vector *b);
int compare_vector(const vector *v1, const vector *v2);
void free_vector(vector *v);
void *get_vector(const vector *v, const size_t i);
void set_vector(vector *v, const size_t i, void * p);
void append_vector(vector *v, void *p);

// list
typedef struct node{
    void *val;
    struct node *next;
    struct node *prev;
} node;

typedef struct list{
    size_t size;
    node *front;
    node *back;
} list;

list *make_list();

list *push_list(list *, void *);
void *pop_list(list *l);

void **list_to_array(list *l);

void free_list(list *l);
void free_list_contents(list *l);

// map
typedef struct kvp{
    vector *key;
    void *val;
} kvp;

typedef struct map{
    size_t capacity;
    size_t size;
    list **data;
} map;

map *make_map();
void *set_map(map *d, const vector *key, void *val);
void *get_map(map *d, const vector *key, void *def);

void *set_map_s(map *d, const char *key, void *val);
void *get_map_s(map *d, const char *key, void *def);

void free_map(map *d);
void print_map(map *d);
void free_map(map *d);

// args
int find_arg(int argc, char* argv[], char *arg);
int find_int_arg(int argc, char **argv, char *arg, int def);
double find_float_arg(int argc, char **argv, char *arg, double def);
char *find_char_arg(int argc, char **argv, char *arg, char *def);

// io
char *fgetl(FILE *fp);

// utils
double now();
void error(const char *s, ...);
void malloc_error();

#endif
