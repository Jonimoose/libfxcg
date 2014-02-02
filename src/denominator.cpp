#include "stdafx.h"
#include "defs.h"

void
eval_denominator(void)
{
	push(cadr(p1));
	eval();
	denominator();
}

void
denominator(void)
{
	int h;

	save();

	p1 = pop();

	if (car(p1) == symbol(ADD)) {
		push(p1);
		rationalize();
		p1 = pop();
	}

	if (car(p1) == symbol(MULTIPLY)) {
		h = tos;
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			denominator();
			p1 = cdr(p1);
		}
		multiply_all(tos - h);
	} else if (isrational(p1)) {
		push(p1);
		mp_denominator();
	} else if (car(p1) == symbol(POWER) && isnegativeterm(caddr(p1))) {
		push(p1);
		reciprocate();
	} else
		push(one);

	restore();
}

#if SELFTEST

static char *s[] = {

	"denominator(2/3)",
	"3",

	"denominator(x)",
	"1",

	"denominator(1/x)",
	"x",

	"denominator(a+b)",
	"1",

	"denominator(1/a+1/b)",
	"a*b",

	// denominator function expands

	"denominator(1/(x-1)/(x-2))",
	"x^2-3*x+2",
};

void
test_denominator(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
