/* Taylor expansion of a function

	push(F)
	push(X)
	push(N)
	push(A)
	taylor()
*/

#include "stdafx.h"
#include "defs.h"

void
eval_taylor(void)
{
	// 1st arg

	p1 = cdr(p1);
	push(car(p1));
	eval();

	// 2nd arg

	p1 = cdr(p1);
	push(car(p1));
	eval();
	p2 = pop();
	if (p2 == symbol(NIL))
		guess();
	else
		push(p2);

	// 3rd arg

	p1 = cdr(p1);
	push(car(p1));
	eval();
	p2 = pop();
	if (p2 == symbol(NIL))
		push_integer(24); // default number of terms
	else
		push(p2);

	// 4th arg

	p1 = cdr(p1);
	push(car(p1));
	eval();
	p2 = pop();
	if (p2 == symbol(NIL))
		push_integer(0); // default expansion point
	else
		push(p2);

	taylor();
}

#define F p1
#define X p2
#define N p3
#define A p4
#define C p5

void
taylor(void)
{
	int i, k;

	save();

	A = pop();
	N = pop();
	X = pop();
	F = pop();

	push(N);
	k = pop_integer();
	if (k == (int) 0x80000000) {
		push_symbol(TAYLOR);
		push(F);
		push(X);
		push(N);
		push(A);
		list(5);
		restore();
		return;
	}

	push(F);	// f(a)
	push(X);
	push(A);
	subst();
	eval();

	push_integer(1);
	C = pop();

	for (i = 1; i <= k; i++) {

		push(F);	// f = f'
		push(X);
		derivative();
		F = pop();

		if (iszero(F))
			break;

		push(C);	// c = c * (x - a)
		push(X);
		push(A);
		subtract();
		multiply();
		C = pop();

		push(F);	// f(a)
		push(X);
		push(A);
		subst();
		eval();

		push(C);
		multiply();
		push_integer(i);
		factorial();
		divide();

		add();
	}

	restore();
}

#if SELFTEST

static char *s[] = {

	"taylor(1/(5+4*cos(x)),x,6,0)-(1/9+2/81*x^2+5/1458*x^4+49/131220*x^6)",
	"0",

	"taylor(1/(5+4*cos(x)),x,6)-(1/9+2/81*x^2+5/1458*x^4+49/131220*x^6)",
	"0",
};

void
test_taylor(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
