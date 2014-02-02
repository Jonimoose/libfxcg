// pretty print

#include "stdafx.h"
#include "defs.h"

void
bake(void)
{
	int h, s, t, x, y, z;

	expanding++;

	save();

	p1 = pop();

	s = ispoly(p1, symbol(SYMBOL_S));
	t = ispoly(p1, symbol(SYMBOL_T));
	x = ispoly(p1, symbol(SYMBOL_X));
	y = ispoly(p1, symbol(SYMBOL_Y));
	z = ispoly(p1, symbol(SYMBOL_Z));

	if (s == 1 && t == 0 && x == 0 && y == 0 && z == 0) {
		p2 = symbol(SYMBOL_S);
		bake_poly();
	} else if (s == 0 && t == 1 && x == 0 && y == 0 && z == 0) {
		p2 = symbol(SYMBOL_T);
		bake_poly();
	} else if (s == 0 && t == 0 && x == 1 && y == 0 && z == 0) {
		p2 = symbol(SYMBOL_X);
		bake_poly();
	} else if (s == 0 && t == 0 && x == 0 && y == 1 && z == 0) {
		p2 = symbol(SYMBOL_Y);
		bake_poly();
	} else if (s == 0 && t == 0 && x == 0 && y == 0 && z == 1) {
		p2 = symbol(SYMBOL_Z);
		bake_poly();
	} else if (iscons(p1)) {
		h = tos;
		push(car(p1));
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			bake();
			p1 = cdr(p1);
		}
		list(tos - h);
	} else
		push(p1);

	restore();

	expanding--;
}

void
polyform(void)
{
	int h;

	save();

	p2 = pop();
	p1 = pop();

	if (ispoly(p1, p2))
		bake_poly();
	else if (iscons(p1)) {
		h = tos;
		push(car(p1));
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			polyform();
			p1 = cdr(p1);
		}
		list(tos - h);
	} else
		push(p1);

	restore();
}

void
bake_poly()
{
	int h, i, k, n;
	U **a;
	a = stack + tos;
	push(p1);		// p(x)
	push(p2);		// x
	k = coeff();
	h = tos;
	for (i = k - 1; i >= 0; i--) {
		p1 = a[i];
		bake_poly_term(i);
	}
	n = tos - h;
	if (n > 1) {
		list(n);
		push(symbol(ADD));
		swap();
		cons();
	}
	p1 = pop();
	tos -= k;
	push(p1);
}

// p1 points to coefficient of p2 ^ k

void
bake_poly_term(int k)
{
	int h, n;

	if (iszero(p1))
		return;

	// constant term?

	if (k == 0) {
		if (car(p1) == symbol(ADD)) {
			p1 = cdr(p1);
			while (iscons(p1)) {
				push(car(p1));
				p1 = cdr(p1);
			}
		} else
			push(p1);
		return;
	}

	h = tos;

	// coefficient

	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			p1 = cdr(p1);
		}
	} else if (!equaln(p1, 1))
		push(p1);

	// x ^ k

	if (k == 1)
		push(p2);
	else {
		push(symbol(POWER));
		push(p2);
		push_integer(k);
		list(3);
	}

	n = tos - h;

	if (n > 1) {
		list(n);
		push(symbol(MULTIPLY));
		swap();
		cons();
	}
}

#if SELFTEST

static char *s[] = {

	"(x+3)^3",
	"x^3+9*x^2+27*x+27",

	"factor",
	"(x+3)^3",
};

void
test_bake(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
