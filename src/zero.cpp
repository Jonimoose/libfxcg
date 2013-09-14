#include "stdafx.h"
#include "defs.h"

void
eval_zero(void)
{
	int i, k[MAXDIM], m, n;
	m = 1;
	n = 0;
	p2 = cdr(p1);
	while (iscons(p2)) {
		push(car(p2));
		eval();
		i = pop_integer();
		if (i < 2) {
			push(zero);
			return;
		}
		m *= i;
		k[n++] = i;
		p2 = cdr(p2);
	}
	if (n == 0) {
		push(zero);
		return;
	}
	p1 = alloc_tensor(m);
	p1->u.tensor->ndim = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->dim[i] = k[i];
	push(p1);
}

#if SELFTEST

static char *s[] = {

	"zero(2,2)",
	"((0,0),(0,0))",

	"zero(1,1)",
	"0",
};

void
test_zero(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
