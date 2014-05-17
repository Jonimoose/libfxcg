#ifndef _FXCG_STDLIB_H
#define _FXCG_STDLIB_H


#define        EXIT_FAILURE        1
#define        EXIT_SUCCESS        0
#define	RAND_MAX 0x7ffffffd

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
typedef unsigned size_t;
#endif
#include <stddef.h>

int putenv(char *string);

void* calloc (size_t num, size_t size);

int rename(const char *old, const char *newName);

long abs(long n);

void free(void *p);
void *malloc(size_t sz);
void *realloc(void *p, size_t sz);

int rand(void);
void srand(unsigned seed);
void sranddev(void);
int sys_rand(void);
void sys_srand(unsigned seed);

long strtol(const char *str, char **str_end, int base);
#define atoi(s) ((int)strtol(s, NULL, 10))
#define atol(s) strtol(s, NULL, 10)
int sys_atoi(const char * s);

double strtod (const char* str, char** endptr);
#define atof(s) ((float)strtod(s, NULL))

void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

void exit(int status);

int getenv(const char * name);

void abort(void);

int system (const char* command);

#ifdef __cplusplus
}
#endif

#endif
