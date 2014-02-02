/*	Substitute new expr for old expr in expr.

	Input:	push	expr

		push	old expr

		push	new expr

	Output:	Result on stack
*/

#include "stdafx.h"
#include "defs.h"

void
subst(void)
{
	int i;
	save();
	p3 = pop(); // new expr
	p2 = pop(); // old expr
	if (p2 == symbol(NIL) || p3 == symbol(NIL)) {
		restore();
		return;
	}
	p1 = pop(); // expr
	if (istensor(p1)) {
		p4 = alloc_tensor(p1->u.tensor->nelem);
		p4->u.tensor->ndim = p1->u.tensor->ndim;
		for (i = 0; i < p1->u.tensor->ndim; i++)
			p4->u.tensor->dim[i] = p1->u.tensor->dim[i];
		for (i = 0; i < p1->u.tensor->nelem; i++) {
			push(p1->u.tensor->elem[i]);
			push(p2);
			push(p3);
			subst();
			p4->u.tensor->elem[i] = pop();
		}
		push(p4);
	} else if (equal(p1, p2))
		push(p3);
	else if (iscons(p1)) {
		push(car(p1));
		push(p2);
		push(p3);
		subst();
		push(cdr(p1));
		push(p2);
		push(p3);
		subst();
		cons();
	} else
		push(p1);
	restore();
}
