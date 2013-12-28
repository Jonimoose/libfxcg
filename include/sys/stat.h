#ifndef __FXCG_STAT
#define __FXCG_STAT
#include <unistd.h>
#include <sys/types.h>
typedef unsigned int dev_t;
typedef unsigned int blksize_t;
typedef unsigned int blkcnt_t;
struct stat{
    dev_t     st_dev;     /* ID of device containing file */
    ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    blksize_t st_blksize; /* blocksize for file system I/O */
    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
};
#define S_IFMT		0170000	//bit mask for the file type bit fields
#define S_IFSOCK	0140000	//socket
#define S_IFLNK		0120000	//symbolic link
#define S_IFREG		0100000	//regular file
#define S_IFBLK		0060000	//block device
#define S_IFDIR		0040000	//directory

#define S_IFCHR		0020000	//character device
#define S_IFIFO		0010000	//FIFO
#define S_ISUID		0004000	//set-user-ID bit
#define S_ISGID		0002000	//set-group-ID bit (see below)
#define S_ISVTX		0001000	//sticky bit (see below)
#define S_IRWXU		00700	//mask for file owner permissions
#define S_IRUSR		00400	//owner has read permission
#define S_IWUSR		00200	//owner has write permission
#define S_IXUSR		00100	//owner has execute permission
#define S_IRWXG		00070	//mask for group permissions
#define S_IRGRP		00040	//group has read permission
#define S_IWGRP		00020	//group has write permission
#define S_IXGRP		00010	//group has execute permission
#define S_IRWXO		00007	//mask for permissions for others (not in group)
#define S_IROTH		00004	//others have read permission
#define S_IWOTH		00002	//others have write permission
#define S_IXOTH		00001	//others have execute permission

#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

int stat(const char *path, struct stat *buf);
#endif
