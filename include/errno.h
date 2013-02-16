#ifndef _FXCG_ERRNO_H
#define _FXCG_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

extern int errno;

#define EINVAL (-1)
#define ERANGE (-2)
#define EDOM (-3)
#define ENXIO (-4)
#define EAGAIN (-5)
#define EIO (-6)
#define ENOMEM (-7)
#define EEXIST (-8)
#define ENOENT (-9)

#define ENOTIMPL 0xBEEF

#ifdef __cplusplus
}
#endif

#endif
