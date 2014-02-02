/* Laguerre function

Example

	laguerre(x,3)

Result

	   1   3    3   2
	- --- x  + --- x  - 3 x + 1
	   6        2

The computation uses the following recurrence relation.

	L(x,0,k) = 1

	L(x,1,k) = -x + k + 1

	n*L(x,n,k) = (2*(n-1)+1-x+k)*L(x,n-1,k) - (n-1+k)*L(x,n-2,k)

In the "for" loop i = n-1 so the recurrence relation becomes

	(i+1)*L(x,n,k) = (2*i+1-x+k)*L(x,n-1,k) - (i+k)*L(x,n-2,k)
*/

#include "stdafx.h"
#include "defs.h"

void
eval_laguerre(void)
{
	// 1st arg

	push(cadr(p1));
	eval();

	// 2nd arg

	push(caddr(p1));
	eval();

	// 3rd arg

	push(cadddr(p1));
	eval();

	p2 = pop();
	if (p2 == symbol(NIL))
		push_integer(0);
	else
		push(p2);

	laguerre();
}

#define X p1
#define N p2
#define K p3
#define Y p4
#define Y0 p5
#define Y1 p6

void
laguerre(void)
{
	int n;
	save();

	K = pop();
	N = pop();
	X = pop();

	push(N);
	n = pop_integer();

	if (n < 0) {
		push_symbol(LAGUERRE);
		push(X);
		push(N);
		push(K);
		list(4);
		restore();
		return;
	}

	if (issymbol(X))
		laguerre2(n);
	else {
		Y = X;			// do this when X is an expr
		X = symbol(SECRETX);
		laguerre2(n);
		X = Y;
		push(symbol(SECRETX));
		push(X);
		subst();
		eval();
	}

	restore();
}

void
laguerre2(int n)
{
	int i;

	push_integer(1);
	push_integer(0);

	Y1 = pop();

	for (i = 0; i < n; i++) {

		Y0 = Y1;

		Y1 = pop();

		push_integer(2 * i + 1);
		push(X);
		subtract();
		push(K);
		add();
		push(Y1);
		multiply();

		push_integer(i);
		push(K);
		add();
		push(Y0);
		multiply();

		subtract();

		push_integer(i + 1);
		divide();
	}
}

#if SELFTEST

static char *s[] = {

	"laguerre(x,n)",
	"laguerre(x,n,0)",

	"laguerre(x,n,k)",
	"laguerre(x,n,k)",

	"laguerre(x,0)-1",
	"0",

	"laguerre(x,1)-(-x+1)",
	"0",

	"laguerre(x,2)-1/2*(x^2-4*x+2)",
	"0",

	"laguerre(x,3)-1/6*(-x^3+9*x^2-18*x+6)",
	"0",

	"laguerre(x,0,k)-1",
	"0",

	"laguerre(x,1,k)-(-x+k+1)",
	"0",

	"laguerre(x,2,k)-1/2*(x^2-2*(k+2)*x+(k+1)*(k+2))",
	"0",

	"laguerre(x,3,k)-1/6*(-x^3+3*(k+3)*x^2-3*(k+2)*(k+3)*x+(k+1)*(k+2)*(k+3))",
	"0",

	"laguerre(a-b,10)-eval(subst(a-b,x,laguerre(x,10)))",
	"0",
};

void
test_laguerre(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
