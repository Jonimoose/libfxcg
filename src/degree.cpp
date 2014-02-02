#include "stdafx.h"
#include "defs.h"

void
eval_degree(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p1 = pop();
	if (p1 == symbol(NIL))
		guess();
	else
		push(p1);
	degree();
}

//-----------------------------------------------------------------------------
//
//	Find the degree of a polynomial
//
//	Input:		tos-2		p(x)
//
//			tos-1		x
//
//	Output:		Result on stack
//
//	Note: Finds the largest numerical power of x. Does not check for
//	weirdness in p(x).
//
//-----------------------------------------------------------------------------

#define POLY p1
#define X p2
#define DEGREE p3

void
degree(void)
{
	save();
	X = pop();
	POLY = pop();
	DEGREE = zero;
	yydegree(POLY);
	push(DEGREE);
	restore();
}

void
yydegree(U *p)
{
	if (equal(p, X)) {
		if (iszero(DEGREE))
			DEGREE = one;
	} else if (car(p) == symbol(POWER)) {
		if (equal(cadr(p), X) && isnum(caddr(p)) && lessp(DEGREE, caddr(p)))
			DEGREE = caddr(p);
	 } else if (iscons(p)) {
		p = cdr(p);
		while (iscons(p)) {
			yydegree(car(p));
			p = cdr(p);
		}
	}
}
