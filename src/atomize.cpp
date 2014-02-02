#include "stdafx.h"
#include "defs.h"

void
eval_atomize(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (iscons(p1))
		atomize();
	else
		push(p1);
}

void
atomize(void)
{
	int i, n;
	p1 = cdr(p1);
	n = length(p1);
	if (n == 1) {
		push(car(p1));
		return;
	}
	p2 = alloc_tensor(n);
	p2->u.tensor->ndim = 1;
	p2->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++) {
		p2->u.tensor->elem[i] = car(p1);
		p1 = cdr(p1);
	}
	push(p2);
}
