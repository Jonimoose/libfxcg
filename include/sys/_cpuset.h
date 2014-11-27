/*-
 * Copyright (c) 2008,	Jeffrey Roberson <jeff@freebsd.org>
 * All rights reserved.
 *
 * Copyright (c) 2008 Nokia Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD: head/sys/sys/_cpuset.h 270222 2014-08-20 16:05:15Z jhb $
 */

#ifndef _SYS__CPUSET_H_
#define	_SYS__CPUSET_H_

#include <sys/_bitset.h>

#ifdef _KERNEL
#define	CPU_SETSIZE	MAXCPU
#endif

#define	CPU_MAXSIZE	256

#ifndef	CPU_SETSIZE
#define	CPU_SETSIZE	CPU_MAXSIZE
#endif

#define	_NCPUBITS	_BITSET_BITS
#define	_NCPUWORDS	__bitset_words(CPU_SETSIZE)

BITSET_DEFINE(_cpuset, CPU_SETSIZE);
typedef struct _cpuset cpuset_t;

#define	CPUSET_FSET		BITSET_FSET(_NCPUWORDS)
#define	CPUSET_T_INITIALIZER	BITSET_T_INITIALIZER

#endif /* !_SYS__CPUSET_H_ */
