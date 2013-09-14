// Rational power function

#include "stdafx.h"
#include "defs.h"

static void qpowf(void);
static void normalize_angle(void);
static int is_small_integer(U *);

void
qpow()
{
	save();
	qpowf();
	restore();
}

#define BASE p1
#define EXPO p2

static void
qpowf(void)
{
	int expo;
	unsigned int a, b, *t, *x, *y;

	EXPO = pop();
	BASE = pop();

	// if base is 1 or exponent is 0 then return 1

	if (isplusone(BASE) || iszero(EXPO)) {
		push_integer(1);
		return;
	}

	// if base is zero then return 0

	if (iszero(BASE)) {
		if (isnegativenumber(EXPO))
			stop("divide by zero");
		push(zero);
		return;
	}

	// if exponent is 1 then return base

	if (isplusone(EXPO)) {
		push(BASE);
		return;
	}

	// if exponent is integer then power

	if (isinteger(EXPO)) {
		push(EXPO);
		expo = pop_integer();
		if (expo == (int) 0x80000000) {
			// expo greater than 32 bits
			push_symbol(POWER);
			push(BASE);
			push(EXPO);
			list(3);
			return;
		}
		x = mpow(BASE->u.q.a, abs(expo));
		y = mpow(BASE->u.q.b, abs(expo));
		if (expo < 0) {
			t = x;
			x = y;
			y = t;
			MSIGN(x) = MSIGN(y);
			MSIGN(y) = 1;
		}
		p3 = alloc();
		p3->k = NUM;
		p3->u.q.a = x;
		p3->u.q.b = y;
		push(p3);
		return;
	}

	// from here on out the exponent is NOT an integer

	// if base is -1 then normalize polar angle

	if (isminusone(BASE)) {
		push(EXPO);
		normalize_angle();
		return;
	}

	// if base is negative then (-N)^M -> N^M * (-1)^M

	if (isnegativenumber(BASE)) {
		push(BASE);
		negate();
		push(EXPO);
		qpow();
		push_integer(-1);
		push(EXPO);
		qpow();
		multiply();
		return;
	}

	// if BASE is not an integer then power numerator and denominator

	if (!isinteger(BASE)) {
		push(BASE);
		mp_numerator();
		push(EXPO);
		qpow();
		push(BASE);
		mp_denominator();
		push(EXPO);
		negate();
		qpow();
		multiply();
		return;
	}

	// At this point BASE is a positive integer.

	// If BASE is small then factor it.

	if (is_small_integer(BASE)) {
		push(BASE);
		push(EXPO);
		quickfactor();
		return;
	}

	// At this point BASE is a positive integer and EXPO is not an integer.

	if (MLENGTH(EXPO->u.q.a) > 1 || MLENGTH(EXPO->u.q.b) > 1) {
		push_symbol(POWER);
		push(BASE);
		push(EXPO);
		list(3);
		return;
	}

	a = EXPO->u.q.a[0];
	b = EXPO->u.q.b[0];

	x = mroot(BASE->u.q.a, b);

	if (x == 0) {
		push_symbol(POWER);
		push(BASE);
		push(EXPO);
		list(3);
		return;
	}

	y = mpow(x, a);

	mfree(x);

	p3 = alloc();

	p3->k = NUM;

	if (MSIGN(EXPO->u.q.a) == -1) {
		p3->u.q.a = mint(1);
		p3->u.q.b = y;
	} else {
		p3->u.q.a = y;
		p3->u.q.b = mint(1);
	}

	push(p3);
}

//-----------------------------------------------------------------------------
//
//	Normalize the angle of unit imaginary, i.e. (-1) ^ N
//
//	Input:		N on stack (must be rational, not float)
//
//	Output:		Result on stack
//
//	Note:
//
//	n = q * d + r
//
//	Example:
//						n	d	q	r
//
//	(-1)^(8/3)	->	 (-1)^(2/3)	8	3	2	2
//	(-1)^(7/3)	->	 (-1)^(1/3)	7	3	2	1
//	(-1)^(5/3)	->	-(-1)^(2/3)	5	3	1	2
//	(-1)^(4/3)	->	-(-1)^(1/3)	4	3	1	1
//	(-1)^(2/3)	->	 (-1)^(2/3)	2	3	0	2
//	(-1)^(1/3)	->	 (-1)^(1/3)	1	3	0	1
//
//	(-1)^(-1/3)	->	-(-1)^(2/3)	-1	3	-1	2
//	(-1)^(-2/3)	->	-(-1)^(1/3)	-2	3	-1	1
//	(-1)^(-4/3)	->	 (-1)^(2/3)	-4	3	-2	2
//	(-1)^(-5/3)	->	 (-1)^(1/3)	-5	3	-2	1
//	(-1)^(-7/3)	->	-(-1)^(2/3)	-7	3	-3	2
//	(-1)^(-8/3)	->	-(-1)^(1/3)	-8	3	-3	1
//
//-----------------------------------------------------------------------------

#define A p1
#define Q p2
#define R p3

static void
normalize_angle(void)
{
	save();

	A = pop();

	// integer exponent?

	if (isinteger(A)) {
		if (A->u.q.a[0] & 1)
			push_integer(-1); // odd exponent
		else
			push_integer(1); // even exponent
		restore();
		return;
	}

	// floor

	push(A);
	bignum_truncate();
	Q = pop();

	if (isnegativenumber(A)) {
		push(Q);
		push_integer(-1);
		add();
		Q = pop();
	}

	// remainder (always positive)

	push(A);
	push(Q);
	subtract();
	R = pop();

	// remainder becomes new angle

	push_symbol(POWER);
	push_integer(-1);
	push(R);
	list(3);

	// negate if quotient is odd

	if (Q->u.q.a[0] & 1)
		negate();

	restore();
}

static int
is_small_integer(U *p)
{
	if (isinteger(p) && MLENGTH(p->u.q.a) == 1 && (p->u.q.a[0] & 0x80000000) == 0)
		return 1;
	else
		return 0;
}
