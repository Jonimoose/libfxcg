#include "stdafx.h"
#include "defs.h"

void
eval_arcsin(void)
{
	push(cadr(p1));
	eval();
	arcsin();
}

void
arcsin(void)
{
	int n;
	double d;

	save();

	p1 = pop();

	if (car(p1) == symbol(SIN)) {
		push(cadr(p1));
		restore();
		return;
	}

	if (isdouble(p1)) {
		errno = 0;
		d = asin(p1->u.d);
		if (errno)
			stop("arcsin function argument is not in the interval [-1,1]");
		push_double(d);
		restore();
		return;
	}

	// if p1 == 1/sqrt(2) then return 1/4*pi (45 degrees)

	if (isoneoversqrttwo(p1)) {
		push_rational(1, 4);
		push_symbol(PI);
		multiply();
		restore();
		return;
	}

	// if p1 == -1/sqrt(2) then return -1/4*pi (-45 degrees)

	if (isminusoneoversqrttwo(p1)) {
		push_rational(-1, 4);
		push_symbol(PI);
		multiply();
		restore();
		return;
	}

	if (!isrational(p1)) {
		push_symbol(ARCSIN);
		push(p1);
		list(2);
		restore();
		return;
	}

	push(p1);
	push_integer(2);
	multiply();
	n = pop_integer();

	switch (n) {

	case -2:
		push_rational(-1, 2);
		push_symbol(PI);
		multiply();
		break;

	case -1:
		push_rational(-1, 6);
		push_symbol(PI);
		multiply();
		break;

	case 0:
		push(zero);
		break;

	case 1:
		push_rational(1, 6);
		push_symbol(PI);
		multiply();
		break;

	case 2:
		push_rational(1, 2);
		push_symbol(PI);
		multiply();
		break;

	default:
		push_symbol(ARCSIN);
		push(p1);
		list(2);
		break;
	}

	restore();
}

#if SELFTEST

static char *s[] = {

	"arcsin(-1)",
	"-1/2*pi",

	"arcsin(-1/2)",
	"-1/6*pi",

	"arcsin(0)",
	"0",

	"arcsin(1/2)",
	"1/6*pi",

	"arcsin(1)",
	"1/2*pi",

	"arcsin(sin(-1/2*pi))",
	"-1/2*pi",

	"arcsin(sin(-1/6*pi))",
	"-1/6*pi",

	"arcsin(sin(0))",
	"0",

	"arcsin(sin(1/6*pi))",
	"1/6*pi",

	"arcsin(sin(1/2*pi))",
	"1/2*pi",

	"arcsin(sin(x))",
	"x",

	"arcsin(1/sqrt(2))",
	"1/4*pi",

	"arcsin(-1/sqrt(2))",
	"-1/4*pi",

	"arcsin(sin(1/4*pi))",
	"1/4*pi",

	"arcsin(sin(-1/4*pi))",
	"-1/4*pi",
};

void
test_arcsin(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
