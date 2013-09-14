#include "stdafx.h"
#include "defs.h"
#define DEBUG 0
static void __rationalize_tensor(void);
static void multiply_denominators(U *);
static void multiply_denominators_term(U *);
static void multiply_denominators_factor(U *);
static void __lcm(void);

void
eval_rationalize(void)
{
	push(cadr(p1));
	eval();
	rationalize();
}

void
rationalize(void)
{
	int x = expanding;
	save();
	yyrationalize();
	restore();
	expanding = x;
}

void
yyrationalize(void)
{
	p1 = pop();

	if (istensor(p1)) {
		__rationalize_tensor();
		return;
	}

	expanding = 0;

	if (car(p1) != symbol(ADD)) {
		push(p1);
		return;
	}

#if DEBUG
	printf("rationalize: this is the input expr:\n");
	printline(p1);
#endif

	// get common denominator

	push(one);
	multiply_denominators(p1);
	p2 = pop();

#if DEBUG
	printf("rationalize: this is the common denominator:\n");
	printline(p2);
#endif

	// multiply each term by common denominator

	push(zero);
	p3 = cdr(p1);
	while (iscons(p3)) {
		push(p2);
		push(car(p3));
		multiply();
		add();
		p3 = cdr(p3);
	}

#if DEBUG
	printf("rationalize: original expr times common denominator:\n");
	printline(stack[tos - 1]);
#endif

	// collect common factors

	Condense();

#if DEBUG
	printf("rationalize: after factoring:\n");
	printline(stack[tos - 1]);
#endif

	// divide by common denominator

	push(p2);
	divide();

#if DEBUG
	printf("rationalize: after dividing by common denom. (and we're done):\n");
	printline(stack[tos - 1]);
#endif
}

static void
multiply_denominators(U *p)
{
	if (car(p) == symbol(ADD)) {
		p = cdr(p);
		while (iscons(p)) {
			multiply_denominators_term(car(p));
			p = cdr(p);
		}
	} else
		multiply_denominators_term(p);
}

static void
multiply_denominators_term(U *p)
{
	if (car(p) == symbol(MULTIPLY)) {
		p = cdr(p);
		while (iscons(p)) {
			multiply_denominators_factor(car(p));
			p = cdr(p);
		}
	} else
		multiply_denominators_factor(p);
}

static void
multiply_denominators_factor(U *p)
{
	if (car(p) != symbol(POWER))
		return;

	push(p);

	p = caddr(p);

	// like x^(-2) ?

	if (isnegativenumber(p)) {
		inverse();
		__lcm();
		return;
	}

	// like x^(-a) ?

	if (car(p) == symbol(MULTIPLY) && isnegativenumber(cadr(p))) {
		inverse();
		__lcm();
		return;
	}

	// no match

	pop();
}

static void
__rationalize_tensor(void)
{
	int i, n;

	push(p1);

	eval(); // makes a copy

	p1 = pop();

	if (!istensor(p1)) { // might be zero
		push(p1);
		return;
	}

	n = p1->u.tensor->nelem;

	for (i = 0; i < n; i++) {
		push(p1->u.tensor->elem[i]);
		rationalize();
		p1->u.tensor->elem[i] = pop();
	}

	push(p1);
}

#if SELFTEST

static char *s[] = {

	"rationalize(a/b+c/d)",
	"(a*d+b*c)/(b*d)",

	"rationalize(t*y/(t+y)+2*t^2*y*(2*t+y)^(-2))",
	"t*y*(6*t^2+y^2+6*t*y)/((t+y)*(2*t+y)^2)",

	"rationalize(x^(-2*a)+x^(-4*a))",
	"(1+x^(2*a))/(x^(4*a))",

	"rationalize(x^(1/3)+x^(2/3))",
	"x^(1/3)*(1+x^(1/3))",
};

void
test_rationalize(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif

static void
__lcm(void)
{
	save();

	p1 = pop();
	p2 = pop();

	push(p1);
	push(p2);
	multiply();
	push(p1);
	push(p2);
	gcd();
	divide();

	restore();
}
