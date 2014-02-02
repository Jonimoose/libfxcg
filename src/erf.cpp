//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	Error function erf(x)
//	erf(-x)=erf(x)
//  
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void yyerf(void);

void
eval_erf(void)
{
	push(cadr(p1));
	eval();
	yerf();
}

void
yerf(void)
{
	save();
	yyerf();
	restore();
}

static void
yyerf(void)
{
	double d;

	p1 = pop();

	if (isdouble(p1)) {
		d = 1.0 - erfc(p1->u.d);
		push_double(d);
		return;
	}

	if (isnegativeterm(p1)) {
		push_symbol(ERF);
		push(p1);
		negate();
		list(2);
		negate();
		return;
	}
	
	push_symbol(ERF);
	push(p1);
	list(2);
	return;
}

#if SELFTEST

static char *s[] = {

	"erf(a)",
	"erf(a)",

	"erf(0.0) + 1",		// add 1 to round off
	"1",

	"float(erf(0)) + 1",	// add 1 to round off
	"1",
#if 0
	"float(erf(1))",
	"0.842701",
#endif
};

void
test_erf(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
