#include <sys/stat.h>
#include <fxcg/file.h>
#include <stdio.h>
typedef struct{
	unsigned short id, type;
	unsigned long fsize, dsize;
	unsigned int property;
	unsigned long address;
}file_type_t;
int stat(const char *path, struct stat *buf){
	fprintf(stderr,"Stat not yet supported %s\n",path);
	int Fh;
	unsigned int flen=strlen(path);
	unsigned short * strC=(unsigned short *)alloca(flen*2+2);
	Bfile_StrToName_ncpy(strC,path,flen+1);
	return -1;
}
