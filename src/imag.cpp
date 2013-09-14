/* Returns the coefficient of the imaginary part of complex z

	z		imag(z)
	-		-------

	a + i b		b

	exp(i a)	sin(a)
*/

#include "stdafx.h"
#include "defs.h"

void
eval_imag(void)
{
	push(cadr(p1));
	eval();
	imag();
}

void
imag(void)
{
	save();
	rect();
	p1 = pop();
	push(p1);
	push(p1);
	conjugate();
	subtract();
	push_integer(2);
	divide();
	push(imaginaryunit);
	divide();
	restore();
}

#if SELFTEST

static char *s[] = {

	"imag(a+i*b)",
	"b",

	"imag(1+exp(i*pi/3))",
	"1/2*3^(1/2)",

	"imag(i)",
	"1",

	"imag((-1)^(1/3))",
	"1/2*3^(1/2)",

	"imag(-i)",
	"-1",
};

void
test_imag(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
