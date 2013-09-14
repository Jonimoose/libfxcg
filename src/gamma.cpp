//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	Gamma function gamma(x)
//	
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
void gamma(void);
static void gammaf(void);
static void gamma_of_sum(void);

void
eval_gamma(void)
{
	push(cadr(p1));
	eval();
	gamma();
}

void
gamma(void)
{
	save();
	gammaf();
	restore();
}

static void
gammaf(void)
{
//	double d;

	p1 = pop();

	if (isrational(p1) && MEQUAL(p1->u.q.a, 1) && MEQUAL(p1->u.q.b, 2))  {
		push_symbol(PI);;
		push_rational(1,2);
		power();
		return;
	}

	if (isrational(p1) && MEQUAL(p1->u.q.a, 3) && MEQUAL(p1->u.q.b, 2))  {
		push_symbol(PI);;
		push_rational(1,2);
		power();
		push_rational(1,2);
		multiply();
		return;
	}
	
//	if (p1->k == DOUBLE) {
//		d = exp(lgamma(p1->u.d));
//		push_double(d);
//		return;
//	}

	if (isnegativeterm(p1)) {
		push_symbol(PI);
		push_integer(-1);
		multiply();
		push_symbol(PI);
		push(p1);
		multiply();
		sine();
		push(p1);
		multiply();
		push(p1);
		negate();
		gamma();
		multiply();
		divide();
		return;
	}
	
	if (car(p1) == symbol(ADD)) {
		gamma_of_sum();
		return;
	}
	
		
	push_symbol(GAMMA);
	push(p1);
	list(2);
	return;
}

static void
gamma_of_sum(void)
{
	p3 = cdr(p1);
	if (isrational(car(p3)) && MEQUAL(car(p3)->u.q.a, 1) && MEQUAL(car(p3)->u.q.b, 1)) {
		push(cadr(p3));
		push(cadr(p3));
		gamma();
		multiply();
	}
	else {
		if (isrational(car(p3)) && MEQUAL(car(p3)->u.q.a, -1) && MEQUAL(car(p3)->u.q.b, 1)) {
			push(cadr(p3));
			gamma();
			push(cadr(p3));
			push_integer(-1);
			add();
			divide();
		}
		else	{
			push_symbol(GAMMA);
			push(p1);
			list(2);
			return;
		}
	}
}

#if SELFTEST

static char *s[] = {

	"Gamma(a)",
	"Gamma(a)",

//	"float(gamma(10))",
//	"362880",

	"Gamma(x+1)",
	"x*Gamma(x)",
	
	"Gamma(1/2)",
	"pi^(1/2)",
	
	"Gamma(x-1)-Gamma(x)/(-1+x)",
	"0",

	"Gamma(-x)",
	"-pi/(x*Gamma(x)*sin(pi*x))",
	
};

void
test_gamma(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
