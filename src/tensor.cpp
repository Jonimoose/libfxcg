#include "stdafx.h"

#include "defs.h"

static void promote_tensor(void);
static int compatible(U *, U *);

//-----------------------------------------------------------------------------
//
//	Called from the "eval" module to evaluate tensor elements.
//
//	p1 points to the tensor operand.
//
//-----------------------------------------------------------------------------

void
eval_tensor(void)
{
	int i, ndim, nelem;
	U **a, **b;

	//---------------------------------------------------------------------
	//
	//	create a new tensor for the result
	//
	//---------------------------------------------------------------------

	nelem = p1->u.tensor->nelem;

	ndim = p1->u.tensor->ndim;

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];

	//---------------------------------------------------------------------
	//
	//	b = eval(a)
	//
	//---------------------------------------------------------------------

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(a[i]);
		eval();
		b[i] = pop();
	}

	//---------------------------------------------------------------------
	//
	//	push the result
	//
	//---------------------------------------------------------------------

	push(p2);

	promote_tensor();
}

//-----------------------------------------------------------------------------
//
//	Add tensors
//
//	Input:		Operands on stack
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

void
tensor_plus_tensor(void)
{
	int i, ndim, nelem;
	U **a, **b, **c;

	save();

	p2 = pop();
	p1 = pop();

	// are the dimension lists equal?

	ndim = p1->u.tensor->ndim;

	if (ndim != p2->u.tensor->ndim) {
		push(symbol(NIL));
		restore();
		return;
	}

	for (i = 0; i < ndim; i++)
		if (p1->u.tensor->dim[i] != p2->u.tensor->dim[i]) {
			push(symbol(NIL));
			restore();
			return;
		}

	// create a new tensor for the result

	nelem = p1->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	// c = a + b

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;
	c = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(a[i]);
		push(b[i]);
		add();
		c[i] = pop();
	}

	// push the result

	push(p3);

	restore();
}

//-----------------------------------------------------------------------------
//
//	careful not to reorder factors
//
//-----------------------------------------------------------------------------

void
tensor_times_scalar(void)
{
	int i, ndim, nelem;
	U **a, **b;

	save();

	p2 = pop();
	p1 = pop();

	ndim = p1->u.tensor->ndim;
	nelem = p1->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(a[i]);
		push(p2);
		multiply();
		b[i] = pop();
	}

	push(p3);
	restore();
}

void
scalar_times_tensor(void)
{
	int i, ndim, nelem;
	U **a, **b;

	save();

	p2 = pop();
	p1 = pop();

	ndim = p2->u.tensor->ndim;
	nelem = p2->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p2->u.tensor->dim[i];

	a = p2->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(p1);
		push(a[i]);
		multiply();
		b[i] = pop();
	}

	push(p3);

	restore();
}

int
is_square_matrix(U *p)
{
	if (istensor(p) && p->u.tensor->ndim == 2 && p->u.tensor->dim[0] == p->u.tensor->dim[1])
		return 1;
	else
		return 0;
}

//-----------------------------------------------------------------------------
//
//	gradient of tensor
//
//-----------------------------------------------------------------------------

void
d_tensor_tensor(void)
{
	int i, j, ndim, nelem;
	U **a, **b, **c;

	ndim = p1->u.tensor->ndim;
	nelem = p1->u.tensor->nelem;

	if (ndim + 1 >= MAXDIM)
		goto dont_evaluate;

	p3 = alloc_tensor(nelem * p2->u.tensor->nelem);

	p3->u.tensor->ndim = ndim + 1;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	p3->u.tensor->dim[ndim] = p2->u.tensor->dim[0];

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;
	c = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		for (j = 0; j < p2->u.tensor->nelem; j++) {
			push(a[i]);
			push(b[j]);
			derivative();
			c[i * p2->u.tensor->nelem + j] = pop();
		}
	}

	push(p3);

	return;

dont_evaluate:

	push_symbol(DERIVATIVE);
	push(p1);
	push(p2);
	list(3);
}

//-----------------------------------------------------------------------------
//
//	gradient of scalar
//
//-----------------------------------------------------------------------------

void
d_scalar_tensor(void)
{
	int i;
	U **a, **b;

	p3 = alloc_tensor(p2->u.tensor->nelem);

	p3->u.tensor->ndim = 1;

	p3->u.tensor->dim[0] = p2->u.tensor->dim[0];

	a = p2->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < p2->u.tensor->nelem; i++) {
		push(p1);
		push(a[i]);
		derivative();
		b[i] = pop();
	}

	push(p3);
}

//-----------------------------------------------------------------------------
//
//	Derivative of tensor
//
//-----------------------------------------------------------------------------

void
d_tensor_scalar(void)
{
	int i;
	U **a, **b;

	p3 = alloc_tensor(p1->u.tensor->nelem);

	p3->u.tensor->ndim = p1->u.tensor->ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(a[i]);
		push(p2);
		derivative();
		b[i] = pop();
	}

	push(p3);
}

int
compare_tensors(U *p1, U *p2)
{
	int i;

	if (p1->u.tensor->ndim < p2->u.tensor->ndim)
		return -1;

	if (p1->u.tensor->ndim > p2->u.tensor->ndim)
		return 1;

	for (i = 0; i < p1->u.tensor->ndim; i++) {
		if (p1->u.tensor->dim[i] < p2->u.tensor->dim[i])
			return -1;
		if (p1->u.tensor->dim[i] > p2->u.tensor->dim[i])
			return 1;
	}

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		if (equal(p1->u.tensor->elem[i], p2->u.tensor->elem[i]))
			continue;
		if (lessp(p1->u.tensor->elem[i], p2->u.tensor->elem[i]))
			return -1;
		else
			return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
//
//	Raise a tensor to a power
//
//	Input:		p1	tensor
//
//			p2	exponent
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

void
power_tensor(void)
{
	int i, k, n;

	// first and last dims must be equal

	k = p1->u.tensor->ndim - 1;

	if (p1->u.tensor->dim[0] != p1->u.tensor->dim[k]) {
		push_symbol(POWER);
		push(p1);
		push(p2);
		list(3);
		return;
	}

	push(p2);

	n = pop_integer();

	if (n == (int) 0x80000000) {
		push_symbol(POWER);
		push(p1);
		push(p2);
		list(3);
		return;
	}

	if (n == 0) {
		if (p1->u.tensor->ndim != 2)
			stop("power(tensor,0) with tensor rank not equal to 2");
		n = p1->u.tensor->dim[0];
		p1 = alloc_tensor(n * n);
		p1->u.tensor->ndim = 2;
		p1->u.tensor->dim[0] = n;
		p1->u.tensor->dim[1] = n;
		for (i = 0; i < n; i++)
			p1->u.tensor->elem[n * i + i] = one;
		push(p1);
		return;
	}

	if (n < 0) {
		n = -n;
		push(p1);
		inv();
		p1 = pop();
	}

	push(p1);

	for (i = 1; i < n; i++) {
		push(p1);
		inner();
		if (iszero(stack[tos - 1]))
			break;
	}
}

void
copy_tensor(void)
{
	int i;

	save();

	p1 = pop();

	p2 = alloc_tensor(p1->u.tensor->nelem);

	p2->u.tensor->ndim = p1->u.tensor->ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];

	for (i = 0; i < p1->u.tensor->nelem; i++)
		p2->u.tensor->elem[i] = p1->u.tensor->elem[i];

	push(p2);

	restore();
}

// Tensors with elements that are also tensors get promoted to a higher rank.

static void
promote_tensor(void)
{
	int i, j, k, nelem, ndim;

	save();

	p1 = pop();

	if (!istensor(p1)) {
		push(p1);
		restore();
		return;
	}

	p2 = p1->u.tensor->elem[0];

	for (i = 1; i < p1->u.tensor->nelem; i++)
		if (!compatible(p2, p1->u.tensor->elem[i]))
			stop("Cannot promote tensor due to inconsistent tensor components.");

	if (!istensor(p2)) {
		push(p1);
		restore();
		return;
	}

	ndim = p1->u.tensor->ndim + p2->u.tensor->ndim;

	if (ndim > MAXDIM)
		stop("tensor rank > 24");

	nelem = p1->u.tensor->nelem * p2->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	for (j = 0; j < p2->u.tensor->ndim; j++)
		p3->u.tensor->dim[i + j] = p2->u.tensor->dim[j];

	k = 0;

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		p2 = p1->u.tensor->elem[i];
		for (j = 0; j < p2->u.tensor->nelem; j++)
			p3->u.tensor->elem[k++] = p2->u.tensor->elem[j];
	}

	push(p3);

	restore();
}

static int
compatible(U *p, U *q)
{
	int i;

	if (!istensor(p) && !istensor(q))
		return 1;

	if (!istensor(p) || !istensor(q))
		return 0;

	if (p->u.tensor->ndim != q->u.tensor->ndim)
		return 0;

	for (i = 0; i < p->u.tensor->ndim; i++)
		if (p->u.tensor->dim[i] != q->u.tensor->dim[i])
			return 0;

	return 1;
}

#if SELFTEST

static char *s[] = {

	"#test_tensor",

	"a=(1,2,3)",
	"",

	"b=(4,5,6)",
	"",

	"c=(7,8,9)",
	"",

	"rank((a,b,c))",
	"2",

	"(a,b,c)",
	"((1,2,3),(4,5,6),(7,8,9))",

	// check tensor promotion

	"((1,0),(0,0))",
	"((1,0),(0,0))",

	"a=quote(a)",
	"",

	"b=quote(b)",
	"",

	"c=quote(c)",
	"",
};

void
test_tensor(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
