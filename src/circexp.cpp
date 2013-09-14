// Change circular functions to exponentials

#include "stdafx.h"
#include "defs.h"

void
eval_circexp(void)
{
	push(cadr(p1));
	eval();

	circexp();

	// normalize

	eval();
}

void
circexp(void)
{
	int i, h;
	save();
	p1 = pop();

	if (car(p1) == symbol(COS)) {
		push(cadr(p1));
		expcos();
		restore();
		return;
	}

	if (car(p1) == symbol(SIN)) {
		push(cadr(p1));
		expsin();
		restore();
		return;
	}

	if (car(p1) == symbol(TAN)) {
		p1 = cadr(p1);
		push(imaginaryunit);
		push(p1);
		multiply();
		exponential();
		p2 = pop();
		push(imaginaryunit);
		push(p1);
		multiply();
		negate();
		exponential();
		p3 = pop();
		push(p3);
		push(p2);
		subtract();
		push(imaginaryunit);
		multiply();
		push(p2);
		push(p3);
		add();
		divide();
		restore();
		return;
	}

	if (car(p1) == symbol(COSH)) {
		p1 = cadr(p1);
		push(p1);
		exponential();
		push(p1);
		negate();
		exponential();
		add();
		push_rational(1, 2);
		multiply();
		restore();
		return;
	}

	if (car(p1) == symbol(SINH)) {
		p1 = cadr(p1);
		push(p1);
		exponential();
		push(p1);
		negate();
		exponential();
		subtract();
		push_rational(1, 2);
		multiply();
		restore();
		return;
	}

	if (car(p1) == symbol(TANH)) {
		p1 = cadr(p1);
		push(p1);
		push_integer(2);
		multiply();
		exponential();
		p1 = pop();
		push(p1);
		push_integer(1);
		subtract();
		push(p1);
		push_integer(1);
		add();
		divide();
		restore();
		return;
	}

	if (iscons(p1)) {
		h = tos;
		while (iscons(p1)) {
			push(car(p1));
			circexp();
			p1 = cdr(p1);
		}
		list(tos - h);
		restore();
		return;
	}

	if (p1->k == TENSOR) {
		push(p1);
		copy_tensor();
		p1 = pop();
		for (i = 0; i < p1->u.tensor->nelem; i++) {
			push(p1->u.tensor->elem[i]);
			circexp();
			p1->u.tensor->elem[i] = pop();
		}
		push(p1);
		restore();
		return;
	}

	push(p1);
	restore();
}

#if SELFTEST

static char *s[] = {

	"circexp(cos(x))",
	"1/2*exp(-i*x)+1/2*exp(i*x)",

	"circexp(sin(x))",
	"1/2*i*exp(-i*x)-1/2*i*exp(i*x)",

	"circexp(tan(x))",
	"i*exp(-i*x)/(exp(-i*x)+exp(i*x))-i*exp(i*x)/(exp(-i*x)+exp(i*x))",

	"circexp(cosh(x))",
	"1/2*exp(x)+1/2*exp(-x)",

	"circexp(sinh(x))",
	"1/2*exp(x)-1/2*exp(-x)",

	"circexp(tanh(x))",
	"-1/(1+exp(2*x))+exp(2*x)/(1+exp(2*x))",

	"circexp((cos(x),sin(x)))",
	"(1/2*exp(-i*x)+1/2*exp(i*x),1/2*i*exp(-i*x)-1/2*i*exp(i*x))",

	"circexp(cos(x)*sin(x))-expcos(x)*expsin(x)",
	"0",
};

void
test_circexp(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
