typedef int off_t;
#ifndef _FXCG_UNISTD_H
#define _FXCG_UNISTD_H

#ifdef __cplusplus
extern "C" {
#endif

// This stuff doesn't belong since we're not a UNIX system, but python wants
// it.  Implementations here are mostly wrappers for standard library functions
#include <sys/types.h>
int open(const char * name,int flags,...);
int isatty(int fd);
int close(int fd);

int dup(int fd);
int dup2(int fd, int fd2);

// Seek constants in stdio are canonical, these should be the same.
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

size_t write(int fd, const void *buf, size_t n);
off_t lseek(int fd, off_t offset, int whence);

#ifdef _FXCG_LIBFXCG_INTERNAL
// Guts for descriptor tables.  Nasty UNIX emulation. :(
int _dtable_register(int d);
void _dtable_unregister(int d);
#endif

#ifdef __cplusplus
}
#endif

#endif
