#ifndef _FXCG_STDLIB_H
#define _FXCG_STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0

#define	RAND_MAX	0x7ffffffd

#include <stddef.h>

int abs(int n);

void free(void *p);
void *malloc(size_t sz);
void *realloc(void *p, size_t sz);

int rand(void);
void srand(unsigned seed);
int sys_rand(void);
void sys_srand(unsigned seed);

long strtol(const char *str, char **str_end, int base);
static inline int atoi(const char*s){return strtol(s,NULL,10);}
static inline long atol(const char*s){return strtol(s,NULL,10);}
int sys_atoi(const char * s);

double strtod(const char *s, char **str_end);
#define atof(s) ((float)strtod(s, NULL))

void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

void exit(int status) __attribute__ ((noreturn));
void abort();

#ifdef __cplusplus
}
#endif

#endif
