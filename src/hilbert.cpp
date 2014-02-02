#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Create a Hilbert matrix
//
//	Input:		Dimension on stack
//
//	Output:		Hilbert matrix on stack
//
//	Example:
//
//	> hilbert(5)
//	((1,1/2,1/3,1/4),(1/2,1/3,1/4,1/5),(1/3,1/4,1/5,1/6),(1/4,1/5,1/6,1/7))
//
//-----------------------------------------------------------------------------

#include "defs.h"

#define A p1
#define N p2

#define AELEM(i, j) A->u.tensor->elem[i * n + j]

void
hilbert(void)
{
	int i, j, n;
	save();
	N = pop();
	push(N);
	n = pop_integer();
	if (n < 2) {
		push_symbol(HILBERT);
		push(N);
		list(2);
		restore();
		return;
	}
	push_zero_matrix(n, n);
	A = pop();
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			push_integer(i + j + 1);
			inverse();
			AELEM(i, j) = pop();
		}
	}
	push(A);
	restore();
}
