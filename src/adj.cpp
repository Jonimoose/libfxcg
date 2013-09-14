// Adjunct of a matrix

#include "stdafx.h"
#include "defs.h"

void
eval_adj(void)
{
	push(cadr(p1));
	eval();
	adj();
}

void
adj(void)
{
	int i, j, n;

	save();

	p1 = pop();

	if (istensor(p1) && p1->u.tensor->ndim == 2 && p1->u.tensor->dim[0] == p1->u.tensor->dim[1])
		;
	else
		stop("adj: square matrix expected");

	n = p1->u.tensor->dim[0];

	p2 = alloc_tensor(n * n);

	p2->u.tensor->ndim = 2;
	p2->u.tensor->dim[0] = n;
	p2->u.tensor->dim[1] = n;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			cofactor(p1, n, i, j);
			p2->u.tensor->elem[n * j + i] = pop(); /* transpose */
		}

	push(p2);

	restore();
}

#if SELFTEST

static char *s[] = {

	"adj(((a,b),(c,d)))",
	"((d,-b),(-c,a))",

	"adj(((1,2),(3,4)))",
	"((4,-2),(-3,1))",

	"adj(((2,3,-2,5),(6,-2,1,4),(5,10,3,-2),(-1,2,2,3)))",
	"((-4,-177,-73,194),(-117,117,-99,-27),(310,-129,-44,-374),(-130,-51,71,-211))",
};

void
test_adj(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
