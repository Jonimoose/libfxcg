#include <stdlib.h>
#include <string.h>
const char * ironySTR="error strerror is not yet supported";
char *strerror(int errnum){
	return ironySTR;
}

void *memccpy(void *dest, const void *src, int c, size_t num) {
	char* d = (char*)dest;
	char* s = (char*)src;
	while ((*d = *s) != (char)c && num > 0) {
		d++;
		s++;
		num--;
	}

	return num == 0 ? NULL : (void*)(++d);
}

void *memchr(const void *ptr, int c, size_t n) {
	char* s = (char*)ptr;
	while (*s != (char)c && n > 0) {
		s++;
		n--;
	}

	return n == 0 ? NULL : (void*)s;
}

// Syscall is broken. It's sys_memcmp if you prefer.
int memcmp(const void *p1, const void *p2, unsigned int n) {
    char* s1 = (char*)p1;
    char* s2 = (char*)p2;

    while (n-- != 0 && *s1 == *s2) {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

//From dietlibc
/* fast memcpy -- Copyright (C) 2003 Thomas M. Ogrisegg <tom@hi-tek.fnord.at> */
#define UNALIGNED(x,y) (((unsigned long)x & (sizeof (unsigned long)-1)) ^ ((unsigned long)y & (sizeof (unsigned long)-1)))
# define STRALIGN(x) (((unsigned long)x&3)?4-((unsigned long)x&3):0)
void * memcpy (void *dst, const void *src, size_t n){
    void           *res = dst;
    unsigned char  *c1, *c2;

    int             tmp;
    unsigned long  *lx1 = NULL;
    const unsigned long *lx2 = NULL;

    if (!UNALIGNED(dst, src) && n > sizeof(unsigned long)) {

	if ((tmp = STRALIGN(dst))) {
	    c1 = (unsigned char *) dst;
	    c2 = (unsigned char *) src;
	    while (tmp-- && n--)
		*c1++ = *c2++;
	    if (n == (size_t) - 1)
		return (res);
	    dst = c1;
	    src = c2;
	}

	lx1 = (unsigned long *) dst;
	lx2 = (unsigned long *) src;

	for (; n >= sizeof(unsigned long); n -= sizeof(unsigned long))
	    *lx1++ = *lx2++;
    }

    if (n) {
	c1 = (unsigned char *) (lx1?lx1:dst);
	c2 = (unsigned char *) (lx1?lx2:src);
	while (n--)
	    *c1++ = *c2++;
    }

    return (res);
}

void *memmove(void *dst, const void *src, size_t count){
	//From dietlibc
  char *a = dst;
  const char *b = src;
  if (src!=dst)
  {
    if (src>dst)
    {
      while (count--) *a++ = *b++;
    }
    else
    {
      a+=count-1;
      b+=count-1;
      while (count--) *a-- = *b--;
    }
  }
  return dst;
}

void *memset(void *dest, int c, unsigned int n) {
	char* d = (char*)dest;
	while (n-- > 0) { *d++ = (char)c; }
	
	return dest;
}


char *strcat(char *dest, const char *src) {
	char* ret = dest;
	dest += strlen(dest);
	strcpy(dest, src);
	return ret;
}

char *strchr(const char *s, int c) {
	while (*s != (char)c) {
		if (*s == 0)
			return NULL;
		s++;
	}
	return (char *)s;
}

int strcmp(const char *s1, const char *s2) {
	while (*s1 != 0 && *s2 != 0) {
		if (*s1 != *s2)
			break;
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strcoll(const char *s1, const char *s2) {
	return strcmp(s1, s2);
}

char *strcpy(char *s1, const char *s2) {
	char* ret = s1;
	while ((*s1++ = *s2++) != 0);
	return ret;
}

size_t strcspn(const char *s1, const char *s2) {
	char* pbrk = strpbrk(s1, s2);
	return (pbrk == 0) ? strlen(s1) : (size_t)(pbrk-s1);
}

char *strdup(const char *s) {
	char *o = malloc(strlen(s) + 1);
	strcpy(o, s);
	return o;
}

char *strncat(char *dest, const char *src, size_t num) {
	int l = strlen(dest);
	strncpy(dest + l, src, num);
	*(dest + l + num) = 0;
	return dest;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	while (*s1 != 0 && *s2 != 0) {
		n--;
		if (*s1 != *s2 || n == 0)
			break;
		s1++;
		s2++;
	}

	return *s1 - *s2;
}

char *strncpy(char *dest, const char *src, size_t num) {
	int i;
	int l = strlen(src);
	for (i = 0; i < num; i++) {
		dest[i] = (i <= l ? src[i] : 0);
	}

	return dest;
}

char *strpbrk(const char *s1, const char *s2) {
	int j;
	int l2;
	l2 = strlen(s2);
	while (*s1 != 0) {
		for (j = 0; j < l2; j++) {
			if (*s1 == s2[j]) { return (char *)s1; }
		}
		s1++;
	}

	return NULL;
}

char *strrchr(const char *s, int c) {
	int i = strlen(s)-1;
	s += i;
	while (i-- >= 0) {
		if (*s == c) { return (char *)s; }
		s--;
	}

	return NULL;
}

size_t strspn(const char *s1, const char *s2) {
	char *s = (char *)s1;
	while (*s != 0) {
		if (!strchr(s2, *s)) { break; }
		s++;
	}

	return (size_t)(s - s1);
}

char *strstr(const char *needle, const char *haystack) {
	int i;
	int l1 = strlen(needle);
	int l2 = strlen(haystack);
	if (l2 > l1) { return NULL; }

	for (i = 0; i < l1-l2; i++, needle++) {
		if (!strncmp(needle, haystack, l2)) { return (char *)(needle); }
	}

	return NULL;
}

char* _strtoks_;

char *strtok(char *s, const char *sep) {
	if (s != NULL) { _strtoks_ = s; }
	if (*_strtoks_ == 0) { return NULL; }

	char *start = _strtoks_ + (sizeof(char) * strspn(_strtoks_, sep));
	_strtoks_ = start + (sizeof(char) * strcspn(start, sep));
	*_strtoks_ = 0;
	_strtoks_++;
	
	return start;
}

size_t strxfrm(char *s1, const char *s2, size_t n) {
	if (s2 != NULL) { strncpy(s1, s2, n); }
	return strlen(s1);
}
