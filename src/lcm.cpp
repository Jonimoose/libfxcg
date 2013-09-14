// Find the least common multiple of two expressions.

#include "stdafx.h"
#include "defs.h"

void
eval_lcm(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		lcm();
		p1 = cdr(p1);
	}
}

void
lcm(void)
{
	int x;
	x = expanding;
	save();
	yylcm();
	restore();
	expanding = x;
}

void
yylcm(void)
{
	expanding = 1;

	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	gcd();

	push(p1);
	divide();

	push(p2);
	divide();

	inverse();
}

#if SELFTEST

static char *s[] = {

	"lcm(4,6)",
	"12",

	"lcm(4*x,6*x*y)",
	"12*x*y",

	// multiple arguments

	"lcm(2,3,4)",
	"12",
};

void
test_lcm(void)
{
	test(__FILE__, s, sizeof (s) / sizeof (char *));
}

#endif
