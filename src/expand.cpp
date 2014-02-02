// Partial fraction expansion
//
// Example
//
//      expand(1/(x^3+x^2),x)
//
//        1      1       1
//      ---- - --- + -------
//        2     x     x + 1
//       x

#include "stdafx.h"
#include "defs.h"

void
eval_expand(void)
{
	// 1st arg

	push(cadr(p1));
	eval();

	// 2nd arg

	push(caddr(p1));
	eval();

	p2 = pop();
	if (p2 == symbol(NIL))
		guess();
	else
		push(p2);

	expand();
}

#define A p2
#define B p3
#define C p4
#define F p5
#define P p6
#define Q p7
#define T p8
#define X p9

void
expand(void)
{
	save();

	X = pop();
	F = pop();

	if (istensor(F)) {
		expand_tensor();
		restore();
		return;
	}

	// if sum of terms then sum over the expansion of each term

	if (car(F) == symbol(ADD)) {
		push_integer(0);
		p1 = cdr(F);
		while (iscons(p1)) {
			push(car(p1));
			push(X);
			expand();
			add();
			p1 = cdr(p1);
		}
		restore();
		return;
	}

	// B = numerator

	push(F);
	numerator();
	B = pop();

	// A = denominator

	push(F);
	denominator();
	A = pop();

	remove_negative_exponents();

	// Q = quotient

	push(B);
	push(A);
	push(X);
	divpoly();
	Q = pop();

	// remainder B = B - A * Q

	push(B);
	push(A);
	push(Q);
	multiply();
	subtract();
	B = pop();

	// if the remainder is zero then we're done

	if (iszero(B)) {
		push(Q);
		restore();
		return;
	}

	// A = factor(A)

	push(A);
	push(X);
	factorpoly();
	A = pop();

	expand_get_C();
	expand_get_B();
	expand_get_A();

	if (istensor(C)) {
		push(C);
		inv();
		push(B);
		inner();
		push(A);
		inner();
	} else {
		push(B);
		push(C);
		divide();
		push(A);
		multiply();
	}

	push(Q);
	add();

	restore();
}

void
expand_tensor(void)
{
	int i;
	push(F);
	copy_tensor();
	F = pop();
	for (i = 0; i < F->u.tensor->nelem; i++) {
		push(F->u.tensor->elem[i]);
		push(X);
		expand();
		F->u.tensor->elem[i] = pop();
	}
	push(F);
}

void
remove_negative_exponents(void)
{
	int h, i, j, k, n;

	h = tos;
	factors(A);
	factors(B);
	n = tos - h;

	// find the smallest exponent

	j = 0;
	for (i = 0; i < n; i++) {
		p1 = stack[h + i];
		if (car(p1) != symbol(POWER))
			continue;
		if (cadr(p1) != X)
			continue;
		push(caddr(p1));
		k = pop_integer();
		if (k == (int) 0x80000000)
			continue;
		if (k < j)
			j = k;
	}

	tos = h;

	if (j == 0)
		return;

	// A = A / X^j

	push(A);
	push(X);
	push_integer(-j);
	power();
	multiply();
	A = pop();

	// B = B / X^j

	push(B);
	push(X);
	push_integer(-j);
	power();
	multiply();
	B = pop();
}

// Returns the expansion coefficient matrix C.
//
// Example:
//
//       B         1
//      --- = -----------
//       A      2 
//             x (x + 1)
//
// We have
//
//       B     Y1     Y2      Y3
//      --- = ---- + ---- + -------
//       A      2     x      x + 1
//             x
//
// Our task is to solve for the unknowns Y1, Y2, and Y3.
//
// Multiplying both sides by A yields
//
//           AY1     AY2      AY3
//      B = ----- + ----- + -------
//            2      x       x + 1
//           x
//
// Let
//
//            A               A                 A
//      W1 = ----       W2 = ---        W3 = -------
//             2              x               x + 1
//            x
//
// Then the coefficient matrix C is
//
//              coeff(W1,x,0)   coeff(W2,x,0)   coeff(W3,x,0)
//
//       C =    coeff(W1,x,1)   coeff(W2,x,1)   coeff(W3,x,1)
//
//              coeff(W1,x,2)   coeff(W2,x,2)   coeff(W3,x,2)
//
// It follows that
//
//       coeff(B,x,0)     Y1
//
//       coeff(B,x,1) = C Y2
//
//       coeff(B,x,2) =   Y3
//
// Hence
//
//       Y1       coeff(B,x,0)
//             -1
//       Y2 = C   coeff(B,x,1)
//
//       Y3       coeff(B,x,2)

void
expand_get_C(void)
{
	int h, i, j, n;
	U **a;
	h = tos;
	if (car(A) == symbol(MULTIPLY)) {
		p1 = cdr(A);
		while (iscons(p1)) {
			F = car(p1);
			expand_get_CF();
			p1 = cdr(p1);
		}
	} else {
		F = A;
		expand_get_CF();
	}
	n = tos - h;
	if (n == 1) {
		C = pop();
		return;
	}
	C = alloc_tensor(n * n);
	C->u.tensor->ndim = 2;
	C->u.tensor->dim[0] = n;
	C->u.tensor->dim[1] = n;
	a = stack + h;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			push(a[j]);
			push(X);
			push_integer(i);
			power();
			divide();
			push(X);
			filter();
			C->u.tensor->elem[n * i + j] = pop();
		}
	}
	tos -= n;
}

// The following table shows the push order for simple roots, repeated roots,
// and inrreducible factors.
//
//  Factor F        Push 1st        Push 2nd         Push 3rd      Push 4th
//
//
//                   A
//  x               ---
//                   x
//
//
//   2               A               A
//  x               ----            ---
//                    2              x
//                   x
//
//
//                     A
//  x + 1           -------
//                   x + 1
//
//
//         2            A              A
//  (x + 1)         ----------      -------
//                          2        x + 1
//                   (x + 1)
//
//
//   2                   A               Ax
//  x  + x + 1      ------------    ------------
//                    2               2
//                   x  + x + 1      x  + x + 1
//
//
//    2         2          A              Ax              A             Ax
//  (x  + x + 1)    --------------- ---------------  ------------  ------------
//                     2         2     2         2     2             2
//                   (x  + x + 1)    (x  + x + 1)     x  + x + 1    x  + x + 1
//
//
// For T = A/F and F = P^N we have
//
//
//      Factor F          Push 1st    Push 2nd    Push 3rd    Push 4th
//
//      x                 T
//
//       2
//      x                 T           TP
//
//
//      x + 1             T
//
//             2
//      (x + 1)           T           TP
//
//       2
//      x  + x + 1        T           TX
//
//        2         2
//      (x  + x + 1)      T           TX          TP          TPX
//
//
// Hence we want to push in the order
//
//      T * (P ^ i) * (X ^ j)
//
// for all i, j such that
//
//      i = 0, 1, ..., N - 1
//
//      j = 0, 1, ..., deg(P) - 1
//
// where index j runs first.

void
expand_get_CF(void)
{	int d, i, j, n;
	if (!find(F, X))
		return;
	trivial_divide();
	if (car(F) == symbol(POWER)) {
		push(caddr(F));
		n = pop_integer();
		P = cadr(F);
	} else {
		n = 1;
		P = F;
	}
	push(P);
	push(X);
	degree();
	d = pop_integer();
	for (i = 0; i < n; i++) {
		for (j = 0; j < d; j++) {
			push(T);
			push(P);
			push_integer(i);
			power();
			multiply();
			push(X);
			push_integer(j);
			power();
			multiply();
		}
	}
}

// Returns T = A/F where F is a factor of A.

void
trivial_divide(void)
{
	int h;
	if (car(A) == symbol(MULTIPLY)) {
		h = tos;
		p0 = cdr(A);
		while (iscons(p0)) {
			if (!equal(car(p0), F)) {
				push(car(p0));
				eval(); // force expansion of (x+1)^2, f.e.
			}
			p0 = cdr(p0);
		}
		multiply_all(tos - h);
	} else
		push_integer(1);
	T = pop();
}

// Returns the expansion coefficient vector B.

void
expand_get_B(void)
{
	int i, n;
	if (!istensor(C))
		return;
	n = C->u.tensor->dim[0];
	T = alloc_tensor(n);
	T->u.tensor->ndim = 1;
	T->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++) {
		push(B);
		push(X);
		push_integer(i);
		power();
		divide();
		push(X);
		filter();
		T->u.tensor->elem[i] = pop();
	}
	B = T;
}

// Returns the expansion fractions in A.

void
expand_get_A(void)
{
	int h, i, n;
	if (!istensor(C)) {
		push(A);
		reciprocate();
		A = pop();
		return;
	}
	h = tos;
	if (car(A) == symbol(MULTIPLY)) {
		T = cdr(A);
		while (iscons(T)) {
			F = car(T);
			expand_get_AF();
			T = cdr(T);
		}
	} else {
		F = A;
		expand_get_AF();
	}
	n = tos - h;
	T = alloc_tensor(n);
	T->u.tensor->ndim = 1;
	T->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++)
		T->u.tensor->elem[i] = stack[h + i];
	tos = h;
	A = T;
}

void
expand_get_AF(void)
{	int d, i, j, n = 1;
	if (!find(F, X))
		return;
	if (car(F) == symbol(POWER)) {
		push(caddr(F));
		n = pop_integer();
		F = cadr(F);
	}
	push(F);
	push(X);
	degree();
	d = pop_integer();
	for (i = n; i > 0; i--) {
		for (j = 0; j < d; j++) {
			push(F);
			push_integer(i);
			power();
			reciprocate();
			push(X);
			push_integer(j);
			power();
			multiply();
		}
	}
}

#if SELFTEST

static char *s[] = {

	// general cases

	"expand(1/(x+1)/(x+2))",
	"1/(x+1)-1/(x+2)",

	"expand((2x^3-x+2)/(x^2-2x+1))",
	"4+2*x+5/(x-1)+3/(x^2-2*x+1)",

	"expand(1/x^2/(x-1))",
	"-1/(x^2)-1/x+1/(x-1)",

	"p=5s+2",
	"",

	"q=(s+1)(s+2)^2",
	"",

	"expand(p/q)",
	"-3/(s+1)+3/(s+2)+8/(s^2+4*s+4)",

	// ensure denominators are expanded (result seems preferable that way)

	"q=(x-1)(x-2)^3",
	"",

	"expand(1/q)",
	"1/(x^3-6*x^2+12*x-8)+1/(x-2)-1/(x-1)-1/(x^2-4*x+4)",

	// fractional poles

	"expand(1/(x+1/2)/(x+1/3))",
	"-12/(2*x+1)+18/(3*x+1)",

	// expand tensor

	"f=1/(x+1)/(x+2)",
	"",

	"g=1/(x+1)-1/(x+2)",
	"",

	"expand(((f,f),(f,f)))-((g,g),(g,g))",
	"((0,0),(0,0))",

	// denominator normalized?

	"expand(1/(1+1/x))",
	"1-1/(x+1)",

	// poles at zero

	"expand(1/x/(x+1))",
	"1/x-1/(x+1)",

	"expand(1/x^2/(x+1))",
	"x^(-2)-1/x+1/(x+1)",

	// other corner cases

	"expand(1/x)",
	"1/x",

	"expand(1/x^2)",
	"x^(-2)",

	"expand(1/(x^2-4x+4))",
	"1/(x^2-4*x+4)",
};

void
test_expand(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
