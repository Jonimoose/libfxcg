// Cofactor of a matrix component.

#include "stdafx.h"
#include "defs.h"

void
eval_cofactor(void)
{
	int i, j, n;
	push(cadr(p1));
	eval();
	p2 = pop();
	if (istensor(p2) && p2->u.tensor->ndim == 2 && p2->u.tensor->dim[0] == p2->u.tensor->dim[1])
		;
	else
		stop("cofactor: 1st arg: square matrix expected");
	n = p2->u.tensor->dim[0];
	push(caddr(p1));
	eval();
	i = pop_integer();
	if (i < 1 || i > n)
		stop("cofactor: 2nd arg: row index expected");
	push(cadddr(p1));
	eval();
	j = pop_integer();
	if (j < 1 || j > n)
		stop("cofactor: 3rd arg: column index expected");
	cofactor(p2, n, i - 1, j - 1);
}

void
cofactor(U *p, int n, int row, int col)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (i != row && j != col)
				push(p->u.tensor->elem[n * i + j]);
	determinant(n - 1);
	if ((row + col) % 2)
		negate();
}

#if SELFTEST

static char *s[] = {

	"cofactor(((1,2),(3,4)),1,1)",
	"4",

	"cofactor(((1,2),(3,4)),1,2)",
	"-3",

	"cofactor(((1,2),(3,4)),2,1)",
	"-2",

	"cofactor(((1,2),(3,4)),2,2)",
	"1",

	"cofactor(((1,2,3),(4,5,6),(7,8,9)),1,2)",
	"6",
};

void
test_cofactor(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
