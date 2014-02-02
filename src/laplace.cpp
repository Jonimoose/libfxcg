// Laplace transform

#include "stdafx.h"
#include "defs.h"

void
eval_laplace(void)
{
	push(cadr(p1));
	eval();
	push(symbol(SYMBOL_T));
	laplace();
}

#define F p3
#define T p4
#define A p5

void
laplace(void)
{
	int h;
	save();

	T = pop();
	F = pop();

	// L[f + g] = L[f] + L[g]

	if (car(F) == symbol(ADD)) {
		p1 = cdr(F);
		h = tos;
		while (iscons(p1)) {
			push(car(p1));
			push(T);
			laplace();
			p1 = cdr(p1);
		}
		add_all(tos - h);
		restore();
		return;
	}

	// L[Af] = A L[f]

	if (car(F) == symbol(MULTIPLY)) {
		push(F);
		push(T);
		partition();
		F = pop();
		A = pop();
		laplace_main();
		push(A);
		multiply();
	} else
		laplace_main();

	restore();
}

void
laplace_main(void)
{
	int n;

	// L[t] = 1 / s^2

	if (F == symbol(SYMBOL_T)) {
		push_symbol(SYMBOL_S);
		push_integer(-2);
		power();
		return;
	}

	// L[t^n] = n! / s^(n+1)

	if (car(F) == symbol(POWER) && cadr(F) == T) {
		push(caddr(F));
		n = pop_integer();
		if (n > 0) {
			push_integer(n);
			factorial();
			push_symbol(SYMBOL_S);
			push_integer(n + 1);
			power();
			divide();
			return;
		}
	}

	stop("laplace: cannot solve");
}

#if SELFTEST

static char *s[] = {

// float ok?

	"laplace(3t^2.0)",
	"6/(s^3)",
};

void
test_laplace(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
