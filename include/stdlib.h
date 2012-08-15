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
#define atoi(s) ((int)strtol(s, NULL, 10))
#define atol(s) strtol(s, NULL, 10)

double strtod(const char *s, char **str_end);
#define atof(s) ((float)strtod(s, NULL))

void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

void exit(int status);

#ifdef __cplusplus
}
#endif

#endif
