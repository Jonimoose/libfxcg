#ifndef _FXCG_UNISTD_H
#define _FXCG_UNISTD_H
typedef int off_t;

#ifdef __cplusplus
extern "C" {
#endif

// This stuff doesn't belong since we're not a UNIX system, but python wants
// it.  Implementations here are mostly wrappers for standard library functions
#include <sys/types.h>

unsigned sleep(unsigned seconds);

char *getlogin(void);

char * getwd(char * p);

int access(const char *path, int amode);

int execv(const char *file, char *const argv[]);

int execve(const char *filename,char *const argv[],char *const envp[]);

int pipe(int fildes[2]);

gid_t getgid(void);

uid_t geteuid(void);

pid_t getppid(void);

pid_t getpid(void);

pid_t fork(void);

ssize_t write(int fildes, const void *buf, size_t nbyte);

off_t lseek(int fd, off_t offset, int whence);

uid_t getuid(void);

gid_t getegid(void);

int open(const char * name,int flags,...);
int isatty(int fd);
int close(int fd);

int chdir(const char *path);

int unlink(const char *path);

int rmdir(const char *path);

int dup(int fd);
int dup2(int fd, int fd2);

// Seek constants in stdio are canonical, these should be the same.
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

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
