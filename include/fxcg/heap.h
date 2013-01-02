#ifndef _FXCG_HEAP_H
#define _FXCG_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

void *sys_calloc(int elements, int elementSize);
void *sys_malloc(int sz);
void *sys_realloc(void *p, int sz);
void sys_free(void *p);

#ifdef __cplusplus
}
#endif

#endif
