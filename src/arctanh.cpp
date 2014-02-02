#include "stdafx.h"
#include "defs.h"

void
eval_arctanh(void)
{
	push(cadr(p1));
	eval();
	arctanh();
}

void
arctanh(void)
{
	double d;
	save();
	p1 = pop();
	if (car(p1) == symbol(TANH)) {
		push(cadr(p1));
		restore();
		return;
	}
	if (isdouble(p1)) {
		d = p1->u.d;
		if (d < -1.0 || d > 1.0)
			stop("arctanh function argument is not in the interval [-1,1]");
		d = log((1.0 + d) / (1.0 - d)) / 2.0;
		push_double(d);
		restore();
		return;
	}
	if (iszero(p1)) {
		push(zero);
		restore();
		return;
	}
	push_symbol(ARCTANH);
	push(p1);
	list(2);
	restore();
}

#if SELFTEST

static char *s[] = {

	"arctanh(0.0)",
	"0",

	"arctanh(0)",
	"0",

	"arctanh(tanh(x))",
	"x",
};

void
test_arctanh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
