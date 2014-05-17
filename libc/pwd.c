#include <pwd.h>
#include <stdio.h>
static void WaitKey(void){
	puts("Press any key to continue");
	int key;
	GetKey(&key);
}
struct passwd *getpwnam(const char *name){
	fprintf(stderr,"getpwnam %s\n",name);
	WaitKey();
	return 0;
}
struct passwd *getpwuid(uid_t uid){
	fprintf(stderr,"getpwuid %d\n",uid);
	WaitKey();
	return 0;
}
