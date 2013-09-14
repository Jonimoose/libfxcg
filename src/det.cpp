//-----------------------------------------------------------------------------
//
//	Input:		Matrix on stack
//
//	Output:		Determinant on stack
//
//	Example:
//
//	> det(((1,2),(3,4)))
//	-2
//
//	Note:
//
//	Uses Gaussian elimination for numerical matrices.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

static int
check_arg(void)
{
	if (!istensor(p1))
		return 0;
	else if (p1->u.tensor->ndim != 2)
		return 0;
	else if (p1->u.tensor->dim[0] != p1->u.tensor->dim[1])
		return 0;
	else
		return 1;
}

void
det(void)
{
	int i, n;
	U **a;

	save();

	p1 = pop();

	if (check_arg() == 0) {
		push_symbol(DET);
		push(p1);
		list(2);
		restore();
		return;
	}

	n = p1->u.tensor->nelem;

	a = p1->u.tensor->elem;

	for (i = 0; i < n; i++)
		if (!isnum(a[i]))
			break;

	if (i == n)
		yydetg();
	else {
		for (i = 0; i < p1->u.tensor->nelem; i++)
			push(p1->u.tensor->elem[i]);
		determinant(p1->u.tensor->dim[0]);
	}

	restore();
}

// determinant of n * n matrix elements on the stack

void
determinant(int n)
{
	int h, i, j, k, q, s, sign, t;
	int *a, *c, *d;

	h = tos - n * n;

	a = (int *) malloc(3 * n * sizeof (int));

	if (a == NULL)
		out_of_memory();

	c = a + n;

	d = c + n;

	for (i = 0; i < n; i++) {
		a[i] = i;
		c[i] = 0;
		d[i] = 1;
	}

	sign = 1;

	push(zero);

	for (;;) {

		if (sign == 1)
			push_integer(1);
		else
			push_integer(-1);

		for (i = 0; i < n; i++) {
			k = n * a[i] + i;
			push(stack[h + k]);
			multiply(); // FIXME -- problem here
		}

		add();

		/* next permutation (Knuth's algorithm P) */

		j = n - 1;
		s = 0;
	P4:	q = c[j] + d[j];
		if (q < 0) {
			d[j] = -d[j];
			j--;
			goto P4;
		}
		if (q == j + 1) {
			if (j == 0)
				break;
			s++;
			d[j] = -d[j];
			j--;
			goto P4;
		}

		t = a[j - c[j] + s];
		a[j - c[j] + s] = a[j - q + s];
		a[j - q + s] = t;
		c[j] = q;

		sign = -sign;
	}

	free(a);

	stack[h] = stack[tos - 1];

	tos = h + 1;
}

//-----------------------------------------------------------------------------
//
//	Input:		Matrix on stack
//
//	Output:		Determinant on stack
//
//	Note:
//
//	Uses Gaussian elimination which is faster for numerical matrices.
//
//	Gaussian Elimination works by walking down the diagonal and clearing
//	out the columns below it.
//
//-----------------------------------------------------------------------------

void
detg(void)
{
	save();

	p1 = pop();

	if (check_arg() == 0) {
		push_symbol(DET);
		push(p1);
		list(2);
		restore();
		return;
	}

	yydetg();

	restore();
}

void
yydetg(void)
{
	int i, n;

	n = p1->u.tensor->dim[0];

	for (i = 0; i < n * n; i++)
		push(p1->u.tensor->elem[i]);

	lu_decomp(n);

	tos -= n * n;

	push(p1);
}

//-----------------------------------------------------------------------------
//
//	Input:		n * n matrix elements on stack
//
//	Output:		p1	determinant
//
//			p2	mangled
//
//			upper diagonal matrix on stack
//
//-----------------------------------------------------------------------------

#define M(i, j) stack[h + n * (i) + (j)]

void
lu_decomp(int n)
{
	int d, h, i, j;

	h = tos - n * n;

	p1 = one;

	for (d = 0; d < n - 1; d++) {

		// diagonal element zero?

		if (equal(M(d, d), zero)) {

			// find a new row

			for (i = d + 1; i < n; i++)
				if (!equal(M(i, d), zero))
					break;

			if (i == n) {
				p1 = zero;
				break;
			}

			// exchange rows

			for (j = d; j < n; j++) {
				p2 = M(d, j);
				M(d, j) = M(i, j);
				M(i, j) = p2;
			}

			// negate det

			push(p1);
			negate();
			p1 = pop();
		}

		// update det

		push(p1);
		push(M(d, d));
		multiply();
		p1 = pop();

		// update lower diagonal matrix

		for (i = d + 1; i < n; i++) {

			// multiplier

			push(M(i, d));
			push(M(d, d));
			divide();
			negate();

			p2 = pop();

			// update one row

			M(i, d) = zero; // clear column below pivot d

			for (j = d + 1; j < n; j++) {
				push(M(d, j));
				push(p2);
				multiply();
				push(M(i, j));
				add();
				M(i, j) = pop();
			}
		}
	}

	// last diagonal element

	push(p1);
	push(M(n - 1, n - 1));
	multiply();
	p1 = pop();
}
