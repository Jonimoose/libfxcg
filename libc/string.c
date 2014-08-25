#include <stdlib.h>
#include <string.h>

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
int memcmp(const void* s1, const void* s2, unsigned int n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

// GCC builtin
/*
void* memcpy(void* destination, const void* source, size_t num) {
	char* d = (char*)destination;
	char* s = (char*)source;
	while (num-- > 0) {
		*d = *s;
		d++;
		s++;
	}

	return destination;
}
*/

void *memmove(void *dst, const void *src, size_t count){ 
  //From dietlibc 
  char *a = dst; 
  const char *b = src; 
  if (src!=dst){ 
    if (src>dst){ 
        while (count--) *a++ = *b++; 
    }else{ 
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
