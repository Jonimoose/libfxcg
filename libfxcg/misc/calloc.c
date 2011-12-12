#include <stdlib.h>

extern void* memsetZero(void *dest, int n);

void *sys_calloc(int elements, int elementSize){
    int p = elements*elementSize;
    void *tmp;
    tmp=malloc(p);
    if (tmp==0)
        return 0;

    memsetZero(tmp, p);
    return tmp;
}
