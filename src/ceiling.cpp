#include "stdafx.h"
#include "defs.h"

void
eval_ceiling(void)
{
	push(cadr(p1));
	eval();
	ceiling();
}

void
ceiling(void)
{
	save();
	yyceiling();
	restore();
}

void
yyceiling(void)
{
	double d;

	p1 = pop();

	if (!isnum(p1)) {
		push_symbol(CEILING);
		push(p1);
		list(2);
		return;
	}

	if (isdouble(p1)) {
		d = ceil(p1->u.d);
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

	if (isnegativenumber(p1))
		;
	else {
		push_integer(1);
		add();
	}
}

#if SELFTEST

static char *s[] = {

	"ceiling(a)",
	"ceiling(a)",

	"ceiling(a+b)",
	"ceiling(a+b)",

	"ceiling(5/2)",
	"3",

	"ceiling(4/2)",
	"2",

	"ceiling(3/2)",
	"2",

	"ceiling(2/2)",
	"1",

	"ceiling(1/2)",
	"1",

	"ceiling(0/2)",
	"0",

	"ceiling(-1/2)",
	"0",

	"ceiling(-2/2)",
	"-1",

	"ceiling(-3/2)",
	"-1",

	"ceiling(-4/2)",
	"-2",

	"ceiling(-5/2)",
	"-2",

	"ceiling(5/2.0)",
	"3",

	"ceiling(4/2.0)",
	"2",

	"ceiling(3/2.0)",
	"2",

	"ceiling(2/2.0)",
	"1",

	"ceiling(1/2.0)",
	"1",

	"ceiling(0.0)",
	"0",

	"ceiling(-1/2.0)",
	"0",

	"ceiling(-2/2.0)",
	"-1",

	"ceiling(-3/2.0)",
	"-1",

	"ceiling(-4/2.0)",
	"-2",

	"ceiling(-5/2.0)",
	"-2",
};

void
test_ceiling(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
