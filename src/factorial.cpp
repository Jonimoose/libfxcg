#include "stdafx.h"
#include "defs.h"

extern void bignum_factorial(int);

void
factorial(void)
{
	int n;
	save();
	p1 = pop();
	push(p1);
	n = pop_integer();
	if (n < 0 || n == (int) 0x80000000) {
		push_symbol(FACTORIAL);
		push(p1);
		list(2);
		restore();
		return;
	}
	bignum_factorial(n);
	restore();
}

void sfac_product(void);
void sfac_product_f(U **, int, int);

// simplification rules for factorials (m < n)
//
//	(e + 1) * factorial(e)	->	factorial(e + 1)
//
//	factorial(e) / e	->	factorial(e - 1)
//
//	e / factorial(e)	->	1 / factorial(e - 1)
//
//	factorial(e + n)
//	----------------	->	(e + m + 1)(e + m + 2)...(e + n)
//	factorial(e + m)
//
//	factorial(e + m)                               1
//	----------------	->	--------------------------------
//	factorial(e + n)		(e + m + 1)(e + m + 2)...(e + n)

void
simplifyfactorials(void)
{
	int x;

	save();

	x = expanding;
	expanding = 0;

	p1 = pop();

	if (car(p1) == symbol(ADD)) {
		push(zero);
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			simplifyfactorials();
			add();
			p1 = cdr(p1);
		}
		expanding = x;
		restore();
		return;
	}

	if (car(p1) == symbol(MULTIPLY)) {
		sfac_product();
		expanding = x;
		restore();
		return;
	}

	push(p1);

	expanding = x;
	restore();
}

void
sfac_product(void)
{
	int i, j, n;
	U **s;

	s = stack + tos;

	p1 = cdr(p1);
	n = 0;
	while (iscons(p1)) {
		push(car(p1));
		p1 = cdr(p1);
		n++;
	}

	for (i = 0; i < n - 1; i++) {
		if (s[i] == symbol(NIL))
			continue;
		for (j = i + 1; j < n; j++) {
			if (s[j] == symbol(NIL))
				continue;
			sfac_product_f(s, i, j);
		}
	}

	push(one);

	for (i = 0; i < n; i++) {
		if (s[i] == symbol(NIL))
			continue;
		push(s[i]);
		multiply();
	}

	p1 = pop();

	tos -= n;

	push(p1);
}

void
sfac_product_f(U **s, int a, int b)
{
	int i, n;

	p1 = s[a];
	p2 = s[b];

	if (ispower(p1)) {
		p3 = caddr(p1);
		p1 = cadr(p1);
	} else
		p3 = one;

	if (ispower(p2)) {
		p4 = caddr(p2);
		p2 = cadr(p2);
	} else
		p4 = one;

	if (isfactorial(p1) && isfactorial(p2)) {

		// Determine if the powers cancel.

		push(p3);
		push(p4);
		add();
		yyexpand();
		n = pop_integer();
		if (n != 0)
			return;

		// Find the difference between the two factorial args.

		// For example, the difference between (a + 2)! and a! is 2.

		push(cadr(p1));
		push(cadr(p2));
		subtract();
		yyexpand(); // to simplify

		n = pop_integer();
		if (n == 0 || n == (int) 0x80000000)
			return;
		if (n < 0) {
			n = -n;
			p5 = p1;
			p1 = p2;
			p2 = p5;
			p5 = p3;
			p3 = p4;
			p4 = p5;
		}

		push(one);

		for (i = 1; i <= n; i++) {
			push(cadr(p2));
			push_integer(i);
			add();
			push(p3);
			power();
			multiply();
		}
		s[a] = pop();
		s[b] = symbol(NIL);
	}
}
