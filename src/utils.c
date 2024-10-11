#include "jcr.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

static __uint128_t state = 1;

void srand64(uint64_t seed)
{
    state = seed;
}

uint64_t rand64()
{
    const __uint128_t M1 = 0xa4c1f32680f70c55;
    const __uint128_t M2 = ((__uint128_t)0x2d99787926d46932) << 64;
    const __uint128_t MULTIPLIER = M1+M2;
    state *= MULTIPLIER;
    state += MULTIPLIER;
    return state >> 64;

}

uint32_t rand32()
{
    return (uint32_t)(rand64()>>32);
}

float rand_unif()
{
    return (float)rand32() / ((uint32_t)0xFFFFFFFF);
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

double system_time()
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

