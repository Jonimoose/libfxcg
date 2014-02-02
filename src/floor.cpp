#include "stdafx.h"
#include "defs.h"

void
eval_floor(void)
{
	push(cadr(p1));
	eval();
	yfloor();
}

void
yfloor(void)
{
	save();
	yyfloor();
	restore();
}

void
yyfloor(void)
{
	double d;

	p1 = pop();

	if (!isnum(p1)) {
		push_symbol(FLOOR);
		push(p1);
		list(2);
		return;
	}

	if (isdouble(p1)) {
		d = floor(p1->u.d);
		push_double(d);
		return;
	}

	if (isinteger(p1)) {
		push(p1);
		return;
	}

	p3 = alloc();
	p3->k = NUM;
	p3->u.q.a = mdiv(p1->u.q.a, p1->u.q.b);
	p3->u.q.b = mint(1);
	push(p3);

	if (isnegativenumber(p1)) {
		push_integer(-1);
		add();
	}
}

#if SELFTEST

static char *s[] = {

	"floor(a)",
	"floor(a)",

	"floor(a+b)",
	"floor(a+b)",

	"floor(5/2)",
	"2",

	"floor(4/2)",
	"2",

	"floor(3/2)",
	"1",

	"floor(2/2)",
	"1",

	"floor(1/2)",
	"0",

	"floor(0/2)",
	"0",

	"floor(-1/2)",
	"-1",

	"floor(-2/2)",
	"-1",

	"floor(-3/2)",
	"-2",

	"floor(-4/2)",
	"-2",

	"floor(-5/2)",
	"-3",

	"floor(5/2.0)",
	"2",

	"floor(4/2.0)",
	"2",

	"floor(3/2.0)",
	"1",

	"floor(2/2.0)",
	"1",

	"floor(1/2.0)",
	"0",

	"floor(0.0)",
	"0",

	"floor(-1/2.0)",
	"-1",

	"floor(-2/2.0)",
	"-1",

	"floor(-3/2.0)",
	"-2",

	"floor(-4/2.0)",
	"-2",

	"floor(-5/2.0)",
	"-3",
};

void
test_floor(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
