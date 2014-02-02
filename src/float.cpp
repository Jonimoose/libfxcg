#include "stdafx.h"
#include "defs.h"

void
eval_float(void)
{
	push(cadr(p1));
	eval();
	yyfloat();
	eval(); // normalize
}

void
yyfloat(void)
{
	int i, h;
	save();
	p1 = pop();
	if (iscons(p1)) {
		h = tos;
		while (iscons(p1)) {
			push(car(p1));
			yyfloat();
			p1 = cdr(p1);
		}
		list(tos - h);
	} else if (p1->k == TENSOR) {
		push(p1);
		copy_tensor();
		p1 = pop();
		for (i = 0; i < p1->u.tensor->nelem; i++) {
			push(p1->u.tensor->elem[i]);
			yyfloat();
			p1->u.tensor->elem[i] = pop();
		}
		push(p1);
	} else if (p1->k == NUM) {
		push(p1);
		bignum_float();
	} else if (p1 == symbol(PI))
		push_double(M_PI);
	else if (p1 == symbol(E))
		push_double(M_E);
	else
		push(p1);
	restore();
}

#if SELFTEST

static char *s[] = {

	"float(x)",
	"x",

	"float(1/2)",
	"0.5",

	"float(pi)",
	"3.14159",

	"float(exp(1))",
	"2.71828",

	"x=(1/2,1/4)",
	"",

	"float(x)",
	"(0.5,0.25)",

	"x",
	"(1/2,1/4)",

	"x=quote(x)",
	"",
};

void
test_float(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
