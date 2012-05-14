#ifndef _FXCG_STDIO_H
#define _FXCG_STDIO_H

#include <stddef.h>

#define EOF (-1)

typedef struct {
    /* Usual values for stdin and friends, everything else is the handle
     * from the system +3 */
    unsigned fileno;
    unsigned error : 1;
    /* Just for rewinding the stream. */
    unsigned has_unput : 1;
    unsigned char unput;
} FILE;

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

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream);
int fflush(FILE *stream);
int ferror(FILE *stream);

void clearerr(FILE *stream);
int feof(FILE *stream);
int fileno(FILE *stream);

int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int getc(FILE *stream);
int getchar(void);
char *gets(char *s);
int ungetc(int c, FILE *stream);

int printf(const char *fmt, ...);
int sprintf(char *dest, const char *fmt, ...);
int fprintf(FILE *stream, const char *fmt, ...);

#endif
