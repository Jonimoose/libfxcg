/* wf_log10.c -- float version of w_log10.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* 
 * wrapper log10f(X)
 */

#include "fdlibm.h"
#include <errno.h>

#ifdef __STDC__
	float log10f(float x)		/* wrapper log10f */
#else
	float log10f(x)			/* wrapper log10f */
	float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log10f(x);
#else
	float z;
	struct exception exc;
	z = __ieee754_log10f(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<=(float)0.0) {
#ifndef HUGE_VAL 
#define HUGE_VAL inf
	    double inf = 0.0;

	    SET_HIGH_WORD(inf,0x7ff00000);	/* set inf to infinite */
#endif
	    exc.name = "log10f";
	    exc.err = 0;
	    exc.arg1 = exc.arg2 = (double)x;
	    if (_LIB_VERSION == _SVID_)
               exc.retval = -HUGE;
	    else
	       exc.retval = -HUGE_VAL;
	    if(x==(float)0.0) {
	        /* log10f(0) */
	        exc.type = SING;
	        if (_LIB_VERSION == _POSIX_)
	           errno = ERANGE;
	        else if (!matherr(&exc)) {
	           errno = ERANGE;
	        }
	    } else { 
	        /* log10f(x<0) */
	        exc.type = DOMAIN;
	        if (_LIB_VERSION == _POSIX_)
	           errno = EDOM;
	        else if (!matherr(&exc)) {
	           errno = EDOM;
	        }
                exc.retval = nan("");
            }
	    if (exc.err != 0)
               errno = exc.err;
            return (float)exc.retval; 
	} else
	    return z;
#endif
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double log10(double x)
#else
	double log10(x)
	double x;
#endif
{
	return (double) log10f((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
