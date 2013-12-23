#ifndef _FXCG_STDIO_H
#define _FXCG_STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <font6x8ext.h>
#include <stdarg.h>

#define EOF (-1)

#define PATH_MAX 1024 //needed for python


typedef struct {
    /* Usual values for stdin and friends, everything else is the handle
     * from the system +3 */
    unsigned fileno;
    unsigned error : 1;
    unsigned eof : 1;
    /* Just for rewinding the stream. */
    unsigned has_unput : 1;
    unsigned char unput;
    /* Used only for terminal output (on stdout) */
} FILE;
extern int termx,termy;
extern FILE _impl_stdin, _impl_stdout, _impl_stderr;
#define stdin (&_impl_stdin)
#define stdout (&_impl_stdout)
#define stderr (&_impl_stderr)

#define BUFSIZ 1

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define FILENAME_MAX 16
#define FOPEN_MAX (1 << (sizeof(FILE) - 1))

FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
int fclose(FILE *fp);

size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream);
int fflush(FILE *stream);
int ferror(FILE *stream);
int fileno(FILE *stream);

void clearerr(FILE *stream);
int feof(FILE *stream);
int fileno(FILE *stream);
#define getc fgetc
int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int getchar(void);
char *gets(char *s);
int ungetc(int c, FILE *stream);

int fputc(int c, FILE *stream);
#define putc fputc
int putchar(int c);
int puts(const char *s);

int fseek(FILE *f, long offset, int whence);
long ftell(FILE *f);

int printf(const char *fmt, ...);
int sprintf(char *dest, const char *fmt, ...);
int fprintf(FILE *stream, const char *fmt, ...);
int vprintf(const char * format, va_list arg);
int vfprintf(FILE *stream, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
