#include "jcr.h"

#include <string.h>
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
    list *l = make_list();
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
    free_list(l);
    printf("\n");
}

int main()
{
    test_args();
    test_list();
    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}
