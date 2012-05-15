#ifndef _FXCG_HEAP_H
#define _FXCG_HEAP_H

void *sys_malloc(int sz);
void *sys_realloc(void *p, int sz);
void sys_free(void *p);

#endif
