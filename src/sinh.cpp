//	          exp(x) - exp(-x)
//	sinh(x) = ----------------
//	                 2

#include "stdafx.h"
#include "defs.h"

void
eval_sinh(void)
{
	push(cadr(p1));
	eval();
	ysinh();
}

void
ysinh(void)
{
	save();
	yysinh();
	restore();
}

void
yysinh(void)
{
	double d;
	p1 = pop();
	if (car(p1) == symbol(ARCSINH)) {
		push(cadr(p1));
		return;
	}
	if (isdouble(p1)) {
		d = sinh(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}
	if (iszero(p1)) {
		push(zero);
		return;
	}
	push_symbol(SINH);
	push(p1);
	list(2);
}

#if SELFTEST

static char *s[] = {

	"sinh(x)",
	"sinh(x)",

	"sinh(0)",
	"0",

	"sinh(arcsinh(x))",
	"x",
};

void
test_sinh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
