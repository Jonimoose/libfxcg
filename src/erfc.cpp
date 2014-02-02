//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	erfc(x)
//
//	GW	Added erfc() from Numerical Recipes in C
//	
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void yyerfc(void);

void
eval_erfc(void)
{
	push(cadr(p1));
	eval();
	yerfc();
}

void
yerfc(void)
{
	save();
	yyerfc();
	restore();
}

static void
yyerfc(void)
{
	double d;

	p1 = pop();

	if (isdouble(p1)) {
		d = erfc(p1->u.d);
		push_double(d);
		return;
	}

	push_symbol(ERFC);
	push(p1);
	list(2);
	return;
}

// from Numerical Recipes in C

#ifndef LINUX
double
erfc(double x)
{
	double t, z, ans;
	z = fabs(x);
	t = 1.0 / (1.0 + 0.5 * z);

	ans=t*exp(-z*z-1.26551223+t*(1.00002368+t*(0.37409196+t*(0.09678418+
	t*(-0.18628806+t*(0.27886807+t*(-1.13520398+t*(1.48851587+
	t*(-0.82215223+t*0.17087277)))))))));

	return x >= 0.0 ? ans : 2.0-ans;
}
#endif

#if SELFTEST

static char *s[] = {

	"erfc(a)",
	"erfc(a)",

	"erfc(0.0)",
	"1",

	"float(erfc(0))",
	"1",
#if 0
	"float(erfc(1))",
	"0.157299",
#endif
};

void
test_erfc(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
