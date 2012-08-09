#ifndef _FXCG_STDLIB_H
#define _FXCG_STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

long abs(long n);

void free(void *p);
void *malloc(size_t sz);
void *realloc(void *p, size_t sz);

int rand(void);
void srand(unsigned seed);

long strtol(const char *str, char **str_end, int base);
void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

void exit(int status);

#ifdef __cplusplus
}
#endif

#endif
