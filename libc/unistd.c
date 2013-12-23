#include <stddef.h>
#include <unistd.h>

/*
This is a bogus implementation of UNIX descriptor tables, which describe to
the system what resources a process has open.  In order for dup() and friends
to operate normally, we need a descriptor table to map resource IDs to system
handles. These functions handle that mapping.

Use these when throwing around files, mostly.  See their usage in fopen and
fclose.
*/
#define urandom_FD 1
int open(const char * name,int flags,...){
	if(strcmp(name,"/dev/urandom")==0){
		sys_srand(RTC_GetTicks());
		return urandom_FD;
	}else{
		//Display message on screen informing user that file does not exists
		int x,y;
		x=y=0;
		PrintMini(&x,&y,"Error not supported",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
		x=0;
		y=16;
		PrintMini(&x,&y,name,0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
		x=0;
		y=32;
		PrintMini(&x,&y,"Press any key to continue",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
		GetKey(&x);
		return -1;
	}
}
int isatty(int fd){
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
