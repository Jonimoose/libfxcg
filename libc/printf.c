#include <stdarg.h>
#include <stdio.h>

/* These are the workers for formatting.  The main printf wrapper gets a writer
 * and destination, and this does the formatting while feeding characters to
 * the writer, passing wdest to the writer. */
int _printf_decimal(va_list ap, void (*writer)(const void *, char), const void *wdest, int width, int precision) {
#error decimal worker incomplete
}
int _printf_string(va_list ap, void (*writer)(const void *, char), const void *wdest, int width, int precision) {
    const char *s = va_arg(ap, const char *);
    int count = 0;
    char c;
    while ((c = *s++) != 0) {
        writer(wdest, c);
        count++;
    }
}
int _printf_weird(va_list ap, void (*writer)(const void *, char), const void *wdest, int width, int precision) {
    // Format out a warning, not actually something
}

/* Character writers.  Pass a destination (void * interpreted as needed), eg
 * a buffer or stream. */
void _writer_stream(void *wdest, char c) {
    fputc(c, (FILE *)wdest);
}
void _writer_buffer(void *wdest, char c) {
    // Needs to track buffer location, so double pointer
    char **dest = (char **)wdest;
    **dest = c;
    *dest++;
}

/* Main worker and such. */

// Used for catching end-of-string while grabbing chars
// Used within switch blocks, so can't break.
#define _NEXT(fmt) if ((c = *fmt++) == 0) goto _term

// TODO mostly incomplete.  Python parser only depends on %d and %s, so this
// is mostly placeholder code.
int vfprintf(FILE *stream, const char *fmt, va_list ap) {
#error Must push vfprintf guts out into new worker per generic functions above.
    // Bytes transmitted
    int count = 0;
    // -1 indicates default, whatever that ends up being for a handler
    int width = -1;
    int precision = -1;

    while (1) {     // Tad ugly.  Meh.
        _NEXT(fmt);
        if (c != '%') {
            fputc(c, stream);
            count++;
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
        if (isdigit(c) && c != '0') {   // From string
            width = (int)strtol(fmt - 1, &fmt, 10);
            _NEXT(fmt);
        } else if (c == '*') {          // From arg
            width = va_arg(ap, int);
            _NEXT(fmt);
        }
        // Precision
        if (c == '.') {
            _NEXT(fmt);
            if (c == '*') { // From arg
                precision = va_arg(ap, int);
                _NEXT(fmt);
            } else {        // From string
                precision = (int)strtol(fmt, &fmt, 10);
            }
            // Negative precision should be ignored
            precision = max(0, precision);
        }
        // Argument width
        switch (c) {
            case 'h':
                _NEXT(fmt);
                if (c == 'h') { // "hh" char
                    _NEXT(fmt);
                } else {        // short
                    ;
                }
                break;
            case 'l':
                _NEXT(fmt);
                if (c == 'l') { // "ll" long long
                    _NEXT(fmt);
                } else {        // long
                    ;
                }
                break;
            case 'j':   // intmax_t
            case 'z':   // size_t
            case 't':   // ptrdiff_t
            case 'L':   // long double
                _NEXT(fmt);
            default:    // Not a length modifier
                break;
        }
        // Conversion
        switch (c) {
            case 'd':
            case 'i':   // decimal
                count += _printf_decimal(ap, wdest, width, precision);
                break;
            case 's':
                count += _printf_string(ap, wdest, width, precision);
                break;
            default:
                count += _printf_weird(ap, wdest, width, precision);
                break;
        }
    }
_term:  // cf _NEXT macro
    return count;
}

int fprintf(FILE *stream, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vfprintf(stream, fmt, ap);
    va_end(ap);
    return ret;
}

