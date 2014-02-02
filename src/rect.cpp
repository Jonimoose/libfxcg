/* Convert complex z to rectangular form

	Input:		push	z

	Output:		Result on stack
*/

#include "stdafx.h"
#include "defs.h"

void
eval_rect(void)
{
	push(cadr(p1));
	eval();
	rect();
}

void
rect(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(ADD)) {
		push_integer(0);
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			rect();
			add();
			p1 = cdr(p1);
		}
	} else {
		push(p1);	// mag(z) * (cos(arg(z)) + i sin(arg(z)))
		mag();
		push(p1);
		arg();
		p1 = pop();
		push(p1);
		cosine();
		push(imaginaryunit);
		push(p1);
		sine();
		multiply();
		add();
		multiply();
	}
	restore();
}

#if SELFTEST

static char *s[] = {

	"rect(a+i*b)",
	"a+i*b",

	"rect(exp(a+i*b))",
	"i*exp(a)*sin(b)+exp(a)*cos(b)",

	"rect(1+exp(i*pi/3))",
	"3/2+1/2*i*3^(1/2)",

	"z=(a+b*i)/(c+d*i)",
	"",

	"rect(z)-real(z)-i*imag(z)",
	"0",

	"z=quote(z)",
	"",
};

void
test_rect(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
