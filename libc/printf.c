/*
 * Standaloneified version of the FreeBSD kernel printf family.
 */

#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNBUF (sizeof(intmax_t) * CHAR_BIT + 1)

char const hex2ascii[] = "0123456789abcdefghijklmnopqrstuvwxyz";

typedef void(kvprintf_fn_t)(int, void *);

static char *ksprintn(char *buf, uintmax_t num, int base, int *len, int upper);
static int kvprintf(char const *fmt, kvprintf_fn_t *func, void *arg, int radix,
                    va_list ap);

static void putchar_wrapper(int cc, void *arg) { putchar(cc); }

int printf(const char *fmt, ...) {
  va_list ap;
  int retval;

  va_start(ap, fmt);
  retval = kvprintf(fmt, putchar_wrapper, NULL, 10, ap);
  va_end(ap);
  return retval;
}

int vprintf(const char *fmt, va_list ap) {
  return (kvprintf(fmt, putchar_wrapper, NULL, 10, ap));
}

int sprintf(char *buf, const char *cfmt, ...) {
  int retval;
  va_list ap;

  va_start(ap, cfmt);
  retval = kvprintf(cfmt, NULL, (void *)buf, 10, ap);
  buf[retval] = '\0';
  va_end(ap);
  return retval;
}

struct print_buf {
  char *buf;
  size_t size;
};

static void snprint_func(int ch, void *arg) {
  struct print_buf *pbuf = arg;

  if (pbuf->size < 2) {
    /*
     * Reserve last buffer position for the terminating
     * character:
     */
    return;
  }
  *(pbuf->buf)++ = ch;
  pbuf->size--;
}

int asprintf(char **buf, const char *cfmt, ...) {
  int retval;
  struct print_buf arg;
  va_list ap;

  *buf = NULL;
  va_start(ap, cfmt);
  retval = kvprintf(cfmt, NULL, NULL, 10, ap);
  va_end(ap);
  if (retval <= 0)
    return (-1);

  arg.size = retval + 1;
  arg.buf = *buf = malloc(arg.size);
  if (*buf == NULL)
    return (-1);

  va_start(ap, cfmt);
  retval = kvprintf(cfmt, &snprint_func, &arg, 10, ap);
  va_end(ap);

  if (arg.size >= 1)
    *(arg.buf)++ = 0;
  return (retval);
}

int snprintf(char *buf, size_t size, const char *cfmt, ...) {
  int retval;
  va_list ap;
  struct print_buf arg;

  arg.buf = buf;
  arg.size = size;

  va_start(ap, cfmt);
  retval = kvprintf(cfmt, &snprint_func, &arg, 10, ap);
  va_end(ap);

  if (arg.size >= 1)
    *(arg.buf)++ = 0;
  return retval;
}

int vsnprintf(char *buf, size_t size, const char *cfmt, va_list ap) {
  struct print_buf arg;
  int retval;

  arg.buf = buf;
  arg.size = size;

  retval = kvprintf(cfmt, &snprint_func, &arg, 10, ap);

  if (arg.size >= 1)
    *(arg.buf)++ = 0;

  return (retval);
}

int vsprintf(char *buf, const char *cfmt, va_list ap) {
  int retval;

  retval = kvprintf(cfmt, NULL, (void *)buf, 10, ap);
  buf[retval] = '\0';

  return (retval);
}

/*
 * Put a NUL-terminated ASCII number (base <= 36) in a buffer in reverse
 * order; return an optional length and a pointer to the last character
 * written in the buffer (i.e., the first character of the string).
 * The buffer pointed to by `nbuf' must have length >= MAXNBUF.
 */
static char *ksprintn(char *nbuf, uintmax_t num, int base, int *lenp,
                      int upper) {
  char *p, c;

  p = nbuf;
  *p = '\0';
  do {
    c = hex2ascii[num % base];
    *++p = upper ? toupper(c) : c;
  } while (num /= base);
  if (lenp)
    *lenp = p - nbuf;
  return (p);
}

/*
 * Scaled down version of printf(3).
 *
 * Two additional formats:
 *
 * The format %b is supported to decode error registers.
 * Its usage is:
 *
 *	printf("reg=%b\n", regval, "<base><arg>*");
 *
 * where <base> is the output base expressed as a control character, e.g.
 * \10 gives octal; \20 gives hex.  Each arg is a sequence of characters,
 * the first of which gives the bit number to be inspected (origin 1), and
 * the next characters (up to a control character, i.e. a character <= 32),
 * give the name of the register.  Thus:
 *
 *	kvprintf("reg=%b\n", 3, "\10\2BITTWO\1BITONE");
 *
 * would produce output:
 *
 *	reg=3<BITTWO,BITONE>
 *
 * XXX:  %D  -- Hexdump, takes pointer and separator string:
 *		("%6D", ptr, ":")   -> XX:XX:XX:XX:XX:XX
 *		("%*D", len, ptr, " " -> XX XX XX XX ...
 */
static int kvprintf(char const *fmt, kvprintf_fn_t *func, void *arg, int radix,
                    va_list ap) {
#define PCHAR(c)                                                               \
  {                                                                            \
    int cc = (c);                                                              \
                                                                               \
    if (func) {                                                                \
      (*func)(cc, arg);                                                        \
    } else if (d != NULL) {                                                    \
      *d++ = cc;                                                               \
    }                                                                          \
    retval++;                                                                  \
  }

  char nbuf[MAXNBUF];
  char *d;
  const char *p, *percent;
  int ch, n;
  uintmax_t num;
  int base, lflag, tmp, width, ladjust, sharpflag, neg, sign, dot;
  int cflag, hflag;
#ifndef PRINTF_MINIMAL
  int jflag, qflag, tflag, zflag;
  uint16_t *S;
  unsigned char *up;
  const char *q;
#endif
  int dwidth, upper;
  char padc;
  int stop = 0, retval = 0;

  num = 0;
  if (!func)
    d = (char *)arg;
  else
    d = NULL;

  if (fmt == NULL)
    fmt = "(fmt null)\n";

  if (radix < 2 || radix > 36)
    radix = 10;

  for (;;) {
    padc = ' ';
    width = 0;
    while ((ch = (unsigned char)*fmt++) != '%' || stop) {
      if (ch == '\0')
        return retval;

      PCHAR(ch);
    }

    percent = fmt - 1;
    lflag = 0;
    ladjust = 0;
    sharpflag = 0;
    neg = 0;
    sign = 0;
    dot = 0;
    dwidth = 0;
    upper = 0;
    cflag = 0;
    hflag = 0;

#ifndef PRINTF_MINIMAL
    qflag = 0;
    jflag = 0;
    tflag = 0;
    zflag = 0;
#endif

  reswitch:
    switch (ch = (unsigned char)*fmt++) {
#ifndef PRINTF_MINIMAL
    case 'b':
      num = (unsigned int)va_arg(ap, int);
      p = va_arg(ap, char *);
      for (q = ksprintn(nbuf, num, *p++, NULL, 0); *q;)
        PCHAR(*q--);

      if (num == 0)
        break;

      for (tmp = 0; *p;) {
        n = *p++;
        if (num & (1 << (n - 1))) {
          PCHAR(tmp ? ',' : '<');
          for (; (n = *p) > ' '; ++p)
            PCHAR(n);
          tmp = 1;
        } else
          for (; *p > ' '; ++p)
            continue;
      }
      if (tmp)
        PCHAR('>');
      break;
    case 'D':
      up = va_arg(ap, unsigned char *);
      p = va_arg(ap, char *);
      if (!width)
        width = 16;
      while (width--) {
        PCHAR(hex2ascii[*up >> 4]);
        PCHAR(hex2ascii[*up & 0x0f]);
        up++;
        if (width)
          for (q = p; *q; q++)
            PCHAR(*q);
      }
      break;
    case 'j':
      jflag = 1;
      goto reswitch;
    case 'n':
      if (jflag)
        *(va_arg(ap, intmax_t *)) = retval;
      else if (qflag)
        *(va_arg(ap, int64_t *)) = retval;
      else if (lflag)
        *(va_arg(ap, long *)) = retval;
      else if (zflag)
        *(va_arg(ap, size_t *)) = retval;
      else if (hflag)
        *(va_arg(ap, short *)) = retval;
      else if (cflag)
        *(va_arg(ap, char *)) = retval;
      else
        *(va_arg(ap, int *)) = retval;
      break;
    case 'q':
      qflag = 1;
      goto reswitch;
    case 'r':
      base = radix;
      if (sign)
        goto handle_sign;
      goto handle_nosign;
    case 'S': /* Assume console can cope with wide chars */
      for (S = va_arg(ap, uint16_t *); *S != 0; S++)
        PCHAR(*S);
      break;
    case 't':
      tflag = 1;
      goto reswitch;
    case 'y':
      base = 16;
      sign = 1;
      goto handle_sign;
    case 'z':
      zflag = 1;
      goto reswitch;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      for (n = 0;; ++fmt) {
        n = n * 10 + ch - '0';
        ch = *fmt;
        if (ch < '0' || ch > '9')
          break;
      }
      if (dot)
        dwidth = n;
      else
        width = n;
      goto reswitch;
#endif
    case '.':
      dot = 1;
      goto reswitch;
    case '#':
      sharpflag = 1;
      goto reswitch;
    case '+':
      sign = 1;
      goto reswitch;
    case '-':
      ladjust = 1;
      goto reswitch;
    case '%':
      PCHAR(ch);
      break;
    case '*':
      if (!dot) {
        width = va_arg(ap, int);
        if (width < 0) {
          ladjust = !ladjust;
          width = -width;
        }
      } else {
        dwidth = va_arg(ap, int);
      }
      goto reswitch;
    case '0':
      if (!dot) {
        padc = '0';
        goto reswitch;
      }
    case 'c':
      PCHAR(va_arg(ap, int));
      break;
    case 'd':
    case 'i':
      base = 10;
      sign = 1;
      goto handle_sign;
    case 'h':
      if (hflag) {
        hflag = 0;
        cflag = 1;
      } else
        hflag = 1;
      goto reswitch;
    case 'l':
      if (lflag) {
        lflag = 0;
#ifndef PRINTF_MINIMAL
        qflag = 1;
#endif
      } else
        lflag = 1;
      goto reswitch;
    case 'o':
      base = 8;
      goto handle_nosign;
    case 'p':
      base = 16;
      sharpflag = (width == 0);
      sign = 0;
      num = (uintptr_t)va_arg(ap, void *);
      goto number;
    case 's':
      p = va_arg(ap, char *);
      if (p == NULL)
        p = "(null)";
      if (!dot)
        n = strlen(p);
      else
        for (n = 0; n < dwidth && p[n]; n++)
          continue;

      width -= n;

      if (!ladjust && width > 0)
        while (width--)
          PCHAR(padc);
      while (n--)
        PCHAR(*p++);
      if (ladjust && width > 0)
        while (width--)
          PCHAR(padc);
      break;
    case 'u':
      base = 10;
      goto handle_nosign;
    case 'X':
      upper = 1;
    case 'x':
      base = 16;
      goto handle_nosign;
    handle_nosign:
      sign = 0;
      if (lflag)
        num = va_arg(ap, unsigned long);
      else if (hflag)
        num = (unsigned short)va_arg(ap, int);
      else if (cflag)
        num = (unsigned char)va_arg(ap, int);
#ifndef PRINTF_MINIMAL
      else if (jflag)
        num = va_arg(ap, uintmax_t);
      else if (qflag)
        num = va_arg(ap, uint64_t);
      else if (tflag)
        num = va_arg(ap, ptrdiff_t);
      else if (zflag)
        num = va_arg(ap, size_t);
#endif
      else
        num = va_arg(ap, unsigned int);
      goto number;
    handle_sign:
      if (lflag)
        num = va_arg(ap, long);
      else if (hflag)
        num = (short)va_arg(ap, int);
      else if (cflag)
        num = (char)va_arg(ap, int);
#ifndef PRINTF_MINIMAL
      else if (jflag)
        num = va_arg(ap, intmax_t);
      else if (qflag)
        num = va_arg(ap, int64_t);
      else if (tflag)
        num = va_arg(ap, ptrdiff_t);
      else if (zflag)
        num = va_arg(ap, int);
#endif
      else
        num = va_arg(ap, int);
    number:
      if (sign && (intmax_t)num < 0) {
        neg = 1;
        num = -(intmax_t)num;
      }
      p = ksprintn(nbuf, num, base, &n, upper);
      tmp = 0;
      if (sharpflag && num != 0) {
        if (base == 8)
          tmp++;
        else if (base == 16)
          tmp += 2;
      }
      if (neg)
        tmp++;

      if (!ladjust && padc == '0')
        dwidth = width - tmp;

      // Get max of dwidth and n
      width -= tmp + dwidth > n ? dwidth : n;
      dwidth -= n;
      if (!ladjust)
        while (width-- > 0)
          PCHAR(' ');
      if (neg)
        PCHAR('-');
      if (sharpflag && num != 0) {
        if (base == 8) {
          PCHAR('0');
        } else if (base == 16) {
          PCHAR('0');
          PCHAR('x');
        }
      }
      while (dwidth-- > 0)
        PCHAR('0');

      while (*p)
        PCHAR(*p--);

      if (ladjust)
        while (width-- > 0)
          PCHAR(' ');

      break;
    default:
      while (percent < fmt)
        PCHAR(*percent++);
      /*
       * Since we ignore a formatting argument it is no
       * longer safe to obey the remaining formatting
       * arguments as the arguments will no longer match
       * the format specs.
       */
      stop = 1;
      break;
    }
  }
#undef PCHAR
}
