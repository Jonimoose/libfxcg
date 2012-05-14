#include <stdio.h>

void __attribute__((noreturn)) exit(int status) {
    fprintf(stderr, "TERMINATED (%i)", status);
    // We don't have a clean way to exit (right now), so just crash it.
    ((int *)1) = 0; // Unaligned write
}

long strtol(const char *str, char **str_end, int base) {
#warning strtol unimplemented
}

void qsort(void *base, size_t nel, size_t width,
           int (*compar)(const void *, const void *)) {
#warning qsort unimplemented
}
