// Do the exponential cosine function.

#include "stdafx.h"
#include "defs.h"

void
eval_expcos(void)
{
	push(cadr(p1));
	eval();
	expcos();
}

void
expcos(void)
{
	save();

	p1 = pop();

	push(imaginaryunit);
	push(p1);
	multiply();
	exponential();
	push_rational(1, 2);
	multiply();

	push(imaginaryunit);
	negate();
	push(p1);
	multiply();
	exponential();
	push_rational(1, 2);
	multiply();

	add();

	restore();
}

#if SELFTEST

static char *s[] = {

	"expcos(x)",
	"1/2*exp(-i*x)+1/2*exp(i*x)",
};

void
test_expcos(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
