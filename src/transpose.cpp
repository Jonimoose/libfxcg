// Transpose tensor indices

#include "stdafx.h"
#include "defs.h"

void
eval_transpose(void)
{
	push(cadr(p1));
	eval();
	if (cddr(p1) == symbol(NIL)) {
		push_integer(1);
		push_integer(2);
	} else {
		push(caddr(p1));
		eval();
		push(cadddr(p1));
		eval();
	}
	transpose();
}

void
transpose(void)
{
	int i, j, k, l, m, ndim, nelem, t;
	int ai[MAXDIM], an[MAXDIM];
	U **a, **b;

	save();

	p3 = pop();
	p2 = pop();
	p1 = pop();

	if (!istensor(p1)) {
		if (!iszero(p1))
			stop("transpose: tensor expected, 1st arg is not a tensor");
		push(zero);
		restore();
		return;
	}

	ndim = p1->u.tensor->ndim;
	nelem = p1->u.tensor->nelem;

	// vector?

	if (ndim == 1) {
		push(p1);
		restore();
		return;
	}

	push(p2);
	l = pop_integer();

	push(p3);
	m = pop_integer();

	if (l < 1 || l > ndim || m < 1 || m > ndim)
		stop("transpose: index out of range");

	l--;
	m--;

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];

	p2->u.tensor->dim[l] = p1->u.tensor->dim[m];
	p2->u.tensor->dim[m] = p1->u.tensor->dim[l];

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	// init tensor index

	for (i = 0; i < ndim; i++) {
		ai[i] = 0;
		an[i] = p1->u.tensor->dim[i];
	}

	// copy components from a to b

	for (i = 0; i < nelem; i++) {

		// swap indices l and m

		t = ai[l]; ai[l] = ai[m]; ai[m] = t;
		t = an[l]; an[l] = an[m]; an[m] = t;

		// convert tensor index to linear index k

		k = 0;
		for (j = 0; j < ndim; j++)
			k = (k * an[j]) + ai[j];

		// swap indices back

		t = ai[l]; ai[l] = ai[m]; ai[m] = t;
		t = an[l]; an[l] = an[m]; an[m] = t;

		// copy one element

		b[k] = a[i];

		// increment tensor index

		// Suppose the tensor dimensions are 2 and 3.
		// Then the tensor index ai increments as follows:
		// 00 -> 01
		// 01 -> 02
		// 02 -> 10
		// 10 -> 11
		// 11 -> 12
		// 12 -> 00

		for (j = ndim - 1; j >= 0; j--) {
			if (++ai[j] < an[j])
				break;
			ai[j] = 0;
		}
	}

	push(p2);
	restore();
}

#if SELFTEST

static char *s[] = {

	"transpose(0)",
	"0",

	"transpose(0.0)",
	"0",

	"transpose(((a,b),(c,d)))",
	"((a,c),(b,d))",

	"transpose(((a,b),(c,d)),1,2)",
	"((a,c),(b,d))",

	"transpose(((a,b,c),(d,e,f)),1,2)",
	"((a,d),(b,e),(c,f))",

	"transpose(((a,d),(b,e),(c,f)),1,2)",
	"((a,b,c),(d,e,f))",

	"transpose((a,b,c))",
	"(a,b,c)",
};

void
test_transpose(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
