#include <fxcg/fddefs.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

/*
This is a bogus implementation of UNIX descriptor tables, which describe to
the system what resources a process has open.  In order for dup() and friends
to operate normally, we need a descriptor table to map resource IDs to system
handles. These functions handle that mapping.

Use these when throwing around files, mostly.  See their usage in fopen and
fclose.
*/
extern unsigned char * LCDstatefd;
int close(int fd){
	if(fd>5)
		Bfile_CloseFile_OS(toNativeFD(fd));
	return 0;
}

ssize_t pread(int fd, void * ptr, size_t num, off_t off){
	if(fd>=5)
		return Bfile_ReadFile_OS(toNativeFD(fd),ptr, num,off);
	else if(fd==frameBuf_FD){
		memcpy(ptr,(unsigned char *)0xA8000000+off,num);
		return num;
	}else if(fd==urandom_FD)
		return read(fd,ptr,num);
	else
		return -1;
}
ssize_t read(int fd,void * buffer,size_t n){
	if(fd==frameBuf_FD){
		memcpy(buffer,LCDstatefd,n);
		LCDstatefd+=n;
		return n;
	}else if(fd==urandom_FD){
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
	}else if(fd==0){
		//printf("Read %d bytes from stdin\n",n);
		inputStrTiny(buffer,n,1);
	}else if(fd>=5)
		return Bfile_ReadFile_OS(toNativeFD(fd),buffer, n,-1);
	else
		return -1;
}

int isatty(int fd){//Yes belive it or not this is most likely a full implentation
	if(fd>=3){
		return 0;
	}else
		return 1;
}
// Returns nonzero for failure
int _dtable_register(int d) {
    return 0;
}

void _dtable_unregister(int d) {

}

// TODO don't always fail
// Python (Parser/tokenizer.c) only uses this to check file encoding, and falls
// back to the default if this call fails.
// Otherwise this is seemingly only useful with fdopen().
int dup(int oldd) {
    return -1;
}
