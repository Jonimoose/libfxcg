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
#define O_NONBLOCK 256

/* XXX close on exec request; must match UF_EXCLOSE in user.h */
#define	FD_CLOEXEC	1	/* posix */

#define        F_DUPFD                0                /* duplicate file descriptor */
#define        F_GETFD                1                /* get file descriptor flags */
#define        F_SETFD                2                /* set file descriptor flags */
#define        F_GETFL                3                /* get file status flags */
#define        F_SETFL                4                /* set file status flags */
ssize_t read(int fd,void * buffer,size_t n);
int fcntl(int descriptor,int command, ...);
#endif
