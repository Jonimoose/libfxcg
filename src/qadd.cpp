//	Add rational numbers
//
//	Input:		tos-2		addend
//
//			tos-1		addend
//
//	Output:		sum on stack

#include "stdafx.h"
#include "defs.h"

void
qadd(void)
{
	unsigned int *a, *ab, *b, *ba, *c;

	save();

	p2 = pop();
	p1 = pop();

	ab = mmul(p1->u.q.a, p2->u.q.b);
	ba = mmul(p1->u.q.b, p2->u.q.a);

	a = madd(ab, ba);

	mfree(ab);
	mfree(ba);

	// zero?

	if (MZERO(a)) {
		mfree(a);
		push(zero);
		restore();
		return;
	}

	b = mmul(p1->u.q.b, p2->u.q.b);

	c = mgcd(a, b);

	MSIGN(c) = MSIGN(b);

	p1 = alloc();

	p1->k = NUM;

	p1->u.q.a = mdiv(a, c);
	p1->u.q.b = mdiv(b, c);

	mfree(a);
	mfree(b);
	mfree(c);

	push(p1);

	restore();
}
