// Symbolic multiplication

#include "stdafx.h"
#include "defs.h"

extern void append(void);
static void parse_p1(void);
static void parse_p2(void);
static void __normalize_radical_factors(int);

void
multiply(void)
{
	if (esc_flag)
		stop("escape key stop");
	if (isnum(stack[tos - 2]) && isnum(stack[tos - 1]))
		multiply_numbers();
	else {
		save();
		yymultiply();
		restore();
	}
}

void
yymultiply(void)
{
	int h, i, n;

	// pop operands

	p2 = pop();
	p1 = pop();

	h = tos;

	// is either operand zero?

	if (iszero(p1) || iszero(p2)) {
		push(zero);
		return;
	}

	// is either operand a sum?

	if (expanding && isadd(p1)) {
		p1 = cdr(p1);
		push(zero);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			multiply();
			add();
			p1 = cdr(p1);
		}
		return;
	}

	if (expanding && isadd(p2)) {
		p2 = cdr(p2);
		push(zero);
		while (iscons(p2)) {
			push(p1);
			push(car(p2));
			multiply();
			add();
			p2 = cdr(p2);
		}
		return;
	}

	// scalar times tensor?

	if (!istensor(p1) && istensor(p2)) {
		push(p1);
		push(p2);
		scalar_times_tensor();
		return;
	}

	// tensor times scalar?

	if (istensor(p1) && !istensor(p2)) {
		push(p1);
		push(p2);
		tensor_times_scalar();
		return;
	}

	// adjust operands

	if (car(p1) == symbol(MULTIPLY))
		p1 = cdr(p1);
	else {
		push(p1);
		list(1);
		p1 = pop();
	}

	if (car(p2) == symbol(MULTIPLY))
		p2 = cdr(p2);
	else {
		push(p2);
		list(1);
		p2 = pop();
	}

	// handle numerical coefficients

	if (isnum(car(p1)) && isnum(car(p2))) {
		push(car(p1));
		push(car(p2));
		multiply_numbers();
		p1 = cdr(p1);
		p2 = cdr(p2);
	} else if (isnum(car(p1))) {
		push(car(p1));
		p1 = cdr(p1);
	} else if (isnum(car(p2))) {
		push(car(p2));
		p2 = cdr(p2);
	} else
		push(one);

	parse_p1();
	parse_p2();

	while (iscons(p1) && iscons(p2)) {

//		if (car(p1)->gamma && car(p2)->gamma) {
//			combine_gammas(h);
//			p1 = cdr(p1);
//			p2 = cdr(p2);
//			parse_p1();
//			parse_p2();
//			continue;
//		}

		if (caar(p1) == symbol(OPERATOR) && caar(p2) == symbol(OPERATOR)) {
			push_symbol(OPERATOR);
			push(cdar(p1));
			push(cdar(p2));
			append();
			cons();
			p1 = cdr(p1);
			p2 = cdr(p2);
			parse_p1();
			parse_p2();
			continue;
		}

		switch (cmp_expr(p3, p4)) {
		case -1:
			push(car(p1));
			p1 = cdr(p1);
			parse_p1();
			break;
		case 1:
			push(car(p2));
			p2 = cdr(p2);
			parse_p2();
			break;
		case 0:
			combine_factors(h);
			p1 = cdr(p1);
			p2 = cdr(p2);
			parse_p1();
			parse_p2();
			break;
		default:
			stop("internal error 2");
			break;
		}
	}

	// push remaining factors, if any

	while (iscons(p1)) {
		push(car(p1));
		p1 = cdr(p1);
	}

	while (iscons(p2)) {
		push(car(p2));
		p2 = cdr(p2);
	}

	// normalize radical factors

	// example: 2*2(-1/2) -> 2^(1/2)

	// must be done after merge because merge may produce radical

	// example: 2^(1/2-a)*2^a -> 2^(1/2)

	__normalize_radical_factors(h);

	// this hack should not be necessary, unless power returns a multiply

	//for (i = h; i < tos; i++) {
	//	if (car(stack[i]) == symbol(MULTIPLY)) {
	//		multiply_all(tos - h);
	//		return;
	//	}
	//}

	if (expanding) {
		for (i = h; i < tos; i++) {
			if (isadd(stack[i])) {
				multiply_all(tos - h);
				return;
			}
		}
	}

	// n is the number of result factors on the stack

	n = tos - h;

	if (n == 1)
		return;

	// discard integer 1

	if (isrational(stack[h]) && equaln(stack[h], 1)) {
		if (n == 2) {
			p7 = pop();
			pop();
			push(p7);
		} else {
			stack[h] = symbol(MULTIPLY);
			list(n);
		}
		return;
	}

	list(n);
	p7 = pop();
	push_symbol(MULTIPLY);
	push(p7);
	cons();
}

// Decompose a factor into base and power.
//
// input:	car(p1)		factor
//
// output:	p3		factor's base
//
//		p5		factor's power (possibly 1)

static void
parse_p1(void)
{
	p3 = car(p1);
	p5 = one;
	if (car(p3) == symbol(POWER)) {
		p5 = caddr(p3);
		p3 = cadr(p3);
	}
}

// Decompose a factor into base and power.
//
// input:	car(p2)		factor
//
// output:	p4		factor's base
//
//		p6		factor's power (possibly 1)

static void
parse_p2(void)
{
	p4 = car(p2);
	p6 = one;
	if (car(p4) == symbol(POWER)) {
		p6 = caddr(p4);
		p4 = cadr(p4);
	}
}

void
combine_factors(int h)
{
	push(p4);
	push(p5);
	push(p6);
	add();
	power();
	p7 = pop();
	if (isnum(p7)) {
		push(stack[h]);
		push(p7);
		multiply_numbers();
		stack[h] = pop();
	} else if (car(p7) == symbol(MULTIPLY)) {
		// power can return number * factor (i.e. -1 * i)
		if (isnum(cadr(p7)) && cdddr(p7) == symbol(NIL)) {
			push(stack[h]);
			push(cadr(p7));
			multiply_numbers();
			stack[h] = pop();
			push(caddr(p7));
		} else
			push(p7);
	} else
		push(p7);
}

int gp[17][17] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,-6,-7,-8,-3,-4,-5,13,14,15,-16,9,10,11,-12},
	{0,0,6,-1,-11,10,-2,-15,14,12,-5,4,-9,16,-8,7,-13},
	{0,0,7,11,-1,-9,15,-2,-13,5,12,-3,-10,8,16,-6,-14},
	{0,0,8,-10,9,-1,-14,13,-2,-4,3,12,-11,-7,6,16,-15},
	{0,0,3,2,15,-14,1,11,-10,16,-8,7,13,12,-5,4,9},
	{0,0,4,-15,2,13,-11,1,9,8,16,-6,14,5,12,-3,10},
	{0,0,5,14,-13,2,10,-9,1,-7,6,16,15,-4,3,12,11},
	{0,0,13,12,-5,4,16,-8,7,-1,-11,10,-3,-2,-15,14,-6},
	{0,0,14,5,12,-3,8,16,-6,11,-1,-9,-4,15,-2,-13,-7},
	{0,0,15,-4,3,12,-7,6,16,-10,9,-1,-5,-14,13,-2,-8},
	{0,0,16,-9,-10,-11,-13,-14,-15,-3,-4,-5,1,-6,-7,-8,2},
	{0,0,9,-16,8,-7,-12,5,-4,-2,-15,14,6,-1,-11,10,3},
	{0,0,10,-8,-16,6,-5,-12,3,15,-2,-13,7,11,-1,-9,4},
	{0,0,11,7,-6,-16,4,-3,-12,-14,13,-2,8,-10,9,-1,5},
	{0,0,12,13,14,15,9,10,11,-6,-7,-8,-2,-3,-4,-5,-1}
};

#if 0

static void
combine_gammas(int h)
{
	int n;
	n = gp[(int) p1->gamma][(int) p2->gamma];
	if (n < 0) {
		n = -n;
		push(stack[h]);
		negate();
		stack[h] = pop();
	}
	if (n > 1)
		push(_gamma[n]);
}

#endif

void
multiply_noexpand(void)
{
	int x;
	x = expanding;
	expanding = 0;
	multiply();
	expanding = x;
}

// multiply n factors on stack

void
multiply_all(int n)
{
	int h, i;
	if (n == 1)
		return;
	if (n == 0) {
		push(one);
		return;
	}
	h = tos - n;
	push(stack[h]);
	for (i = 1; i < n; i++) {
		push(stack[h + i]);
		multiply();
	}
	stack[h] = pop();
	tos = h + 1;
}

void
multiply_all_noexpand(int n)
{
	int x;
	x = expanding;
	expanding = 0;
	multiply_all(n);
	expanding = x;
}

//-----------------------------------------------------------------------------
//
//	Symbolic division
//
//	Input:		Dividend and divisor on stack
//
//	Output:		Quotient on stack
//
//-----------------------------------------------------------------------------

void
divide(void)
{
	if (isnum(stack[tos - 2]) && isnum(stack[tos - 1]))
		divide_numbers();
	else {
		inverse();
		multiply();
	}
}

void
inverse(void)
{
	if (isnum(stack[tos - 1]))
		invert_number();
	else {
		push_integer(-1);
		power();
	}
}

void
reciprocate(void)
{
	if (isnum(stack[tos - 1]))
		invert_number();
	else {
		push_integer(-1);
		power();
	}
}

void
negate(void)
{
	if (isnum(stack[tos - 1]))
		negate_number();
	else {
		push_integer(-1);
		multiply();
	}
}

void
negate_expand(void)
{
	int x;
	x = expanding;
	expanding = 1;
	negate();
	expanding = x;
}

void
negate_noexpand(void)
{
	int x;
	x = expanding;
	expanding = 0;
	negate();
	expanding = x;
}

//-----------------------------------------------------------------------------
//
//	Normalize radical factors
//
//	Input:		stack[h]	Coefficient factor, possibly 1
//
//			stack[h + 1]	Second factor
//
//			stack[tos - 1]	Last factor
//
//	Output:		Reduced coefficent and normalized radicals (maybe)
//
//	Example:	2*2^(-1/2) -> 2^(1/2)
//
//	(power number number) is guaranteed to have the following properties:
//
//	1. Base is an integer
//
//	2. Absolute value of exponent < 1
//
//	These properties are assured by the power function.
//
//-----------------------------------------------------------------------------

#define A p1
#define B p2

#define BASE p3
#define EXPO p4

#define TMP p5

static int __is_radical_number(U *);

static void
__normalize_radical_factors(int h)
{
	int i;

	// if coeff is 1 or floating then don't bother

	if (isplusone(stack[h]) || isminusone(stack[h]) || isdouble(stack[h]))
		return;

	// if no radicals then don't bother

	for (i = h + 1; i < tos; i++)
		if (__is_radical_number(stack[i]))
			break;

	if (i == tos)
		return;

	// ok, try to simplify

	save();

	// numerator

	push(stack[h]);
	mp_numerator();
	A = pop();

	for (i = h + 1; i < tos; i++) {

		if (isplusone(A) || isminusone(A))
			break;

		if (!__is_radical_number(stack[i]))
			continue;

		BASE = cadr(stack[i]);
		EXPO = caddr(stack[i]);

		// exponent must be negative

		if (!isnegativenumber(EXPO))
			continue;

		// numerator divisible by BASE?

		push(A);
		push(BASE);
		divide();

		TMP = pop();

		if (!isinteger(TMP))
			continue;

		// reduce numerator

		A = TMP;

		// invert radical

		push_symbol(POWER);
		push(BASE);
		push(one);
		push(EXPO);
		add();
		list(3);
		stack[i] = pop();
	}

	// denominator

	push(stack[h]);
	mp_denominator();
	B = pop();

	for (i = h + 1; i < tos; i++) {

		if (isplusone(B))
			break;

		if (!__is_radical_number(stack[i]))
			continue;

		BASE = cadr(stack[i]);
		EXPO = caddr(stack[i]);

		// exponent must be positive

		if (isnegativenumber(EXPO))
			continue;

		// denominator divisible by BASE?

		push(B);
		push(BASE);
		divide();

		TMP = pop();

		if (!isinteger(TMP))
			continue;

		// reduce denominator

		B = TMP;

		// invert radical

		push_symbol(POWER);
		push(BASE);
		push(EXPO);
		push(one);
		subtract();
		list(3);
		stack[i] = pop();
	}

	// reconstitute the coefficient

	push(A);
	push(B);
	divide();
	stack[h] = pop();

	restore();
}

// don't include i

static int
__is_radical_number(U *p)
{
	// don't use i

	if (car(p) == symbol(POWER) && isnum(cadr(p)) && isnum(caddr(p)) && !isminusone(cadr(p)))
		return 1;
	else
		return 0;
}

//-----------------------------------------------------------------------------
//
//	> a*hilbert(2)
//	((a,1/2*a),(1/2*a,1/3*a))
//
//	Note that "a" is presumed to be a scalar. Is this correct?
//
//	Yes, because "*" has no meaning if "a" is a tensor.
//	To multiply tensors, "dot" or "outer" should be used.
//
//	> dot(a,hilbert(2))
//	dot(a,((1,1/2),(1/2,1/3)))
//
//	In this case "a" could be a scalar or tensor so the result is not
//	expanded.
//
//-----------------------------------------------------------------------------

#if SELFTEST

static char *s[] = {

	"0*a",
	"0",

	"a*0",
	"0",

	"1*a",
	"a",

	"a*1",
	"a",

	"a*a",
	"a^2",

	"a^2*a",
	"a^3",

	"a*a^2",
	"a^3",

	"a^2*a^2",
	"a^4",

	"2^a*2^(3-a)",		// symbolic exponents cancel
	"8",

	"sqrt(2)/2",
	"2^(-1/2)",

	"2/sqrt(2)",
	"2^(1/2)",

	"-sqrt(2)/2",
	"-1/(2^(1/2))",

	"2^(1/2-a)*2^a/10",
	"1/(5*2^(1/2))",

	"i/4",
	"1/4*i",

	"1/(4 i)",
	"-1/4*i",

	// ensure 1.0 is not discarded

	"1.0 pi 1/2",
	"0.5*pi",

	"1.0 1/2 pi",
	"0.5*pi",
};

void
test_multiply(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
