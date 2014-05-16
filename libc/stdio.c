#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/heap.h>
#include <fxcg/serial.h>
#include <fxcg/keyboard.h>
#include <alloca.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
int termxfxcg=0;
int termyfxcg=0;
int termFGfxcg=0xFFFF;
int termBGfxcg=0;
// Unspecified members initialized to zero.
FILE _impl_stdin = {0};
FILE _impl_stdout = {1};
FILE _impl_stderr = {2};

#define SYSFILE_MODE_READ 0
#define SYSFILE_MODE_WRITE 2
#define SYSFILE_MODE_READWRITE 3

#define NATIVEOFFSET 6

#define IOERR(stream, err) errno = err; stream->error = 1

void setbuffer(FILE *stream, char *buf, size_t size){
	fprintf(stderr,"setbuffer %d\n",size);
}

void setbuf(FILE *stream, char *buf){
	fputs("setbuf\n",stderr);
}

FILE * freopen (const char * filename, const char * mode, FILE * stream){
	fprintf(stderr,"freopen not yet supported %s %d\n",filename,mode);
	return 0;
}


char *tmpnam(char *s){
	static unsigned int numf;
	itoa(numf,(unsigned char *)s+strlen(s));
	++numf;
	return s;
}


void clearerr ( FILE * stream ){
	stream->error=0;
	stream->eof=0;
}

FILE *popen(const char *command, const char *type){
	fprintf(stderr,"popen %s %s\n",command,type);
	return 0;
}

int pclose(FILE *stream){
	fputs("pclose\n",stderr);
	return -1;
}

static inline int isstdstream(FILE *f) {
    return f->fileno < 3;
}

static inline int handle_tonative(int fileno) {
    return fileno - NATIVEOFFSET;
}

int feof(FILE *stream) {
    return stream->eof;
}

FILE *fopen(const char *path, const char *mode) {
    // Resolve mode
	int sysmode = 0;
	if (mode[0] == 'r') {
		sysmode = SYSFILE_MODE_READ;
    /*} else if (mode[0] == 'w' || mode[0] == 'a') { 
        sysmode = SYSFILE_MODE_WRITE;
    } else if (strchr(mode, '+')) {
        sysmode = SYSFILE_MODE_READWRITE;*/
	} else {
		errno = EINVAL;
		return NULL;
	}
    // Convert string to system native
    //Emulate unix paths convert slashes to backslashes and add \\fls0\ to filename
	size_t plen = strlen(path);
	plen+=7;
	char * fpath=alloca(plen+1);
	strcpy(fpath,"\\\\fls0\\");
	strcpy(fpath+7,path);
	int i;
	for(i=7;i<plen;++i){
		if(fpath[i]=='/')
			fpath[i]='\\';
	}
    // We have several potential exits, so alloca simplifies ensuring this
    // gets freed (an early revision of the following logic had memory leaks).
    unsigned short *chars16 = alloca(2 * (plen + 1));
    if (chars16 == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // Get a handle from the system
    Bfile_StrToName_ncpy(chars16,fpath, plen+1);//+1 to include null terminator
    int syshandle = Bfile_OpenFile_OS(chars16, sysmode, 0);

	if (syshandle < 0) {
		fprintf(stderr,"Cannot open %s\n",path);
		// Doesn't exist for reading.
		errno = ENOENT;
		return NULL;
	}

    if (mode[0] == 'a') {
        // TODO Need to seek to end and do some junk.
        errno = ENOTIMPL;
        return NULL;
    }

    // Create a FILE and fill it in
    FILE *f = sys_malloc(sizeof(FILE));
    if (f == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    memset(f, 0, sizeof(FILE));
    f->fileno = syshandle + NATIVEOFFSET;
    return f;
}

// TODO implement me.
// Same use-case in Python as dup(), so not necessary.  Just fail.
FILE *fdopen(int fd, const char *mode) {
	fprintf(stderr,"fdopen %d\n",fd);
    return NULL;
}

int fclose(FILE *fp) {
    if (!isstdstream(fp)) {
        // TODO Not sure what this can return.  Assuming never fails.
        Bfile_CloseFile_OS(handle_tonative(fp->fileno));
        sys_free(fp);
    } else {
        // std streams can't actually be closed.
    }
    return 0;
}
static const unsigned char modeSerialOpen[6] = {0, 5, 0, 0, 0, 0};    // 9600 bps 8n1

static int serial_ensureopen() {
    if (Serial_IsOpen() != 1) {
        if (Serial_Open(modeSerialOpen) != 0) {
            return 1;
        }
    }
    return 0;
}
// TODO make ptr, stream restrict for optimization
size_t fwrite(const void *ptr, size_t size, size_t nitems,FILE *stream) {
	if (isstdstream(stream)) {
		if (stream->fileno == 2) {
			// stderr: display but red font
			//return fwrite_serial(ptr, size, nitems, stream);
			int errorC=0xF800,errorCB=0;
			drawTinyStrn(ptr,&termxfxcg,&termyfxcg,&errorC,&errorCB,size*nitems);
			return nitems;
		}else if (stream->fileno == 1) {
            // stdout: display

			drawTinyStrn(ptr,&termxfxcg,&termyfxcg,&termFGfxcg,&termBGfxcg,size*nitems);
			return nitems;
        } else {
            // stdin..?
            return -1;
        }
    }
    // TODO this must be able to fail, but how?
    Bfile_WriteFile_OS(handle_tonative(stream->fileno), ptr, size * nitems);
    return nitems;
}
size_t fread(void *buffer, size_t size, size_t count, FILE *stream) {
	size_t n = size * count;
	if (isstdstream(stream)) {
		if (stream->fileno == 0) {
            // stdin
            return inputStrTiny(buffer, n,0);
        } else {
            // Reading stdout or stderr? No.
            return EOF;
        }
    }

    // TODO failure modes unknown
    size_t ret = Bfile_ReadFile_OS(handle_tonative(stream->fileno), buffer, n, -1);
    if (ret < 0) {
        stream->error = 1;
    } else if (ret < n) {
        stream->eof = 1;
    }
    return ret;
}

int fputc(int c, FILE *stream){
	unsigned char cc = (unsigned char)c;
	if (fwrite(&cc, 1, 1, stream) != 1)
			return EOF;
	return cc;
}

int putchar(int c) {
    return fputc(c, stdout);
}

int fputs(const char *s, FILE *stream) {
    size_t len = strlen(s);
    if (fwrite(s, 1, len, stdout) != len)
        return EOF;
    return 0;
}

int puts(const char *s) {
    int ret = fputs(s, stdout);
    putchar('\n');
    return ret;
}

char *fgets(char *s, int n, FILE *stream) {
	if(isstdstream(stream)){
		if (stream->fileno == 0){
			if(n<1)
				return 0;
			inputStrTiny(s,n-1,1);//reads max-1 characters
		}else
			return EOF;
	}else{
		char *c=s;
		while(n--){
			Bfile_ReadFile_OS(handle_tonative(stream->fileno), *c, 1, -1);
			if(*c=='\n')
				break;
			++c;
		}
		*(++c)=0;
	}
	return s;
}

int fgetc(FILE *f) {
	if (isstdstream(f)) {
		if (f->fileno == 0) {
            // stdin
            return getchar();
        } else {
            // Reading stdout or stderr? No.
            return EOF;
        }
    }else{
		unsigned char c;
		fread(&c, 1, 1, f);
		return c;
	}
}

int ungetc(int c, FILE *f) {
    if (c == EOF || f->has_unput)
        return EOF;
    f->unput = (char)c;
    f->has_unput = 1;
    return f->unput;
}
int getchar(void){
	char tmp;
	inputStrTiny(&tmp,1,0);
	return (tmp!=0)?tmp:EOF;
}

int fseek(FILE *f, long offset, int whence) {
	if (isstdstream(f)) {
		IOERR(f, ERANGE);
		return -1;
	}
	int fileno = handle_tonative(f->fileno);

	switch (whence) {
		case SEEK_CUR:
			offset = ftell(f) + offset;
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

	f->error = 0;
	f->eof = 0;
	return 0;
}

void rewind ( FILE * stream ){
	if (isstdstream(stream)) {
		IOERR(stream, ERANGE);
		return -1;
	}

    // TODO can this fail? Probably.
	Bfile_SeekFile_OS(handle_tonative(stream->fileno), 0);

	stream->error = 0;
	stream->eof = 0;
	return 0;
}

long ftell(FILE *f) {
    return Bfile_TellFile_OS(handle_tonative(f->fileno));
}

int ferror(FILE *f) {
    return f->error;
}

void perror(const char *s) {
    if (s != NULL && strlen(s) > 0) {
        fprintf(stderr, "(%s): ", s);
    }
    fprintf(stderr, "code %i (xref errno.h)\n", -errno);
}

int remove(const char *name) {
    uint16_t *widepath = alloca(2 * strlen(name) + 1);
    Bfile_StrToName_ncpy(widepath, name, strlen(name));

    int ret = Bfile_DeleteEntry(widepath);
    if (ret >= 0) {
        return 0;
    } else {
        switch (ret) {
        case -1:
        case -3:
        case -5:
            errno = ENOENT;
            break;
        default:
            errno = EIO;
        }
        return 1;
    }
}

int rename(const char *old, const char *newName) {
    uint16_t *wideold = alloca(2 * strlen(old) + 1);
    Bfile_StrToName_ncpy(wideold, old, strlen(old));
    uint16_t *widenew = alloca(2 * strlen(newName) + 1);
    Bfile_StrToName_ncpy(widenew, newName, strlen(newName));

    int ret = Bfile_RenameEntry(wideold, widenew);
    if (ret >= 0) {
        return 0;
    } else {
        // Unknown meanings. Fail with success.
        errno = 0;
        return 1;
    }
}

int mkdir(const char *path, unsigned mode) {
    uint16_t *widepath = alloca(2 * strlen(path) + 1);
    Bfile_StrToName_ncpy(widepath, path, strlen(path));

    int ret = Bfile_CreateEntry_OS(widepath, CREATEMODE_FOLDER, 0);
    if (ret >= 0) {
        return 0;
    } else {
        if (ret == -3) {
            errno = EEXIST;
        } else {
            // Unknown meaning. Fail with success.
            errno = 0;
        }
        return 1;
    }
}
int fflush(FILE * stream){
	
	return 0;
}
int fileno(FILE *stream){
	return stream->fileno;
}
