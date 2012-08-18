#include <fxcg/heap.h>
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

// TODO annotate this noreturn
void exit(int status) {
    fprintf(stderr, "TERMINATED (%i)", status);
    // We don't have a clean way to exit (right now), so just crash it.
    ((void (*)())1)(); // Unaligned instruction
}

static unsigned char strtol_consume(unsigned char c, int base) {
    c = toupper(c);
    if (!isalnum(c))
        return -1;

    if ((c - '0') < 10)
        c = c - '0';
    else
        c = (c - 'A') + 10;

    if (c >= base)
        return -1;
    else
        return c;
}

long strtol(const char *str, char **str_end, int base) {
    long v = 0;
    unsigned short v_in;
    // TODO handle {+,-} sign indicators, octal (0), hex (0[Xx]) prefixes
    while (isspace(*str))
        str++;

    while (*str != 0 && (v_in = strtol_consume(*str++, base)) >= 0) {
        long long vc = ((long long)v * base) + v_in;
        if (vc > LONG_MAX)  // Handle overflow
            return LONG_MAX;
        else if (vc < LONG_MIN)
            return LONG_MIN;

        v = (v * base) + v_in;
    }
    if (str_end != NULL)
        *str_end = (char *)str;
    return v;
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
