// Divide polynomials

#include "stdafx.h"
#include "defs.h"

void
eval_quotient(void)
{
	push(cadr(p1));			// 1st arg, p(x)
	eval();

	push(caddr(p1));		// 2nd arg, q(x)
	eval();

	push(cadddr(p1));		// 3rd arg, x
	eval();

	p1 = pop();			// default x
	if (p1 == symbol(NIL))
		p1 = symbol(SYMBOL_X);
	push(p1);

	divpoly();
}

//-----------------------------------------------------------------------------
//
//	Divide polynomials
//
//	Input:		tos-3		Dividend
//
//			tos-2		Divisor
//
//			tos-1		x
//
//	Output:		tos-1		Quotient
//
//-----------------------------------------------------------------------------

#define DIVIDEND p1
#define DIVISOR p2
#define X p3
#define Q p4
#define QUOTIENT p5

void
divpoly(void)
{
	int h, i, m, n, x;
	U **dividend, **divisor;

	save();

	X = pop();
	DIVISOR = pop();
	DIVIDEND = pop();

	h = tos;

	dividend = stack + tos;

	push(DIVIDEND);
	push(X);
	m = coeff() - 1;	// m is dividend's power

	divisor = stack + tos;

	push(DIVISOR);
	push(X);
	n = coeff() - 1;	// n is divisor's power

	x = m - n;

	push_integer(0);
	QUOTIENT = pop();

	while (x >= 0) {

		push(dividend[m]);
		push(divisor[n]);
		divide();
		Q = pop();

		for (i = 0; i <= n; i++) {
			push(dividend[x + i]);
			push(divisor[i]);
			push(Q);
			multiply();
			subtract();
			dividend[x + i] = pop();
		}

		push(QUOTIENT);
		push(Q);
		push(X);
		push_integer(x);
		power();
		multiply();
		add();
		QUOTIENT = pop();

		m--;
		x--;
	}

	tos = h;

	push(QUOTIENT);

	restore();
}

#if SELFTEST

static char *s[] = {

	"quotient(x^2+1,x+1)-x+1",
	"0",

	"quotient(a*x^2+b*x+c,d*x+e)-(-a*e/(d^2)+a*x/d+b/d)",
	"0",
};

void
test_quotient(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
