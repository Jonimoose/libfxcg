/* Convert complex z to polar form

	Input:		push	z

	Output:		Result on stack

	polar(z) = mag(z) * exp(i * arg(z))
*/

#include "stdafx.h"
#include "defs.h"

void
eval_polar(void)
{
	push(cadr(p1));
	eval();
	polar();
}

void
polar(void)
{
	save();
	p1 = pop();
	push(p1);
	mag();
	push(imaginaryunit);
	push(p1);
	arg();
	multiply();
	exponential();
	multiply();
	restore();
}

#if SELFTEST

static char *s[] = {

	"polar(1+i)",
	"2^(1/2)*exp(1/4*i*pi)",

	"polar(-1+i)",
	"2^(1/2)*exp(3/4*i*pi)",

	"polar(-1-i)",
	"2^(1/2)*exp(-3/4*i*pi)",

	"polar(1-i)",
	"2^(1/2)*exp(-1/4*i*pi)",

	"rect(polar(3+4*i))",
	"3+4*i",

	"rect(polar(-3+4*i))",
	"-3+4*i",

	"rect(polar(3-4*i))",
	"3-4*i",

	"rect(polar(-3-4*i))",
	"-3-4*i",
};

void
test_polar(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
