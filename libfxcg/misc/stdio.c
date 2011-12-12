#include <fxcg_syscalls.h>

int pos;

int _open(const char *pathname, int mode) {
    return Bfile_OpenFile_OS( pathname, mode);
}

int _close(int fd) {
    return Bfile_CloseFile_OS( fd );
}

int _write(int fd, const void *buf, int count) {
    return Bfile_WriteFile_OS( fd, buf, count);
}

int _read(int fd, const void *buf, int count) {
    return Bfile_ReadFile_OS( fd, buf, count, pos);
}

int _lseek(int fd, int offset, int whence) {
    return Bfile_SeekFile_OS( fd, offset);
}

int _fseek(int fd, int offset, int whence){
    return Bfile_SeekFile_OS( fd, offset);
}

int _unlink(char *name){
    return Bfile_DeleteEntry( name );
}

