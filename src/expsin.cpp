// Do the exponential sine function.

#include "stdafx.h"
#include "defs.h"

void
eval_expsin(void)
{
	push(cadr(p1));
	eval();
	expsin();
}

void
expsin(void)
{
	save();

	p1 = pop();

	push(imaginaryunit);
	push(p1);
	multiply();
	exponential();
	push(imaginaryunit);
	divide();
	push_rational(1, 2);
	multiply();

	push(imaginaryunit);
	negate();
	push(p1);
	multiply();
	exponential();
	push(imaginaryunit);
	divide();
	push_rational(1, 2);
	multiply();

	subtract();

	restore();
}

#if SELFTEST

static char *s[] = {

	"expsin(x)",
	"1/2*i*exp(-i*x)-1/2*i*exp(i*x)",
};

void
test_expsin(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
