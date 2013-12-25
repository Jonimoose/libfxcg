//This is just a hack for urandom support for now
#ifndef __FXCG_FCNTL
#define __FXCG_FCNTL
#include <stdint.h>
#include <stddef.h>
#define O_EXCL 2
#define O_CREAT 4
#define O_TRUNC 8
#define O_APPEND 16
#define O_RDONLY 32
#define O_WRONLY 64
#define O_RDWR 128

#define F_GETFL 1
#define F_DUPFD 2
ssize_t read(int fd,void * buffer,size_t n);
int fcntl(int descriptor,int command, ...);
#endif
