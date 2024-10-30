#include "jcr.h"
#include <unistd.h>

int rand64stream()
{
    while(1){
        uint64_t r = rand64();
        fwrite(&r, sizeof(r), 1, stdout);
    }
    return 0;
}
