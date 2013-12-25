#include <pwd.h>
#include <stdio.h>
struct passwd *getpwnam(const char *name){
	fputs("getpwnam\n",stderr);
	return 0;
}
struct passwd *getpwuid(uid_t uid){
	fputs("getpwuid\n",stderr);
	return 0;
}
