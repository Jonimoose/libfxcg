#
#  @(#)Makefile 1.4 95/01/18 
# 
#  ====================================================
#  Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
# 
#  Developed at SunSoft, a Sun Microsystems, Inc. business.
#  Permission to use, copy, modify, and distribute this
#  software is freely granted, provided that this notice 
#  is preserved.
#  ====================================================
# 
# 

#
# There are two options in making libm at fdlibm compile time:
# 	_IEEE_LIBM 	--- IEEE libm; smaller, and somewhat faster
#	_MULTI_LIBM	--- Support multi-standard at runtime by 
#			    imposing wrapper functions defined in 
#			    fdlibm.h:
#				_IEEE_MODE 	-- IEEE
#				_XOPEN_MODE 	-- X/OPEN
#				_POSIX_MODE 	-- POSIX/ANSI
#				_SVID3_MODE 	-- SVID
#
# Here is how to set up CFLAGS to create the desired libm at 
# compile time:
#
# 	CFLAGS = -D_IEEE_LIBM		... IEEE libm (recommended)
#	CFLAGS = -D_SVID3_MODE	... Multi-standard supported
#					    libm with SVID as the 
#					    default standard
#	CFLAGS = -D_XOPEN_MODE	... Multi-standard supported
#					    libm with XOPEN as the 
#					    default standard
#	CFLAGS = -D_POSIX_MODE	... Multi-standard supported
#					    libm with POSIX as the 
#					    default standard
#	CFLAGS = 			... Multi-standard supported
#					    libm with IEEE as the 
#					    default standard
# 
# NOTE: if scalb's second arguement is an int, then one must
# define _SCALB_INT in CFLAGS. The default prototype of scalb
# is double scalb(double, double)
#


#
# Default IEEE libm
#
AR=sh3eb-elf-ar
RANLIB=sh3eb-elf-ranlib
CC = sh3eb-elf-gcc
CFLAGS = -D_IEEE_LIBM  -m4a-nofpu -mb -Os -mhitachi -ffreestanding -Wall -L../lib -I../include -Wsystem-headers
LDFLAGS= $(CFLAGS) -Wl,-static

INCFILES = fdlibm.h
.INIT: $(INCFILES)
.KEEP_STATE:
src = 	k_standard.c k_rem_pio2.c \
	k_cos.c k_sin.c k_tan.c \
	e_acos.c e_acosh.c e_asin.c e_atan2.c \
	e_atanh.c e_cosh.c e_exp.c e_fmod.c \
	er_gamma.c e_hypot.c e_j0.c \
	e_j1.c e_jn.c er_lgamma.c \
	e_log.c e_log10.c e_pow.c e_rem_pio2.c e_remainder.c \
	e_scalb.c e_sinh.c e_sqrt.c \
	w_acos.c w_acosh.c w_asin.c w_atan2.c \
	w_atanh.c w_cosh.c w_exp.c w_fmod.c \
	w_gamma.c wr_gamma.c w_hypot.c w_j0.c \
	w_j1.c w_jn.c w_lgamma.c wr_lgamma.c \
	w_log.c w_log10.c w_pow.c w_remainder.c \
	w_scalb.c w_sinh.c w_sqrt.c \
	w_sincos.c \
	w_drem.c \
	s_asinh.c s_atan.c s_ceil.c \
	s_cos.c s_erf.c s_fabs.c s_floor.c \
	s_frexp.c s_ldexp.c \
	s_signif.c s_sin.c \
	s_tan.c s_tanh.c \
	w_exp2.c w_tgamma.c \
	kf_rem_pio2.c \
	kf_cos.c kf_sin.c kf_tan.c \
	ef_acos.c ef_acosh.c ef_asin.c ef_atan2.c \
	ef_atanh.c ef_cosh.c ef_exp.c ef_fmod.c \
	erf_gamma.c ef_hypot.c ef_j0.c \
	ef_j1.c ef_jn.c erf_lgamma.c \
	ef_log.c ef_log10.c ef_pow.c ef_rem_pio2.c ef_remainder.c \
	ef_scalb.c ef_sinh.c ef_sqrt.c \
	wf_acos.c wf_acosh.c wf_asin.c wf_atan2.c \
	wf_atanh.c wf_cosh.c wf_exp.c wf_fmod.c \
	wf_gamma.c wrf_gamma.c wf_hypot.c wf_j0.c \
	wf_j1.c wf_jn.c wf_lgamma.c wrf_lgamma.c \
	wf_log.c wf_log10.c wf_pow.c wf_remainder.c \
	wf_scalb.c wf_sinh.c wf_sqrt.c \
	wf_sincos.c \
	wf_drem.c \
	sf_asinh.c sf_atan.c sf_ceil.c \
	sf_cos.c sf_erf.c sf_fabs.c sf_floor.c \
	sf_frexp.c sf_ldexp.c \
	sf_signif.c sf_sin.c \
	sf_tan.c sf_tanh.c \
	wf_exp2.c wf_tgamma.c \
	s_finite.c s_copysign.c s_modf.c s_scalbn.c \
	s_cbrt.c s_exp10.c s_expm1.c s_ilogb.c \
	s_infinity.c \
	s_log1p.c s_nan.c s_nextafter.c s_pow10.c \
	s_rint.c s_logb.c s_log2.c s_matherr.c s_lib_ver.c \
	s_fdim.c s_fma.c s_fmax.c s_fmin.c s_fpclassify.c \
	s_lrint.c s_llrint.c \
	s_lround.c s_llround.c s_nearbyint.c s_remquo.c s_round.c s_scalbln.c \
	s_signbit.c s_trunc.c \
	sf_finite.c sf_copysign.c sf_modf.c sf_scalbn.c \
	sf_cbrt.c sf_exp10.c sf_expm1.c sf_ilogb.c \
	sf_infinity.c \
	sf_log1p.c sf_nan.c sf_nextafter.c sf_pow10.c \
	sf_rint.c sf_logb.c sf_log2.c \
	sf_fdim.c sf_fma.c sf_fmax.c sf_fmin.c sf_fpclassify.c \
	sf_lrint.c sf_llrint.c \
	sf_lround.c sf_llround.c sf_nearbyint.c sf_remquo.c sf_round.c \
	sf_scalbln.c sf_trunc.c \
	atanl.c cosl.c sinl.c tanl.c tanhl.c frexpl.c modfl.c ceill.c fabsl.c \
	floorl.c log1pl.c expm1l.c acosl.c asinl.c atan2l.c coshl.c sinhl.c \
	expl.c ldexpl.c logl.c log10l.c powl.c sqrtl.c fmodl.c hypotl.c \
	copysignl.c nanl.c ilogbl.c asinhl.c cbrtl.c nextafterl.c rintl.c \
	scalbnl.c exp2l.c scalblnl.c tgammal.c nearbyintl.c lrintl.c llrintl.c \
	roundl.c lroundl.c llroundl.c truncl.c remquol.c fdiml.c fmaxl.c fminl.c \
	fmal.c acoshl.c atanhl.c remainderl.c lgammal.c erfl.c erfcl.c \
	isinf.c isnan.c


obj =	$(src:.c=.o)

all: libm.a 

libm.a : $(obj) 
	$(AR) cru libm.a $(obj)
	$(RANLIB) libm.a
	cp libm.a ../lib/libm.a

source: $(src) README

clean: 
	/bin/rm -f $(obj) a.out libm.a
