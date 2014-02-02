/* Power function

	Input:		push	Base

			push	Exponent

	Output:		Result on stack
*/

#include "stdafx.h"
#include "defs.h"

void
eval_power(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	power();
}

void
power(void)
{
	save();
	yypower();
	restore();
}

void
yypower(void)
{
	int n;

	p2 = pop();
	p1 = pop();

	// both base and exponent are rational numbers?

	if (isrational(p1) && isrational(p2)) {
		push(p1);
		push(p2);
		qpow();
		return;
	}

	// both base and exponent are either rational or double?

	if (isnum(p1) && isnum(p2)) {
		push(p1);
		push(p2);
		dpow();
		return;
	}

	if (istensor(p1)) {
		power_tensor();
		return;
	}

	if (p1 == symbol(E) && car(p2) == symbol(LOG)) {
		push(cadr(p2));
		return;
	}

	if (p1 == symbol(E) && isdouble(p2)) {
		push_double(exp(p2->u.d));
		return;
	}

	//	1 ^ a		->	1

	//	a ^ 0		->	1

	if (equal(p1, one) || iszero(p2)) {
		push(one);
		return;
	}

	//	a ^ 1		->	a

	if (equal(p2, one)) {
		push(p1);
		return;
	}

	//	(a * b) ^ c	->	(a ^ c) * (b ^ c)

	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		push(car(p1));
		push(p2);
		power();
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			power();
			multiply();
			p1 = cdr(p1);
		}
		return;
	}

	//	(a ^ b) ^ c	->	a ^ (b * c)

	if (car(p1) == symbol(POWER)) {
		push(cadr(p1));
		push(caddr(p1));
		push(p2);
		multiply();
		power();
		return;
	}

	//	(a + b) ^ n	->	(a + b) * (a + b) ...

	if (expanding && isadd(p1) && isnum(p2)) {
		push(p2);
		n = pop_integer();
		if (n > 1) {
			power_sum(n);
			return;
		}
	}

	//	sin(x) ^ 2n -> (1 - cos(x) ^ 2) ^ n

	if (trigmode == 1 && car(p1) == symbol(SIN) && iseveninteger(p2)) {
		push_integer(1);
		push(cadr(p1));
		cosine();
		push_integer(2);
		power();
		subtract();
		push(p2);
		push_rational(1, 2);
		multiply();
		power();
		return;
	}

	//	cos(x) ^ 2n -> (1 - sin(x) ^ 2) ^ n

	if (trigmode == 2 && car(p1) == symbol(COS) && iseveninteger(p2)) {
		push_integer(1);
		push(cadr(p1));
		sine();
		push_integer(2);
		power();
		subtract();
		push(p2);
		push_rational(1, 2);
		multiply();
		power();
		return;
	}

	// complex number? (just number, not expression)

	if (iscomplexnumber(p1)) {

		// integer power?

		// n will be negative here, positive n already handled

		if (isinteger(p2)) {

			//               /        \  n
			//         -n   |  a - ib  |
			// (a + ib)   = | -------- |
			//              |   2   2  |
			//               \ a + b  /

			push(p1);
			conjugate();
			p3 = pop();
			push(p3);
			push(p3);
			push(p1);
			multiply();
			divide();
			push(p2);
			negate();
			power();
			return;
		}

		// noninteger or floating power?

		if (isnum(p2)) {

#if 1			// use polar form
			push(p1);
			mag();
			push(p2);
			power();
			push_integer(-1);
			push(p1);
			arg();
			push(p2);
			multiply();
			push(symbol(PI));
			divide();
			power();
			multiply();

#else			// use exponential form
			push(p1);
			mag();
			push(p2);
			power();
			push(symbol(E));
			push(p1);
			arg();
			push(p2);
			multiply();
			push(imaginaryunit);
			multiply();
			power();
			multiply();
#endif
			return;
		}
	}

	if (simplify_polar())
		return;

	push_symbol(POWER);
	push(p1);
	push(p2);
	list(3);
}

//-----------------------------------------------------------------------------
//
//	Compute the power of a sum
//
//	Input:		p1	sum
//
//			n	exponent
//
//	Output:		Result on stack
//
//	Note:
//
//	Uses the multinomial series (see Math World)
//
//                          n              n!          n1   n2       nk
//	(a1 + a2 + ... + ak)  = sum (--------------- a1   a2   ... ak  )
//	                             n1! n2! ... nk!
//
//	The sum is over all n1 ... nk such that n1 + n2 + ... + nk = n.
//
//-----------------------------------------------------------------------------

// first index is the term number 0..k-1, second index is the exponent 0..n

#define A(i, j) frame[(i) * (n + 1) + (j)]

void
power_sum(int n)
{
	int *a, i, j, k;

	// number of terms in the sum

	k = length(p1) - 1;

	// local frame

	push_frame(k * (n + 1));

	// array of powers

	p1 = cdr(p1);
	for (i = 0; i < k; i++) {
		for (j = 0; j <= n; j++) {
			push(car(p1));
			push_integer(j);
			power();
			A(i, j) = pop();
		}
		p1 = cdr(p1);
	}

	push_integer(n);
	factorial();
	p1 = pop();

	a = (int *) malloc(k * sizeof (int));

	if (a == NULL)
		stop("malloc failure");

	push(zero);

	multinomial_sum(k, n, a, 0, n);

	free(a);

	pop_frame(k * (n + 1));
}

//-----------------------------------------------------------------------------
//
//	Compute multinomial sum
//
//	Input:		k	number of factors
//
//			n	overall exponent
//
//			a	partition array
//
//			i	partition array index
//
//			m	partition remainder
//
//			p1	n!
//
//			A	factor array
//
//	Output:		Result on stack
//
//	Note:
//
//	Uses recursive descent to fill the partition array.
//
//-----------------------------------------------------------------------------

void
multinomial_sum(int k, int n, int *a, int i, int m)
{
	int j;

	if (i < k - 1) {
		for (j = 0; j <= m; j++) {
			a[i] = j;
			multinomial_sum(k, n, a, i + 1, m - j);
		}
		return;
	}

	a[i] = m;

	// coefficient

	push(p1);

	for (j = 0; j < k; j++) {
		push_integer(a[j]);
		factorial();
		divide();
	}

	// factors

	for (j = 0; j < k; j++) {
		push(A(j, a[j]));
		multiply();
	}

	add();
}

// exp(n/2 i pi) ?

// p2 is the exponent expression

// clobbers p3

int
simplify_polar(void)
{
	int n;

	n = isquarterturn(p2);

	switch(n) {
	case 0:
		break;
	case 1:
		push_integer(1);
		return 1;
	case 2:
		push_integer(-1);
		return 1;
	case 3:
		push(imaginaryunit);
		return 1;
	case 4:
		push(imaginaryunit);
		negate();
		return 1;
	}

	if (car(p2) == symbol(ADD)) {
		p3 = cdr(p2);
		while (iscons(p3)) {
			n = isquarterturn(car(p3));
			if (n)
				break;
			p3 = cdr(p3);
		}
		switch (n) {
		case 0:
			return 0;
		case 1:
			push_integer(1);
			break;
		case 2:
			push_integer(-1);
			break;
		case 3:
			push(imaginaryunit);
			break;
		case 4:
			push(imaginaryunit);
			negate();
			break;
		}
		push(p2);
		push(car(p3));
		subtract();
		exponential();
		multiply();
		return 1;
	}

	return 0;
}

#if SELFTEST

static char *s[] = {

	"2^(1/2)",
	"2^(1/2)",

	"2^(3/2)",
	"2*2^(1/2)",

	"(-2)^(1/2)",
	"i*2^(1/2)",

	"3^(4/3)",
	"3*3^(1/3)",

	"3^(-4/3)",
	"1/(3*3^(1/3))",

	"3^(5/3)",
	"3*3^(2/3)",

	"3^(2/3)-9^(1/3)",
	"0",

	"3^(10/3)",
	"27*3^(1/3)",

	"3^(-10/3)",
	"1/(27*3^(1/3))",

	"(1/3)^(10/3)",
	"1/(27*3^(1/3))",

	"(1/3)^(-10/3)",
	"27*3^(1/3)",

	"27^(2/3)",
	"9",

	"27^(-2/3)",
	"1/9",

	"102^(1/2)",
	"2^(1/2)*3^(1/2)*17^(1/2)",

	"32^(1/3)",
	"2*2^(2/3)",

	"9999^(1/2)",
	"3*11^(1/2)*101^(1/2)",

	"10000^(1/3)",
	"10*2^(1/3)*5^(1/3)",

	"sqrt(1000000)",
	"1000",

	"sqrt(-1000000)",
	"1000*i",

	"sqrt(2^60)",
	"1073741824",

	// this is why we factor irrationals

	"6^(1/3) 3^(2/3)",
	"3*2^(1/3)",

	// inverse of complex numbers

	"1/(2+3*i)",
	"2/13-3/13*i",

	"1/(2+3*i)^2",
	"-5/169-12/169*i",

	"(-1+3i)/(2-i)",
	"-1+i",

	// other

	"(0.0)^(0.0)",
	"1",

	"(-4.0)^(1.5)",
	"-8*i",

	"(-4.0)^(0.5)",
	"2*i",

	"(-4.0)^(-0.5)",
	"-0.5*i",

	"(-4.0)^(-1.5)",
	"0.125*i",

	// more complex number cases

	"(1+i)^2",
	"2*i",

	"(1+i)^(-2)",
	"-1/2*i",

	"(1+i)^(1/2)",
	"(-1)^(1/8)*2^(1/4)",

	"(1+i)^(-1/2)",
	"-(-1)^(7/8)/(2^(1/4))",

	"(1+i)^(0.5)",
	"1.09868+0.45509*i",

	"(1+i)^(-0.5)",
	"0.776887-0.321797*i",

	// test cases for simplification of polar forms, counterclockwise

	"exp(i*pi/2)",
	"i",

	"exp(i*pi)",
	"-1",

	"exp(i*3*pi/2)",
	"-i",

	"exp(i*2*pi)",
	"1",

	"exp(i*5*pi/2)",
	"i",

	"exp(i*3*pi)",
	"-1",

	"exp(i*7*pi/2)",
	"-i",

	"exp(i*4*pi)",
	"1",

	"exp(A+i*pi/2)",
	"i*exp(A)",

	"exp(A+i*pi)",
	"-exp(A)",

	"exp(A+i*3*pi/2)",
	"-i*exp(A)",

	"exp(A+i*2*pi)",
	"exp(A)",

	"exp(A+i*5*pi/2)",
	"i*exp(A)",

	"exp(A+i*3*pi)",
	"-exp(A)",

	"exp(A+i*7*pi/2)",
	"-i*exp(A)",

	"exp(A+i*4*pi)",
	"exp(A)",

	// test cases for simplification of polar forms, clockwise

	"exp(-i*pi/2)",
	"-i",

	"exp(-i*pi)",
	"-1",

	"exp(-i*3*pi/2)",
	"i",

	"exp(-i*2*pi)",
	"1",

	"exp(-i*5*pi/2)",
	"-i",

	"exp(-i*3*pi)",
	"-1",

	"exp(-i*7*pi/2)",
	"i",

	"exp(-i*4*pi)",
	"1",

	"exp(A-i*pi/2)",
	"-i*exp(A)",

	"exp(A-i*pi)",
	"-exp(A)",

	"exp(A-i*3*pi/2)",
	"i*exp(A)",

	"exp(A-i*2*pi)",
	"exp(A)",

	"exp(A-i*5*pi/2)",
	"-i*exp(A)",

	"exp(A-i*3*pi)",
	"-exp(A)",

	"exp(A-i*7*pi/2)",
	"i*exp(A)",

	"exp(A-i*4*pi)",
	"exp(A)",
};

void
test_power(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
