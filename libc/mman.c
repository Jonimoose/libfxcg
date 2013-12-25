#include <fxcg/fddefs.h>
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off){
	if(fildes==frameBuf_FD){
		return (unsigned char *)0xA8000000+off;
	}else{
		fprintf(stderr,"mmap is only emulation %d %d %d %d %d %d\n",addr,len,prot,flags,fildes,off);
		return MAP_FAILED;
	}
}
int munmap(void *addr, size_t len){
	return 0;
}
