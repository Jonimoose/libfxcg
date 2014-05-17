#include <fcntl.h>
#include <stdio.h>
#include <fxcg/fddefs.h>
#include <fxcg/file.h>
#include <errno.h>
#include <alloca.h>
#define SYSFILE_MODE_READ 0
#define SYSFILE_MODE_WRITE 2
#define SYSFILE_MODE_READWRITE 3
unsigned char * LCDstatefd;
int open(const char * name,int flags,...){
	if(strcmp(name,"/dev/urandom")==0){
		sys_srand(RTC_GetTicks());
		return urandom_FD;
	}else if(strcmp(name,"/dev/fb0")==0){
		LCDstatefd=(unsigned char *)0xA8000000;
		return frameBuf_FD;
	}else{
		//Display message on screen informing user that file does not exists
		int sysmode = 0;
		if (flags==O_RDONLY) {
			sysmode = SYSFILE_MODE_READ;
		/*} else if (flags==O_WRONLY) {
			sysmode = SYSFILE_MODE_WRITE;
		} else if (flags==O_RDWR) {
			sysmode = SYSFILE_MODE_READWRITE;*/
		} else {
			fprintf(stderr,"Flag combination %d not yet supported\n",flags);
			return -1;
		}
		// Convert string to system native
		size_t plen = strlen(name);
		// We have several potential exits, so alloca simplifies ensuring this
		// gets freed (an early revision of the following logic had memory leaks).
		unsigned short *chars16 = alloca(2 * (plen + 1));
		if (chars16 == NULL) {
			errno = ENOMEM;
			return -1;
		}
		// Get a handle from the system
		Bfile_StrToName_ncpy(chars16, name, plen);
		int syshandle = Bfile_OpenFile_OS(chars16, sysmode, 0);
		if(syshandle>0)
			return fromNativeFD(syshandle);
		else
			return -1;
	}
}
int fcntl(int descriptor,int command, ...){
	fprintf(stderr,"fcntl is not yet suppored %d %d\n",descriptor,command);
	return -1;
}
