#include <stdlib.h>
#include <string.h>

char *strcat(char *dest, const char *src) {
	dest += strlen(dest);
	return strcpy(dest, src);
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

char *strcpy(char *s1, const char *s2) {
    while ((*s1++ = *s2++) != 0);
    return s1;
}

size_t strcspn(const char *s1, const char *s2) {
	int i, j;
	int l1, l2;
	l1 = strlen(s1);
	l2 = strlen(s2);
	for (i = 0; i < l1; i++) {
		for (j = 0; j < l2; j++) {
			if (s1[i] == s2[j]) { return i; }
		}
	}

	return l1;
}

char *strdup(const char *s) {
    char *o = malloc(strlen(s) + 1);
    strcpy(o, s);
    return o;
}

char *strncat(char *dest, const char *src, size_t num) {
	dest += strlen(dest);
	return strncpy(dest, src, num);
}

char *strncpy(char *dest, const char *src, size_t num) {
	int i;
	int l = strlen(src);
	for (i = 0; i < num; i++) {
		dest[i] = i <= l ? src[i] : 0;
	}

	return dest;
}