//	Divide rational numbers
//
//	Input:		tos-2		dividend
//
//			tos-1		divisor
//
//	Output:		quotient on stack

#include "stdafx.h"
#include "defs.h"

void
qdiv(void)
{
	unsigned int *aa, *bb, *c;

	save();

	p2 = pop();
	p1 = pop();

	// zero?

	if (MZERO(p2->u.q.a))
		stop("divide by zero");

	if (MZERO(p1->u.q.a)) {
		push(zero);
		restore();
		return;
	}

	aa = mmul(p1->u.q.a, p2->u.q.b);
	bb = mmul(p1->u.q.b, p2->u.q.a);

	c = mgcd(aa, bb);

	MSIGN(c) = MSIGN(bb);

	p1 = alloc();

	p1->k = NUM;

	p1->u.q.a = mdiv(aa, c);
	p1->u.q.b = mdiv(bb, c);

	mfree(aa);
	mfree(bb);
	mfree(c);

	push(p1);

	restore();
}
