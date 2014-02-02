//-----------------------------------------------------------------------------
//
//	Input:		Matrix on stack
//
//	Output:		Inverse on stack
//
//	Example:
//
//	> inv(((1,2),(3,4))
//	((-2,1),(3/2,-1/2))
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
inv(void)
{
	int i, n;
	U **a;

	save();

	p1 = pop();

	if (check_arg() == 0) {
		push_symbol(INV);
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
		yyinvg();
	else {
		push(p1);
		adj();
		push(p1);
		det();
		p2 = pop();
		if (iszero(p2))
			stop("inverse of singular matrix");
		push(p2);
		divide();
	}

	restore();
}

void
invg(void)
{
	save();

	p1 = pop();

	if (check_arg() == 0) {
		push_symbol(INVG);
		push(p1);
		list(2);
		restore();
		return;
	}

	yyinvg();

	restore();
}

// inverse using gaussian elimination

void
yyinvg(void)
{
	int h, i, j, n;

	n = p1->u.tensor->dim[0];

	h = tos;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (i == j)
				push(one);
			else
				push(zero);

	for (i = 0; i < n * n; i++)
		push(p1->u.tensor->elem[i]);

	decomp(n);

	p1 = alloc_tensor(n * n);

	p1->u.tensor->ndim = 2;
	p1->u.tensor->dim[0] = n;
	p1->u.tensor->dim[1] = n;

	for (i = 0; i < n * n; i++)
		p1->u.tensor->elem[i] = stack[h + i];

	tos -= 2 * n * n;

	push(p1);
}

//-----------------------------------------------------------------------------
//
//	Input:		n * n unit matrix on stack
//
//			n * n operand on stack
//
//	Output:		n * n inverse matrix on stack
//
//			n * n garbage on stack
//
//			p2 mangled
//
//-----------------------------------------------------------------------------

#define A(i, j) stack[a + n * (i) + (j)]
#define U(i, j) stack[u + n * (i) + (j)]

void
decomp(int n)
{
	int a, d, i, j, u;

	a = tos - n * n;

	u = a - n * n;

	for (d = 0; d < n; d++) {

		// diagonal element zero?

		if (equal(A(d, d), zero)) {

			// find a new row

			for (i = d + 1; i < n; i++)
				if (!equal(A(i, d), zero))
					break;

			if (i == n)
				stop("inverse of singular matrix");

			// exchange rows

			for (j = 0; j < n; j++) {

				p2 = A(d, j);
				A(d, j) = A(i, j);
				A(i, j) = p2;

				p2 = U(d, j);
				U(d, j) = U(i, j);
				U(i, j) = p2;
			}
		}

		// multiply the pivot row by 1 / pivot

		p2 = A(d, d);

		for (j = 0; j < n; j++) {

			if (j > d) {
				push(A(d, j));
				push(p2);
				divide();
				A(d, j) = pop();
			}

			push(U(d, j));
			push(p2);
			divide();
			U(d, j) = pop();
		}

		// clear out the column above and below the pivot

		for (i = 0; i < n; i++) {

			if (i == d)
				continue;

			// multiplier

			p2 = A(i, d);

			// add pivot row to i-th row

			for (j = 0; j < n; j++) {

				if (j > d) {
					push(A(i, j));
					push(A(d, j));
					push(p2);
					multiply();
					subtract();
					A(i, j) = pop();
				}

				push(U(i, j));
				push(U(d, j));
				push(p2);
				multiply();
				subtract();
				U(i, j) = pop();
			}
		}
	}
}
