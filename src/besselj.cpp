/* Bessel J function

	1st arg		x

	2nd arg		n

Recurrence relation

	besselj(x,n) = (2/x) (n-1) besselj(x,n-1) - besselj(x,n-2)

	besselj(x,1/2) = sqrt(2/pi/x) sin(x)

	besselj(x,-1/2) = sqrt(2/pi/x) cos(x)

For negative n, reorder the recurrence relation as

	besselj(x,n-2) = (2/x) (n-1) besselj(x,n-1) - besselj(x,n)

Substitute n+2 for n to obtain

	besselj(x,n) = (2/x) (n+1) besselj(x,n+1) - besselj(x,n+2)

Examples

	besselj(x,3/2) = (1/x) besselj(x,1/2) - besselj(x,-1/2)

	besselj(x,-3/2) = -(1/x) besselj(x,-1/2) - besselj(x,1/2)
*/

#include "stdafx.h"
#include "defs.h"

void
eval_besselj(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	besselj();
}

void
besselj(void)
{
	save();
	yybesselj();
	restore();
}

#define X p1
#define N p2
#define SGN p3

void
yybesselj(void)
{
	double d;
	int n;

	N = pop();
	X = pop();

	push(N);
	n = pop_integer();

	// numerical result

	if (isdouble(X) && n != (int) 0x80000000) {
		d = jn(n, X->u.d);
		push_double(d);
		return;
	}

	// bessej(0,0) = 1

	if (iszero(X) && iszero(N)) {
		push_integer(1);
		return;
	}

	// besselj(0,n) = 0

	if (iszero(X) && n != (int) 0x80000000) {
		push_integer(0);
		return;
	}

	// half arguments

	if (N->k == NUM && MEQUAL(N->u.q.b, 2)) {

		// n = 1/2

		if (MEQUAL(N->u.q.a, 1)) {
			push_integer(2);
			push_symbol(PI);
			divide();
			push(X);
			divide();
			push_rational(1, 2);
			power();
			push(X);
			sine();
			multiply();
			return;
		}

		// n = -1/2

		if (MEQUAL(N->u.q.a, -1)) {
			push_integer(2);
			push_symbol(PI);
			divide();
			push(X);
			divide();
			push_rational(1, 2);
			power();
			push(X);
			cosine();
			multiply();
			return;
		}

		// besselj(x,n) = (2/x) (n-sgn(n)) besselj(x,n-sgn(n)) - besselj(x,n-2*sgn(n))

		push_integer(MSIGN(N->u.q.a));
		SGN = pop();

		push_integer(2);
		push(X);
		divide();
		push(N);
		push(SGN);
		subtract();
		multiply();
		push(X);
		push(N);
		push(SGN);
		subtract();
		besselj();
		multiply();
		push(X);
		push(N);
		push_integer(2);
		push(SGN);
		multiply();
		subtract();
		besselj();
		subtract();

		return;
	}

#if 0 // test cases needed
	if (isnegativeterm(X)) {
		push(X);
		negate();
		push(N);
		power();
		push(X);
		push(N);
		negate();
		power();
		multiply();
		push_symbol(BESSELJ);
		push(X);
		negate();
		push(N);
		list(3);
		multiply();
		return;
	}

	if (isnegativeterm(N)) {
		push_integer(-1);
		push(N);
		power();
		push_symbol(BESSELJ);
		push(X);
		push(N);
		negate();
		list(3);
		multiply();
		return;
	}
#endif

	push(symbol(BESSELJ));
	push(X);
	push(N);
	list(3);
}

#if SELFTEST

static char *s[] = {

	"besselj(x,n)",
	"besselj(x,n)",

	"besselj(0,0)",
	"1",

	"besselj(0,1)",
	"0",

	"besselj(0,-1)",
	"0",

	"besselj(x,1/2)-sqrt(2/pi/x)*sin(x)",
	"0",

	"besselj(x,-1/2)-sqrt(2/pi/x)*cos(x)",
	"0",

	"besselj(x,3/2)-sqrt(2/pi/x)*(sin(x)/x-cos(x))",
	"0",

	"besselj(x,-3/2)-sqrt(2/pi/x)*(-cos(x)/x-sin(x))",
	"0",

	"besselj(x,5/2)-sqrt(2/pi/x)*((3/x^2-1)*sin(x)-3/x*cos(x))",
	"0",

	"besselj(x,-5/2)-sqrt(2/pi/x)*((3/x^2-1)*cos(x)+3/x*sin(x))",
	"0",

	// From the note above

	"besselj(x,3/2)-(1/x)*besselj(x,1/2)+besselj(x,-1/2)",
	"0",

	"besselj(x,-3/2)+(1/x)*besselj(x,-1/2)+besselj(x,1/2)",
	"0",

	// this should simplify

	"y=besselj(x,5/2)",
	"",

	"x^2*d(y,x,x)+x*d(y,x)+(x^2-(5/2)^2)*y",
	"0",

	"y=quote(y)",
	"",
};

void
test_besselj(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
