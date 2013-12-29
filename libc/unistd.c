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
const char * userNamelogin="CasioNerd";
char *getlogin(void){
	return userNamelogin;
}

unsigned sleep(unsigned seconds){
	//TODO put cpu in lower power mode
	int a=RTC_GetTicks()+(seconds*128);// returns 1/128 seconds
	while(a>RTC_GetTicks());
	return 0;
}

char * getwd(char * p){
	return 0;
}
static void waitKey(void){
	puts("Press any key to continue");
	int key;
	GetKey(key);
}
int access(const char *path, int amode){
	fprintf(stderr,"acess %s %d\n",path,amode);
	waitKey();
	return -1;
}

int execv(const char *file, char *const argv[]){
	fprintf(stderr,"execv not supported %s\n",file);
	waitKey();
	return -1;
}

int execve(const char *filename,char *const argv[],char *const envp[]){
	fprintf(stderr,"execve not supported %s\n",filename);
	waitKey();
	return -1;
}

int pipe(int fildes[2]){
	fputs("pipe\n",stderr);
	return -1;
}

gid_t getgid(void){
	return 0;
}

uid_t geteuid(void){
	return 0;
}

pid_t getppid(void){
	return 0;
}

pid_t getpid(void){
	return 0;
}

pid_t fork(void){
	fputs("fork\n",stderr);
	return -1;
}

ssize_t write(int fildes, const void *buf, size_t nbyte){
	if(fildes==2){
		return fwrite(buf,1,nbyte,stderr);
	}else if (fildes == 1) {
		// stdout: display
		return fwrite(buf,1,nbyte,stdout);
	}else
		return -1;
}

uid_t getuid(void){
	return 0;
}

gid_t getegid(void){
	return 0;
}

static const char * notYetS=" not supported paramater: ";
const char * stdinNAME="stdin";
const char * stdoutNAME="stdout";
const char * stderrNAME="stderr";
char *ttyname(int fildes){
	switch(fildes){
		case 0:
			return stdinNAME;
			break;
		case 1:
			return stdoutNAME;
			break;
		case 2:
			return stderrNAME;
			break;
		default:
			return 0;
	}
}

int chdir(const char *path){
	fprintf(stderr,"chdir%s%s\n",notYetS,path);
	return -1;
}
int unlink(const char *path){
	fprintf(stderr,"unlink%s%s\n",notYetS,path);
	return -1;
}
int rmdir(const char *path){
	fprintf(stderr,"rmdir%s%s\n",notYetS,path);
	return -1;
}
extern unsigned char * LCDstatefd;
int close(int fd){
	if(fd>5)
		Bfile_CloseFile_OS(toNativeFD(fd));
	return 0;
}
off_t lseek(int fd, off_t offset, int whence){
	if(fd>5){
		int fileno = toNativeFD(toNativeFD(fd));

	switch (whence) {
		case SEEK_CUR:
			offset = Bfile_TellFile_OS(fileno) + offset;
			break;
		case SEEK_END:
			offset = Bfile_GetFileSize_OS(fileno);
			break;
		case SEEK_SET:
			break;
		default:
			return -1;
	}

    // TODO can this fail? Probably.
	Bfile_SeekFile_OS(fileno, (int)offset);
	}else
		return -1;
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
	}else if(fd>5)
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
