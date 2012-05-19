#include <stdarg.h>
#include <stdio.h>

// Used for catching end-of-string while grabbing chars
#define _NEXT(fmt) if ((c = *fmt++) == 0) break

int vfprintf(FILE *stream, const char *fmt, va_list ap) {
    while (1) {     // Tad ugly.  Meh.
        _NEXT(fmt);
        if (c != '%') {
            fputc(c, stream);
            continue;
        }
        // Literal %
        _NEXT(fmt);
        if (c == '%') {
            fputc(c, stream);
            continue;
        }
        // Flags
        switch (c) {
        case '#':   // Alternate form
        case '0':   // Zero-pad
        case '-':   // Left-justify
        case ' ':   // Pad positive sign with space
        case '+':   // Force sign
            _NEXT(fmt);
            break;
        default:
            break;  // Leave c untouched
        }
        // Field width
        if (isdigit(c) && c != '0') {
            unsigned width = (unsigned)strtol(fmt - 1, &fmt, 10);
            _NEXT(fmt);
        }
#error printf not finished :(
    }
}

int fprintf(FILE *stream, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vfprintf(stream, fmt, ap);
    va_end(ap);
    return ret;
}

