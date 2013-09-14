//	          exp(x) + exp(-x)
//	cosh(x) = ----------------
//	                 2

#include "stdafx.h"
#include "defs.h"

void
eval_cosh(void)
{
	push(cadr(p1));
	eval();
	ycosh();
}

void
ycosh(void)
{
	save();
	yycosh();
	restore();
}

void
yycosh(void)
{
	double d;
	p1 = pop();
	if (car(p1) == symbol(ARCCOSH)) {
		push(cadr(p1));
		return;
	}
	if (isdouble(p1)) {
		d = cosh(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}
	if (iszero(p1)) {
		push(one);
		return;
	}
	push_symbol(COSH);
	push(p1);
	list(2);
}

#if SELFTEST

static char *s[] = {

	"cosh(x)",
	"cosh(x)",

	"cosh(0)",
	"1",

	"cosh(arccosh(x))",
	"x",
};

void
test_cosh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
