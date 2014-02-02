#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Encapsulate stack values in a vector
//
//	Input:		n		Number of values on stack
//
//			tos-n		Start of value
//
//	Output:		Vector on stack
//
//-----------------------------------------------------------------------------

#include "defs.h"

void
vectorize(int n)
{
	int i;

	save();

	p1 = alloc_tensor(n);

	p1->u.tensor->ndim = 1;
	p1->u.tensor->dim[0] = n;

	for (i = 0; i < n; i++)
		p1->u.tensor->elem[i] = stack[tos - n + i];

	tos -= n;

	push(p1);

	restore();
}
