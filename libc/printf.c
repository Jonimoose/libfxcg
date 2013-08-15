#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int precision;
    unsigned alternate:1;
    unsigned zeropad:1;
    unsigned leftjustify:1;
    unsigned spacepad:1;
    unsigned sign:1;
} format_t;

typedef void (*writer_t)(const void *, char);
typedef int (*formatter_t)(va_list *, writer_t, const void *, format_t);

/* These are the workers for formatting.  The main printf wrapper gets a writer
 * and destination, and this does the formatting while feeding characters to
 * the writer, passing dest through to the writer. */
static int _printf_do_udecimal(int x, writer_t writer, const void *dest, format_t fmt) {
    int log10 = 0;     // Number of digits neeeded
    int xt = x;
    while (xt > 0) {
        log10++;
        xt /= 10;
    }
    int count = log10;

    do {    // Defer comparison to ensure 0 gets a digit
        int xpow = 1, i;
        for (i = log10; i > 1; i--) {   // 10^log10
            xpow *= 10;
        }
        char digit = (x / xpow) % 10;
        digit += '0';
        writer(dest, digit);
    } while (--log10 > 0);
    return count;
}

static int _printf_udecimal(va_list *ap, writer_t writer, const void *dest, format_t fmt) {
    return _printf_do_udecimal(va_arg(*ap, unsigned), writer, dest, fmt);
}

static int _printf_decimal(va_list *ap, writer_t writer, const void *dest, format_t fmt) {
    int x = va_arg(*ap, int);

    int count = 1;
    if (x < 0) {
        writer(dest, '-');
    } else if (fmt.sign) {
        writer(dest, '+');
    } else if (fmt.spacepad) {
        writer(dest, ' ');
    } else {
        count = 0;
    }

    x = abs(x);
    return count + _printf_do_udecimal(x, writer, dest, fmt);
}

static int _printf_char(va_list *ap, writer_t writer, const void *dest, format_t fmt) {
    char c = va_arg(*ap, int);
    writer(dest, c);
    return 1;
}

static int _printf_string(va_list *ap, writer_t writer, const void *dest, format_t fmt) {
    const char *s = va_arg(*ap, const char *);
    int count = 0;
    char c;
    while ((c = *s++) != 0) {
        writer(dest, c);
        count++;
    }
    return count;
}

int _printf_ptr(va_list *ap, writer_t writer, const void *dest, format_t fmt) {
    writer(dest, '0');
    writer(dest, 'x');
    unsigned x = (unsigned)va_arg(*ap, void *);

    // This will be wrong on non-32-bit platforms
    int i;
    char chars[] = "0123456789ABCDEF";
    for (i = 7; i >= 0; i--) {
        writer(dest, chars[(x >> (4 * i)) & 0xF]);
    }
    return 10;
}

int _printf_weird(va_list *ap, writer_t writer, const void *dest, format_t fmt) {
    // Format out a warning, not actually something
    // Does NOT consume any arguments because we have no way of knowing what
    // size was expected.
    int i;
    for (i = 0; i < 3; i++)
        writer(dest, '!');
    return 3;
}

/* Character writers.  Pass a destination (void * interpreted as needed), eg
 * a buffer or stream. */
void _writer_stream(const void *wdest, char c) {
    fputc(c, (FILE *)wdest);
}
void _writer_buffer(const void *wdest, char c) {
    // Needs to track buffer location, so double pointer
    char **dest = (char **)wdest;
    **dest = c;
    (*dest)++;
}

/* Main worker and such. */

// Used for catching end-of-string while grabbing chars
// Used within switch blocks, so can't break.
#define _NEXT(fmt) if ((c = *fmt++) == 0) goto out;

// TODO mostly incomplete.  Python parser only depends on %d and %s, so this is
// mostly placeholder code.
static int _v_printf(const char *fmt, va_list ap_in, writer_t writer, const void
        *warg) {
    // The va_copy here is dumb, but necessary for things to work on x86_64.
    // See GCC bug #14557, but the issue boils down to va_list being
    // transparently passed by reference so &ap is wrong if ap is a function
    // parameter.
    va_list ap;
    va_copy(ap, ap_in);

    // Bytes transmitted
    int count = 0;
    format_t f = {
        .width = 0,
        .precision = 0,
        .alternate = 0,
        .zeropad = 0,
        .leftjustify = 0,
        .spacepad = 0,
        .sign = 0,
    };

    char c;
    // _NEXT returns on end of string.
    while (1) {
        _NEXT(fmt);
        if (c != '%') {     // Literal character
            writer(warg, c);
            count++;
            continue;
        }
        _NEXT(fmt);
        if (c == '%') {     // Literal %
            writer(warg, c);
            count++;
            continue;
        }

        // Flags
        char getflags = 0;
        do {
            switch (c) {
                case '#':   // Alternate form
                    f.alternate = 1;
                    _NEXT(fmt);
                    break;
                case '0':   // Zero-pad
                    f.zeropad = 1;
                    _NEXT(fmt);
                    break;
                case '-':   // Left-justify
                    f.leftjustify = 1;
                    _NEXT(fmt);
                    break;
                case ' ':   // Pad positive sign with space
                    f.spacepad = 1;
                    _NEXT(fmt);
                    break;
                case '+':   // Force sign
                    f.sign = 1;
                    _NEXT(fmt);
                    break;
                default:
                    getflags = 0;
            }
        } while (getflags);

        // Field width
        if (isdigit(c) && c != '0') {   // From string
            f.width = (int)strtol(fmt - 1, (char **)(&fmt), 10);   // Updates fmt
            _NEXT(fmt);
        } else if (c == '*') {          // From arg
            f.width = va_arg(ap, int);
            _NEXT(fmt);
        }

        // Precision
        if (c == '.') {
            _NEXT(fmt);
            if (c == '*') { // From arg
                f.precision = va_arg(ap, int);
                _NEXT(fmt);
            } else {        // From string
                f.precision = (int)strtol(fmt, (char **)(&fmt), 10); // Updates fmt
            }
            // Negative precision should be ignored
            if (f.precision < 0)
                f.precision = 0;
        }

        // Argument width
        switch (c) {
            case 'h':
                _NEXT(fmt);
                if (c == 'h') { // "hh" char
                    _NEXT(fmt);
                } else {        // short
                }
                break;
            case 'l':
                _NEXT(fmt);
                if (c == 'l') { // "ll" long long
                    _NEXT(fmt);
                } else {        // long
                }
                break;
            case 'j':   // intmax_t
                _NEXT(fmt);
                break;
            case 'z':   // size_t
                _NEXT(fmt);
                break;
            case 't':   // ptrdiff_t
                _NEXT(fmt);
                break;
            case 'L':   // long double (unsupported)
            default:    // Not a length modifier
                break;
        }

        // Conversion
        formatter_t formatter;
        switch (c) {
            case 'd':
            case 'i':   // decimal
                formatter = _printf_decimal;
                break;
            case 'u':
                formatter = _printf_udecimal;
                break;
            case 'c':
                formatter = _printf_char;
                break;
            case 's':
                formatter = _printf_string;
                break;
            case 'o':
                formatter = _printf_weird;
                break;
            case 'x':
            case 'X':
                formatter = _printf_weird;
                break;
            case 'p':
                formatter = _printf_ptr;
                break;
            case 'f':   // All (currently) unsupported
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
            case 'a':
            case 'A':
            case 'C':
            case 'S':
            case 'n':   // Probably needs to be a special case
            default:
                formatter = _printf_weird;
                break;
        }
        formatter(&ap, writer, warg, f);
    }
out:
    va_end(ap);
    return count;
}

int vfprintf(FILE *stream, const char *fmt, va_list ap) {
    return _v_printf(fmt, ap, _writer_stream, stream);
}

int fprintf(FILE *stream, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vfprintf(stream, fmt, ap);
    va_end(ap);
    return ret;
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vfprintf(stdout, fmt, ap);
    va_end(ap);
    return ret;
}

int vsprintf(char *str, const char *fmt, va_list ap) {
    return _v_printf(fmt, ap, _writer_buffer, &str);
}

int sprintf(char *str, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(str, fmt, ap);
    va_end(ap);
    return ret;
}
