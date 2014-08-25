#include <fxcg/heap.h>
#include <fxcg/keyboard.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>

// External linkage
int errno;

void *malloc(size_t sz) {
    return sys_malloc(sz);
}

void *realloc(void *ptr, size_t sz) {
    return sys_realloc(ptr, sz);
}

void free(void *ptr) {
    sys_free(ptr);
}

void exit(int status) { 
   fprintf(stderr, "TERMINATED (%i)\nPress menu key to exit\n", status); 
   int key; 
   while(1) 
      GetKey(&key); 
}

#if __WORDSIZE == 64 
#define ABS_LONG_MIN 9223372036854775808UL 
#else 
#define ABS_LONG_MIN 2147483648UL 
#endif 
long int strtol(const char *nptr, char **endptr, int base) {
    //From dietlibc
    int neg=0;
    unsigned long int v;
    const char*orig=nptr;

    while(isspace(*nptr)) nptr++;

    if (*nptr == '-' && isalnum(nptr[1])) { neg=-1; ++nptr; }
    v=strtoul(nptr,endptr,base);
    if (endptr && *endptr==nptr) *endptr=(char *)orig;
    if (v>=ABS_LONG_MIN) {
        if (v==ABS_LONG_MIN && neg) {
            errno=0;
            return v;
        }
        errno=ERANGE;
        return (neg?LONG_MIN:LONG_MAX);
    }
    return (neg?-v:v);
}

double strtod(const char *s, char **str_end) {
    // TODO handle exponential format, hex format, inf, nan
    double r = 0.0;
    int negative = 0;
    if (!isdigit(*s) && *s != '-' && *s != '+' && *s != '.') {
        if (str_end != NULL)
            *str_end = (char *)s;
        return 0;
    }

    switch (*s)
    {
        case '-':
            negative = 1;
            // Deliberate fallthrough
        case '+':
            s++;
            break;
    }

    while (isdigit(*s))
    {
        r *= 10.0;
        r += *s++ - '0';
    }
    if (*s == '.')
    {
        float f = 10.0f;
        s++;
        while (isdigit(*s))
        {
            r += (*s - '0')/f;
            f *= 10.0f;
            s++;
        }
    }

    if (str_end != NULL)
        *str_end = (char *)s;

    // Portable? Nope. Fast? Yup.
    union {
        double r;
        unsigned long long l;
    } raw;
    raw.r = r;
    raw.l |= (unsigned long long)negative << 63;
    return raw.r;
}

/*
void qsort(void *base, size_t nel, size_t width,
           int (*compar)(const void *, const void *)) {
#warning qsort unimplemented
}
*/
