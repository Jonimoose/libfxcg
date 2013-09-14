// get the coefficient of x^n in polynomial p(x)

#include "stdafx.h"
#include "defs.h"

#define P p1
#define X p2
#define N p3

void
eval_coeff(void)
{
	push(cadr(p1));			// 1st arg, p
	eval();

	push(caddr(p1));		// 2nd arg, x
	eval();

	push(cadddr(p1));		// 3rd arg, n
	eval();

	N = pop();
	X = pop();
	P = pop();

	if (N == symbol(NIL)) {		// only 2 args?
		N = X;
		X = symbol(SYMBOL_X);
	}

	push(P);			// divide p by x^n
	push(X);
	push(N);
	power();
	divide();

	push(X);			// keep the constant part
	filter();
}

//-----------------------------------------------------------------------------
//
//	Put polynomial coefficients on the stack
//
//	Input:		tos-2		p(x)
//
//			tos-1		x
//
//	Output:		Returns number of coefficients on stack
//
//			tos-n		Coefficient of x^0
//
//			tos-1		Coefficient of x^(n-1)
//
//-----------------------------------------------------------------------------

int
coeff(void)
{
	int h, n;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	for (;;) {

		push(p1);
		push(p2);
		push(zero);
		subst();
		eval();

		p3 = pop();
		push(p3);

		push(p1);
		push(p3);
		subtract();

		p1 = pop();

		if (equal(p1, zero)) {
			n = tos - h;
			restore();
			return n;
		}

		push(p1);
		push(p2);
		divide();
		p1 = pop();
	}
}

#if SELFTEST

static char *s[] = {

	"coeff(40*x^3+30*x^2+20*x+10,3)",
	"40",

	"coeff(40*x^3+30*x^2+20*x+10,2)",
	"30",

	"coeff(40*x^3+30*x^2+20*x+10,1)",
	"20",

	"coeff(40*x^3+30*x^2+20*x+10,0)",
	"10",

	"coeff(a*t^3+b*t^2+c*t+d,t,3)",
	"a",

	"coeff(a*t^3+b*t^2+c*t+d,t,2)",
	"b",

	"coeff(a*t^3+b*t^2+c*t+d,t,1)",
	"c",

	"coeff(a*t^3+b*t^2+c*t+d,t,0)",
	"d",
};

void
test_coeff(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
