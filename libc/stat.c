#include <sys/stat.h>
#include <fxcg/file.h>
#include <stdio.h>
#include <alloca.h>
#include <errno.h>
struct file_type{
	unsigned short id, type;
	unsigned long fsize, dsize;
	unsigned int property;
	unsigned long address;
};
int stat(const char *path, struct stat *buf){
	//Emulate unix paths convert slashes to backslashes and add \\fls0\ to filename
	char found[266];
	int plen = strlen(path);
	plen+=7;
	char * fpath=alloca(plen+1);
	strcpy(fpath,"\\\\fls0\\");
	strcpy(fpath+7,path);
	int i;
	for(i=7;i<plen;++i){
		if(fpath[i]=='/')
			fpath[i]='\\';
	}
	printf("stat %s\n",fpath);
	unsigned short * strC=(unsigned short *)alloca(plen*2+2);
	Bfile_StrToName_ncpy(strC,fpath,plen+1);
	struct file_type info;
	int handle,ret;
	ret = Bfile_FindFirst(strC, &handle, found, &info);
	if(!ret){
		printf("found %d %d %d %d %d %d\n",info.id,info.type,info.fsize,info.dsize,info.property,info.address);
		memset(buf,0,sizeof(struct stat));
		buf->st_size=info.fsize;
		buf->st_blocks=info.fsize/512;
		if(info.fsize)
			buf->st_mode=S_IFREG;
		else
			buf->st_mode=S_IFDIR;
		return 0;
	}else if(ret==-16){
		fputs("not found",stderr);
		errno=ENOENT;
	}else{
		fprintf(stderr,"error %d\n",ret);
		errno=EIO;
	}
	Bfile_FindClose(handle);
	return (ret==0)?0:-1;
}
int fstat(int fd, struct stat *buf){
	fprintf(stderr,"fstat not yet supported %d\n",fd);
	return -1;
}
int lstat(const char * path, struct stat * buf){
	return stat(path,buf);
}
mode_t umask(mode_t cmask){
	fprintf(stderr,"unmask %d\n",cmask);
	return -1;
}
int chmod(const char *path, mode_t mode){
	fprintf(stderr,"chmod %s %d\n",path,mode);
	return -1;
}
