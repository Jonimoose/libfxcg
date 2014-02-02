// Contract across tensor indices

#include "stdafx.h"
#include "defs.h"

void
eval_contract(void)
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
	contract();
}

void
contract(void)
{
	save();
	yycontract();
	restore();
}

void
yycontract(void)
{
	int h, i, j, k, l, m, n, ndim, nelem;
	int ai[MAXDIM], an[MAXDIM];
	U **a, **b;

	p3 = pop();
	p2 = pop();
	p1 = pop();

	if (!istensor(p1)) {
		if (!iszero(p1))
			stop("contract: tensor expected, 1st arg is not a tensor");
		push(zero);
		return;
	}

	push(p2);
	l = pop_integer();

	push(p3);
	m = pop_integer();

	ndim = p1->u.tensor->ndim;

	if (l < 1 || l > ndim || m < 1 || m > ndim || l == m
	|| p1->u.tensor->dim[l - 1] != p1->u.tensor->dim[m - 1])
		stop("contract: index out of range");

	l--;
	m--;

	n = p1->u.tensor->dim[l];

	// nelem is the number of elements in "b"

	nelem = 1;
	for (i = 0; i < ndim; i++)
		if (i != l && i != m)
			nelem *= p1->u.tensor->dim[i];

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim - 2;

	j = 0;
	for (i = 0; i < ndim; i++)
		if (i != l && i != m)
			p2->u.tensor->dim[j++] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	for (i = 0; i < ndim; i++) {
		ai[i] = 0;
		an[i] = p1->u.tensor->dim[i];
	}

	for (i = 0; i < nelem; i++) {
		push(zero);
		for (j = 0; j < n; j++) {
			ai[l] = j;
			ai[m] = j;
			h = 0;
			for (k = 0; k < ndim; k++)
				h = (h * an[k]) + ai[k];
			push(a[h]);
			add();
		}
		b[i] = pop();
		for (j = ndim - 1; j >= 0; j--) {
			if (j == l || j == m)
				continue;
			if (++ai[j] < an[j])
				break;
			ai[j] = 0;
		}
	}

	if (nelem == 1)
		push(b[0]);
	else
		push(p2);
}

#if SELFTEST

static char *s[] = {

	"contract(0)",
	"0",

	"contract(0.0)",
	"0",

	"contract(((a,b),(c,d)))",
	"a+d",

	"contract(((1,2),(3,4)),1,2)",
	"5",

	"A=((a11,a12),(a21,a22))",
	"",

	"B=((b11,b12),(b21,b22))",
	"",

	"contract(outer(A,B),2,3)",
	"((a11*b11+a12*b21,a11*b12+a12*b22),(a21*b11+a22*b21,a21*b12+a22*b22))",

	"A=quote(A)",
	"",

	"B=quote(B)",
	"",
};

void
test_contract(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
