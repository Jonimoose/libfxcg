// If the number of args is odd then the last arg is the default result.

#include "stdafx.h"
#include "defs.h"

void
eval_test(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		if (cdr(p1) == symbol(NIL)) {
			push(car(p1)); // default case
			eval();
			return;
		}
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (!iszero(p2)) {
			push(cadr(p1));
			eval();
			return;
		}
		p1 = cddr(p1);
	}
	push_integer(0);
}

// The test for equality is weaker than the other relational operators.

// For example, A<=B causes a stop when the result of A minus B is not a
// numerical value.

// However, A==B never causes a stop.

// For A==B, any nonzero result for A minus B indicates inequality.

void
eval_testeq(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p1 = pop();
	if (iszero(p1))
		push_integer(1);
	else
		push_integer(0);
}

// Relational operators expect a numeric result for operand difference.

void
eval_testge(void)
{
	if (cmp_args() >= 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testgt(void)
{
	if (cmp_args() > 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testle(void)
{
	if (cmp_args() <= 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testlt(void)
{
	if (cmp_args() < 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_not(void)
{
	push(cadr(p1));
	eval_predicate();
	p1 = pop();
	if (iszero(p1))
		push_integer(1);
	else
		push_integer(0);
}

void
eval_and(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (iszero(p2)) {
			push_integer(0);
			return;
		}
		p1 = cdr(p1);
	}
	push_integer(1);
}

void
eval_or(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (!iszero(p2)) {
			push_integer(1);
			return;
		}
		p1 = cdr(p1);
	}
	push_integer(0);
}

// use subtract for cases like A < A + 1

int
cmp_args(void)
{
	int t;

	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p1 = pop();

	// try floating point if necessary

	if (p1->k != NUM && p1->k != DOUBLE) {
		push(p1);
		yyfloat();
		eval();
		p1 = pop();
	}

	if (iszero(p1))
		return 0;

	switch (p1->k) {
	case NUM:
		if (MSIGN(p1->u.q.a) == -1)
			t = -1;
		else
			t = 1;
		break;
	case DOUBLE:
		if (p1->u.d < 0.0)
			t = -1;
		else
			t = 1;
		break;
	default:
		stop("relational operator: cannot determine due to non-numerical comparison");
		t = 0;
	}

	return t;
}

#if SELFTEST

static char *s[] = {

	"a<a+1",
	"1",

	"a-1<a",
	"1",

	"1==1",
	"1",

	"1==2",
	"0",

	"1>=1",
	"1",

	"1>=2",
	"0",

	"2>=1",
	"1",

	"1>1",
	"0",

	"1>2",
	"0",

	"2>1",
	"1",

	"1<=1",
	"1",

	"1<=2",
	"1",

	"2<=1",
	"0",

	"1<1",
	"0",

	"1<2",
	"1",

	"2<1",
	"0",

	"test(0,A,B)",
	"B",

	"test(1,A,B)",
	"A",

	"test(0,A,0,B)",
	"0",

	"test(0,A,0,B,C)",
	"C",

	"not(1)",
	"0",

	"not(0)",
	"1",

	"not(a=a)",
	"0",

	"and(1,1)",
	"1",

	"and(1,0)",
	"0",

	"or(1,0)",
	"1",

	"or(0,0)",
	"0",

	"(0,0)==0",
	"1",

	"1<sqrt(3)",
	"1",
};

void
test_test(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
