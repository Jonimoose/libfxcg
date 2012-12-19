#ifdef __cplusplus
extern "C" {
#endif 

#ifndef _MISC_H_
#define _MISC_H_

#include <stddef.h>

typedef unsigned short color_t;

void VRAM_CopySprite(const color_t* data, int x, int y, int width, int height);
void VRAM_XORSprite(const color_t* data, int x, int y, int width, int height);

int sys_rand(void);
void sys_srand(unsigned seed);

void *sys_memmove(void *dest, const void *src, size_t n);

void *memsetZero(void *s, size_t n);

int sys_atoi(const char * s);

void *sys_calloc(int elements, int elementSize);

char *sys_strcat(char *dest, const char *src);
char *sys_strchr(const char *s, int c);
int sys_strcmp(const char *s1, const char *s2);
char *sys_strcpy(char *dest, const char *src);
char *sys_strncpy(char *dest, const char *src, size_t n);

#ifdef USE_FXCG_STDLIB

#define atoi sys_atoi
#define rand sys_rand
#define srand sys_rand
#define malloc sys_malloc
#define calloc sys_calloc
#define realloc sys_realloc
#define free sys_free
#define memcmp sys_memcmp
#define memcmp3 sys_memcmp3
#define memcpy sys_memcpy
#define memset sys_memset
#define memmove sys_memmove
#define strlen sys_strlen
#define strcpy sys_strcpy
#define strncpy sys_strncpy
#define strchr sys_strchr
#define strcmp sys_strcmp
#define strcat sys_strcat

#endif

#endif

#ifdef __cplusplus
}
#endif
