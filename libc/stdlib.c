#include <fxcg/heap.h>
#include <fxcg/keyboard.h>
#ifndef STDERR_TO_VRAM
#include <fxcg/display.h>
#endif
#include <ctype.h>
#include <limits.h>
#include <stdio.h>

// External linkage
int errno;

__attribute__((weak)) void free(void *p) { sys_free(p); }
__attribute__((weak)) void *malloc(size_t sz) { return sys_malloc(sz); }
__attribute__((weak)) void *realloc(void *p, size_t sz) { return sys_realloc(p, sz); }
__attribute__((weak)) void *calloc(int elements, int elementsize) { return sys_calloc(elements, elementsize); }

void exit(int status) {
    /* TODO: if necessary, perform cleanup here, and call functions
     * registered to be run at exit with a future implementation of
     * atexit.
     * Idea for future atexit development: syscall SetQuitHandler
     * (0x1E6E) could be used to always call exit() when leaving the
     * add-in, so that atexit-registered functions always run on exit.
     * Add-ins that wished to disable this behavior could always call
     * SetQuitHandler with a pointer to their own (possibly nop)
     * function.
     */
    fprintf(stderr, "TERMINATED (%i)\nPress menu key to exit\n", status);
#ifndef STDERR_TO_VRAM
    /* Initialize the status area so that it can display text
     * (the user code may have set the flags in some other way, or
     * disabled the status area entirely)
     */
    EnableStatusArea(0);
    DefineStatusAreaFlags(3, SAF_BATTERY | SAF_TEXT | SAF_GLYPH | SAF_ALPHA_SHIFT, 0, 0);
    char buffer[50];
    sprintf(buffer, "Exited (%i), press [MENU].", status);
    DefineStatusMessage(buffer, 1, 0, 0);
    DisplayStatusArea(); /* not sure if necessary, I have the idea that,
     * at least in some circumstances, GetKey calls DisplayStatusArea().
     */
#endif
    int key;
    while(1)
        GetKey(&key);
}

void abort() {
    fprintf(stderr, "ABORT CALLED\nPress menu key to exit\n");
#ifndef STDERR_TO_VRAM
    /* Initialize the status area so that it can display text
     * (the user code may have set the flags in some other way, or
     * disabled the status area entirely)
     */
    EnableStatusArea(0);
    DefineStatusAreaFlags(3, SAF_BATTERY | SAF_TEXT | SAF_GLYPH | SAF_ALPHA_SHIFT, 0, 0);
    DefineStatusMessage((char*)"Aborted, press [MENU].", 1, 0, 0);
    DisplayStatusArea(); /* not sure if necessary, I have the idea that,
     * at least in some circumstances, GetKey calls DisplayStatusArea().
     */
#endif
    int key;
    while(1)
        GetKey(&key);
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

int abs(int i) {
    if (i < 0)
        return -i;
    else
        return i;
}
