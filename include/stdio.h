#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <font6x8ext.h>
#include <stdarg.h>

extern int termxfxcg,termyfxcg,termFGfxcg,termBGfxcg;


/*Comment and 3 defines from https://github.com/freebsd/freebsd/blob/0680c1f5fe159bba69a242568af0db7d7dc15a79/include/stdio.h
* The following three definitions are for ANSI C, which took them
* from System V, which brilliantly took internal interface macros and
* made them official arguments to setvbuf(), without renaming them.
* Hence, these ugly _IOxxx names are *supposed* to appear in user code.
*
* Although numbered as their counterparts above, the implementation
* does not rely on this.
*/
#define        _IOFBF        0                /* setvbuf should set fully buffered */
#define        _IOLBF        1                /* setvbuf should set line buffered */
#define        _IONBF        2                /* setvbuf should set unbuffered */

#define        EOF        (-1)

#define PATH_MAX 256 /*needed for python*/

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

void setbuffer(FILE *stream, char *buf, size_t size);

void setbuf(FILE *stream, char *buf);

FILE * freopen (const char * filename, const char * mode, FILE * stream);

char *tmpnam(char *s);

extern FILE _impl_stdin, _impl_stdout, _impl_stderr;

#define STDOUT_FILENO 1

#define stdin (&_impl_stdin)
#define stdout (&_impl_stdout)
#define stderr (&_impl_stderr)

#define BUFSIZ 1

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define FILENAME_MAX 16
#define FOPEN_MAX (1 << (sizeof(FILE) - 1))

FILE *popen(const char *command, const char *type);

void rewind ( FILE * stream );

void clearerr ( FILE * stream );

int pclose(FILE *stream);

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
int snprintf ( char * s, size_t n, const char * format, ... );
int fprintf(FILE *stream, const char *fmt, ...);
int vprintf(const char * format, va_list arg);
int vfprintf(FILE *stream, const char *fmt, va_list ap);
int vsprintf(char *str, const char *fmt, va_list ap);
int vsnprintf (char * s, size_t n, const char * format, va_list arg);
#ifdef __cplusplus
}
#endif

#endif
