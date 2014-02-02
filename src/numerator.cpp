#include "stdafx.h"
#include "defs.h"

void
eval_numerator(void)
{
	push(cadr(p1));
	eval();
	numerator();
}

void
numerator(void)
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
			numerator();
			p1 = cdr(p1);
		}
		multiply_all(tos - h);
	} else if (isrational(p1)) {
		push(p1);
		mp_numerator();
	} else if (car(p1) == symbol(POWER) && isnegativeterm(caddr(p1)))
		push(one);
	else 
		push(p1);

	restore();
}

#if SELFTEST

static char *s[] = {

	"numerator(2/3)",
	"2",

	"numerator(x)",
	"x",

	"numerator(1/x)",
	"1",

	"numerator(a+b)",
	"a+b",

	"numerator(1/a+1/b)",
	"a+b",
};

void
test_numerator(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
