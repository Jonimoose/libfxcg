#ifndef _FXCG_UNISTD_H
#define _FXCG_UNISTD_H
// This stuff doesn't belong since we're not a UNIX system, but python wants
// it.  Implementations here are mostly wrappers for standard library functions
#include <sys/types.h>

// Things with file descriptors are actually the canonical versions here, stdio
// wraps these.
int isatty(int fd);
int dup(int fd);
int dup2(int fd, int fd2);

// Seek constants in stdio are canonical, these should be the same.
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

size_t write(int fd, const void *buf, size_t n);
off_t lseek(int fd, off_t offset, int whence);

#endif
