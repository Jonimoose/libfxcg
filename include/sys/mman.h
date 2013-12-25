#ifndef __SYSMMAN_H
#define __SYSMMAN_H 1
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define PROT_NONE 8
#define MAP_SHARED 1
#define MAP_PRIVATE 2
#define MAP_FIXED 4
#define MAP_FAILED 0
typedef int off_t;
void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int munmap(void *addr, size_t len);
#endif
