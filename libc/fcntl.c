#include <fcntl.h>
#define urandom_FD 1
ssize_t read(int fd,void * buffer,size_t n){
	if(fd==urandom_FD){
		size_t c=n;
		unsigned int * d=(unsigned int *)buffer;
		while (c>=4){
			*d++=sys_rand();
			c-=4;
		}
		unsigned char * r=(unsigned char *)d;//remaindor
		while(c--){
			*r++=sys_rand();
		}
		return n;
	}else
		return -1;
}
int fcntl(int descriptor,int command, ...){
	return -1;
}
