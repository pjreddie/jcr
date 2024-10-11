#include "jcr.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>


#define EPS .0001
static int tests_total = 0;
static int tests_fail = 0;
#define TEST(EX) do { ++tests_total; if(!(EX)) {\
    fprintf(stderr, "failed: [%s] testing [%s] in %s, line %d\n", __FUNCTION__, #EX, __FILE__, __LINE__); \
    ++tests_fail; }else{fprintf(stderr, "passed: [%s] testing [%s] in %s, line %d\n", __FUNCTION__, #EX, __FILE__, __LINE__);}} while (0)


int within_eps(double a, double b)
{
    if (fabs(a-b) < EPS) return 1;
    return 0;
}

void test_io()
{
    printf("Testing io...\n");
    FILE *fp = fopen("src/test.c", "r");
    char *l = fgetl(fp);
    TEST(strncmp(l, "#include \"jcr.h\"", 20) == 0);
    while(l){
        char *next = fgetl(fp);
        if(!next){
            TEST(strncmp(l, "// Tha End", 20) > 0);
            TEST(strncmp(l, "// The End", 20) == 0);
        }
        free(l);
        l = next;
    }
    fclose(fp);

    printf("\n");
}

void test_args()
{
    printf("Testing arg parser...\n");
    char *argv[7] = {"-d","10","-test","-s","string", "-pi", "3.14159"};
    int argc = 7;
    TEST(find_arg(argc, argv, "-t") == 0);
    TEST(find_arg(argc, argv, "-b") == 0);
    TEST(find_arg(argc, argv, "-test") == 1);
    TEST(find_int_arg(argc, argv, "-e", 0) == 0);
    TEST(find_int_arg(argc, argv, "-d", 0) == 10);
    TEST(strncmp(find_char_arg(argc, argv, "-s", 0), "string", 7)==0);
    TEST(find_char_arg(argc, argv, "-t", 0) == 0);
    TEST(within_eps(find_float_arg(argc, argv, "-pi", 0), 3.14159));
    TEST(within_eps(find_float_arg(argc, argv, "-e", 2.71828), 2.71828));
    printf("\n");
}

void test_list()
{
    printf("Testing list...\n");
    List *l = make_list();
    TEST(l->size == 0);
    int i;
    for(i = 0; i < 9876; ++i){
        push_list(l, (void *) (size_t) i);   
    }
    TEST(l->size == 9876);
    TEST(l->front->val == (void *) 0);
    TEST(l->back->val == (void *) 9875);
    TEST(pop_list(l) == (void *) 9875);
    TEST(pop_list(l) == (void *) 9874);
    TEST(l->size == 9874);

    void **a = list_to_array(l);
    TEST(a[0] == (void *) 0);
    TEST(a[1] == (void *) 1);
    TEST(a[9873] == (void *) 9873);
    free_list(l);
    free(a);
    printf("\n");
}

void test_ivector()
{
    printf("Testing index vector (IVector)...\n");
    IVector *v = make_ivector();
    size_t i;
    for(i = 0; i < 9876; ++i){
        append_ivector(v, i);
    }
    TEST(v->size == 9876);
    TEST(v->capacity > v->size);
    TEST(get_ivector(v, 0) == 0);
    TEST(get_ivector(v, 2345) == 2345);
    
    IVector *c = copy_ivector(v);
    TEST(compare_ivector(v, c) == 1);

    IVector *d = make_ivector();
    TEST(compare_ivector(d, c) == 0);
    for(i = 0; i < 9876; ++i){
        append_ivector(d, i);
    }
    TEST(compare_ivector(d, c) == 1);
    i = 0;
    set_ivector(d, 9875, i);
    TEST(compare_ivector(d, c) == 0);

    free_ivector(v);
    free_ivector(c);
    free_ivector(d);
    printf("\n");
}

void test_pvector()
{
    printf("Testing pointer vector (PVector)...\n");
    PVector *v = make_pvector();
    size_t i;
    for(i = 0; i < 9876; ++i){
        append_pvector(v, (void *)i);
    }
    TEST(v->size == 9876);
    TEST(v->capacity > v->size);
    TEST(get_pvector(v, 0) == (void *) 0);
    TEST(get_pvector(v, 2345) == (void *) 2345);
    
    PVector *c = copy_pvector(v);
    TEST(compare_pvector(v, c) == 1);

    PVector *d = make_pvector();
    TEST(compare_pvector(d, c) == 0);
    for(i = 0; i < 9876; ++i){
        append_pvector(d, (void *) i);
    }
    TEST(compare_pvector(d, c) == 1);
    i = 0;
    set_pvector(d, 9875, (void *) i);
    TEST(compare_pvector(d, c) == 0);

    free_pvector(v);
    free_pvector(c);
    free_pvector(d);
    printf("\n");
}

void test_vector()
{
    printf("Testing vector...\n");
    Vector *v = make_vector(0);
    size_t i;
    for(i = 0; i < 9876; ++i){
        append_vector(v, &i);
    }
    TEST(v->size == 9876);
    TEST(v->capacity > v->size);
    TEST(get_vector_type(void *, v, 0) == (void *) 0);
    TEST(get_vector_type(void *, v, 2345) == (void *) 2345);
    
    Vector *c = copy_vector(v);
    TEST(compare_vector(v, c) == 1);

    Vector *d = make_vector(0);
    TEST(compare_vector(d, c) == 0);
    for(i = 0; i < 9876; ++i){
        append_vector(d, &i);
    }
    TEST(compare_vector(d, c) == 1);
    i = 0;
    set_vector(d, 9875, &i);
    TEST(compare_vector(d, c) == 0);

    

    free_vector(v);
    free_vector(c);
    free_vector(d);
    printf("\n");
}

void test_map()
{
    printf("Testing map...\n");
    Map *m = make_map();
    TEST(get_map(m, "key1", 0) == (void *) 0);
    set_map(m, "key1", (void *) 420);
    set_map(m, "key2", (void *) 1312);
    TEST(get_map(m, "key1", 0) == (void *) 420);
    TEST(get_map(m, "key3", 0) == (void *) 0);
    TEST(set_map(m, "key2", (void *) 666) == (void *) 1312);
    TEST(m->size == 2);
    size_t i;
    for(i = 0; i < 2000; ++i){
        char buff[32];
        sprintf(buff, "key%ld", i);
        set_map(m, buff, (void*) i);
    }
    TEST(m->size == 2000);
    TEST(get_map(m, "key1", 0) == (void *) 1);
    TEST(get_map(m, "key1234", 0) == (void *) 1234);
    TEST(remove_map(m, "key1234", 0) == (void *) 1234);
    TEST(m->size == 1999);
    TEST(get_map(m, "key1234", 0) == (void *) 0);
    TEST(remove_map(m, "key1234", 0) == (void *) 0);
    TEST(m->size == 1999);
    for(i = 0; i < 2000; ++i){
        char buff[32];
        sprintf(buff, "key%ld", i);
        remove_map(m, buff, (void*) 0);
    }
    TEST(m->size == 0);
    for(i = 0; i < 2000; ++i){
        char buff[32];
        sprintf(buff, "key%ld", i);
        set_map(m, buff, (void*) i);
    }
    TEST(m->size == 2000);
    TEST(get_map(m, "key1", 0) == (void *) 1);
    TEST(get_map(m, "key1234", 0) == (void *) 1234);

    free_map(m);
    printf("\n");
}

void test_rand_unif()
{
    printf("Testing rand_unif...\n");
    srand64(123456);
    int i;
    int N = 1000000;
    float sum = 0;
    float min = FLT_MAX;
    float max = FLT_MIN;
    for(i = 0; i < N; ++i){
        float r = rand_unif();
        if(r < min) min = r;
        if(r > max) max = r;
        sum += r;
    }
    TEST(min < 0.001);
    TEST(min >= 0);
    TEST(max > 0.999);
    TEST(max <= 1);
    TEST(.499 < sum/N);
    TEST(sum/N < .501);
    printf("%f\n", sum/N);
    printf("\n");
}

int main()
{
    test_io();
    test_args();
    test_list();
    test_vector();
    test_ivector();
    test_pvector();
    test_map();
    test_rand_unif();
    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}

// The End
