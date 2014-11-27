#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
int _open(const char*s,int n,...) __attribute__ ((weak, alias ("open")));
int open(const char*s,int n,...){
	errno=ENOSYS;
	return -1;
}
ssize_t	_read(int n,void*ptr,size_t s) __attribute__ ((weak, alias ("read")));
ssize_t	read(int n,void*ptr,size_t s){
	errno=ENOSYS;
	return -1;
}
ssize_t _write(int fildes, const void *buf, size_t nbyte) __attribute__ ((weak, alias ("write")));
ssize_t write(int fildes, const void *buf, size_t nbyte){
	errno=ENOSYS;
	return -1;
}
off_t _lseek(int n, off_t o, int x) __attribute__ ((weak, alias ("lseek")));
off_t lseek(int n, off_t o, int x){
	errno=ENOSYS;
	return -1;
}
int _close(int n) __attribute__ ((weak, alias ("close")));
int close(int n){
	errno=ENOSYS;
	return -1;
}
int _fstat(int fildes, struct stat *buf) __attribute__ ((weak, alias ("fstat")));
int fstat(int fildes, struct stat *buf){
	errno=ENOSYS;
	return -1;
}
int select(int n, fd_set * f, fd_set *s, fd_set *ss, struct timeval *t){
	errno=ENOSYS;
	return -1;
}
int connect(int n, const struct sockaddr * s, socklen_t t){
	errno=ENOSYS;
	return -1;
}
int socket(int domain, int type, int protocol){
	errno=ENOSYS;
	return -1;
}
uid_t getuid(void){
	return 0xCA510;
}
pid_t getpid(void){
	return 0xCA510;
}
int access(const char *path, int amode){
	errno=ENOSYS;
	return -1;
}

