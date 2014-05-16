#ifndef _FXCG_STRING_H
#define _FXCG_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

char *strerror(int errnum);

void *sys_memmove(void *dest, const void *src, size_t n);

void *memsetZero(void *s, size_t n);

void *memccpy(void *s1, const void *s2, int c, size_t n);
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, unsigned int n);
void *memcpy(void *dest, const void *source, unsigned int n);
void *memmove(void *s1, const void *s2, size_t n);
void *memset(void *dest, int c, unsigned int n);

char *strcat(char *s1, const char *s2);
char *strchr(const char *s, int c);
int strcmp(const char*, const char*);
int strcoll(const char *s1, const char *s2);
char *strcpy(char *s1, const char *s2);
size_t strcspn(const char *s1, const char *s2);
char *strdup(const char *s1);
/*char *strerror(int errnum);*/
size_t strlen(const char *s);
char *strncat(char *s1, const char *s2, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *s1, const char *s2, size_t n);
char *strpbrk(const char *s1, const char *s2);
char *strrchr(const char *s, int c);
size_t strspn(const char *s1, const char *s2);
char *strstr(const char *needle, const char *haystack);
char *strtok(char *s, const char *sep);
/*char *strtok_r(char *s, const char *sep, char **lasts);*/
size_t strxfrm(char *s1, const char *s2, size_t n);

char *sys_strcat(char *dest, const char *src);
char *sys_strchr(const char *s, int c);
int sys_strcmp(const char *s1, const char *s2);
char *sys_strcpy(char *dest, const char *src);
char *sys_strncpy(char *dest, const char *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif
