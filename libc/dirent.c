#include <dirent.h>
#include <stdio.h>
DIR	*opendir(const char * dir){
	fprintf(stderr,"opendir not supported %s\n",dir);
	return 0;
}
DIR	*fdopendir(int fd){
	fprintf(stderr,"fdopendir not supported %d\n",fd);
	return 0;
}
struct dirent *readdir(DIR *dirp){
	fputs("readdir\n",stderr);
	return 0;
}
int closedir(DIR *dirp){
	fprintf("closedir\n",stderr);
	return 0;
}
