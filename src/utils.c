#include "jcr.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

float rand_unif()
{
    return (float)rand() / RAND_MAX;
}

void error(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    va_end(args);
    exit(1);
}

void malloc_error()
{
    fprintf(stderr, "Malloc error\n");
    exit(-1);
}

double now()
{
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

char *copy_string(const char *s)
{
    char *copy = malloc(strlen(s)+1);
    strncpy(copy, s, strlen(s)+1);
    return copy;
}

