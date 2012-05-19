#include <stdlib.h>
#include <string.h>

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

char *strdup(const char *s) {
    char *o = malloc(strlen(s) + 1);
    strcpy(o, s);
    return o;
}
