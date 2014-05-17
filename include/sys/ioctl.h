#ifndef __SYS_IOCTL_H
#define __SYS_IOCTL_H 1
#define FBIOGET_FSCREENINFO 2
#define FBIOGET_VSCREENINFO 3
int ioctl(int d, unsigned long request, ...);
#endif
