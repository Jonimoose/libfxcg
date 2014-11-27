/* 
 * strerror.c --
 *
 *	Source code for the "strerror" library routine.
 *
 * Copyright 1988 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/c/string/RCS/strerror.c,v 1.6 90/06/27 11:16:32 shirriff Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include <stdio.h>
#include <string.h>

/*
 * List of known errors:
 */

char *sys_errlist[] = {
    "no error (operation succeeded",		/* 0 */
    "not owner",				/* EPERM */
    "no such file or directory",		/* ENOENT */
    "no such process",				/* ESRCH */
    "interrupted system call",			/* EINTR */
    "I/O error",				/* EIO */
    "no such device or address",		/* ENXIO */
    "argument list too long",			/* E2BIG */
    "exec format error",			/* ENOEXEC */
    "bad file number",				/* EBADF */
    "no children",				/* ECHILD */
    "no more processes",			/* EAGAIN */
    "not enough memory",			/* ENOMEM */
    "permission denied",			/* EACCESS */
    "bad address in system call argument",	/* EFAULT */
    "block device required",			/* ENOTBLK */
    "mount device busy",			/* EBUSY */
    "file already exists",			/* EEXIST */
    "cross-domain link",			/* EXDEV */
    "no such device",				/* ENODEV */
    "not a directory",				/* ENOTDIR */
    "illegal operation on a directory",		/* EISDIR */
    "invalid argument",				/* EINVAL */
    "file table overflow",			/* ENFILE */
    "too many open files",			/* EMFILE */
    "inappropriate device for ioctl",		/* ENOTTY */
    "text file or pseudo-device busy",		/* ETXTBSY */
    "file too large",				/* EFBIG */
    "no space left in file system domain",	/* ENOSPC */
    "illegal seek",				/* ESPIPE */
    "read-only file system",			/* EROFS */
    "too many links",				/* EMLINK */
    "broken pipe",				/* EPIPE */
    "math argument out of range",		/* EDOM */
    "math result unrepresentable",		/* ERANGE */
    "operation would block",			/* EWOULDBLOCK */
    "operation now in progress",		/* EINPROGRESS */
    "operation already in progress",		/* EALREADY */
    "socket operation on non-socket",		/* ENOTSOCK */
    "destination address required",		/* EDESTADDRREQ */
    "message too long",				/* EMSGSIZE */
    "protocol wrong type for socket",		/* EPROTOTYPE */
    "bad proocol option",			/* ENOPROTOOPT */
    "protocol not suppored",			/* EPROTONOSUPPORT */
    "socket type not supported",		/* ESOCKTNOSUPPORT */
    "operation not supported on socket",	/* EOPNOTSUPP */
    "protocol family not supported",		/* EPFNOSUPPORT */
    "address family not supported by protocol family",	/* EAFNOSUPPORT */
    "address already in use",			/* EADDRINUSE */
    "can't assign requested address",		/* EADDRNOTAVAIL */
    "network is down",				/* ENETDOWN */
    "network is unreachable",			/* ENETUNREACH */
    "network dropped connection on reset",	/* ENETRESET */
    "software caused connection abort",		/* ECONNABORTED */
    "connection reset by peer",			/* ECONNRESET */
    "no buffer space available",		/* ENOBUFS */
    "socket is already connected",		/* EISCONN */
    "socket is not connected",			/* ENOTCONN */
    "can't send afer socket shutdown",		/* ESHUTDOWN */
    "undefined error (59)",			/* not used */
    "connection timed out",			/* ETIMEDOUT */
    "connection refused",			/* ECONNREFUSED */
    "too many levels of symbolic links",	/* ELOOP */
    "file name too long",			/* ENAMETOOLONG */
    "host is down",				/* EHOSTDOWN */
    "host is unreachable",			/* EHOSTUNREACH */
    "directory not empty",			/* ENOTEMPTY */
    "too many processes",			/* EPROCLIM */
    "too many users",				/* EUSERS */
    "disk quota exceeded",			/* EDQUOT */
    "stale remote file handle",			/* ESTALE */
    "pathname hit remote file system",		/* EREMOTE */
    "undefined error (72)",			/* not used */
    "undefined error (73)",			/* not used */
    "undefined error (74)",			/* not used */
    "undefined error (75)",			/* not used */
    "undefined error (76)",			/* not used */
    "identifier removed",			/* EIDRM */
};
int sys_nerr = sizeof(sys_errlist)/sizeof(char *);

/*
 *----------------------------------------------------------------------
 *
 * strerror --
 *
 *	Map an integer error number into a printable string.
 *
 * Results:
 *	The return value is a pointer to a string describing
 *	error.  The first character of string isn't capitalized.
 *
 * Side effects:
 *	Each call to this procedure may overwrite the value returned
 *	by the previous call.
 *
 *----------------------------------------------------------------------
 */

char *
strerror(error)
    int error;			/* Integer identifying error (must be
				 * one of the officially-defined Sprite
				 * errors, as defined in errno.h). */
{
    static char defaultMsg[50];

    if ((error <= sys_nerr) && (error > 0)) {
	return sys_errlist[error];
    }
    (void) sprintf(defaultMsg, "unknown error (%d)", error);
    return defaultMsg;
}
