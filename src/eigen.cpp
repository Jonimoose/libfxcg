//-----------------------------------------------------------------------------
//
//	Compute eigenvalues and eigenvectors
//
//	Input:		stack[tos - 1]		symmetric matrix
//
//	Output:		D			diagnonal matrix
//
//			Q			eigenvector matrix
//
//	D and Q have the property that
//
//		A == dot(transpose(Q),D,Q)
//
//	where A is the original matrix.
//
//	The eigenvalues are on the diagonal of D.
//
//	The eigenvectors are row vectors in Q.
//
//	The eigenvalue relation
//
//		A X = lambda X
//
//	can be checked as follows:
//
//		lambda = D[1,1]
//
//		X = Q[1]
//
//		dot(A,X) - lambda X
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

#define D(i, j) yydd[n * (i) + (j)]
#define Q(i, j) yyqq[n * (i) + (j)]

extern void copy_tensor(void);
static void eigen(int);
static int check_arg(void);
static int step(void);
static void step2(int, int);
static int n;
static double *yydd, *yyqq;

void
eval_eigen(void)
{
	if (check_arg() == 0)
		stop("eigen: argument is not a square matrix");

	eigen(EIGEN);

	p1 = usr_symbol("D");
	set_binding(p1, p2);

	p1 = usr_symbol("Q");
	set_binding(p1, p3);

	push(symbol(NIL));
}

void
eval_eigenval(void)
{
	if (check_arg() == 0) {
		push_symbol(EIGENVAL);
		push(p1);
		list(2);
		return;
	}

	eigen(EIGENVAL);

	push(p2);
}

void
eval_eigenvec(void)
{
	if (check_arg() == 0) {
		push_symbol(EIGENVEC);
		push(p1);
		list(2);
		return;
	}

	eigen(EIGENVEC);

	push(p3);
}

static int
check_arg(void)
{
	int i, j;

	push(cadr(p1));
	eval();
	yyfloat();
	eval();
	p1 = pop();

	if (!istensor(p1))
		return 0;

	if (p1->u.tensor->ndim != 2 || p1->u.tensor->dim[0] != p1->u.tensor->dim[1])
		stop("eigen: argument is not a square matrix");

	n = p1->u.tensor->dim[0];

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (!isdouble(p1->u.tensor->elem[n * i + j]))
				stop("eigen: matrix is not numerical");

	for (i = 0; i < n - 1; i++)
		for (j = i + 1; j < n; j++)
			if (fabs(p1->u.tensor->elem[n * i + j]->u.d - p1->u.tensor->elem[n * j + i]->u.d) > 1e-10)
				stop("eigen: matrix is not symmetrical");

	return 1;
}

//-----------------------------------------------------------------------------
//
//	Input:		p1		matrix
//
//	Output:		p2		eigenvalues
//
//			p3		eigenvectors
//
//-----------------------------------------------------------------------------

static void
eigen(int op)
{
	int i, j;

	// malloc working vars

	yydd = (double *) malloc(n * n * sizeof (double));

	if (yydd == NULL)
		stop("malloc failure");

	yyqq = (double *) malloc(n * n * sizeof (double));

	if (yyqq == NULL)
		stop("malloc failure");

	// initialize D

	for (i = 0; i < n; i++) {
		D(i, i) = p1->u.tensor->elem[n * i + i]->u.d;
		for (j = i + 1; j < n; j++) {
			D(i, j) = p1->u.tensor->elem[n * i + j]->u.d;
			D(j, i) = p1->u.tensor->elem[n * i + j]->u.d;
		}
	}

	// initialize Q

	for (i = 0; i < n; i++) {
		Q(i, i) = 1.0;
		for (j = i + 1; j < n; j++) {
			Q(i, j) = 0.0;
			Q(j, i) = 0.0;
		}
	}

	// step up to 100 times

	for (i = 0; i < 100; i++)
		if (step() == 0)
			break;

	if (i == 100)
		printstr("\nnote: eigen did not converge\n");

	// p2 = D

	if (op == EIGEN || op == EIGENVAL) {

		push(p1);
		copy_tensor();
		p2 = pop();

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				push_double(D(i, j));
				p2->u.tensor->elem[n * i + j] = pop();
			}
		}
	}

	// p3 = Q

	if (op == EIGEN || op == EIGENVEC) {

		push(p1);
		copy_tensor();
		p3 = pop();

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				push_double(Q(i, j));
				p3->u.tensor->elem[n * i + j] = pop();
			}
		}
	}

	// free working vars

	free(yydd);
	free(yyqq);
}

//-----------------------------------------------------------------------------
//
//	Example: p = 1, q = 3
//
//		c	0	s	0
//
//		0	1	0	0
//	G =
//		-s	0	c	0
//
//		0	0	0	1
//
//	The effect of multiplying G times A is...
//
//	row 1 of A    = c (row 1 of A ) + s (row 3 of A )
//	          n+1                n                 n
//
//	row 3 of A    = c (row 3 of A ) - s (row 1 of A )
//	          n+1                n                 n
//
//	In terms of components the overall effect is...
//
//	row 1 = c row 1 + s row 3
//
//		A[1,1] = c A[1,1] + s A[3,1]
//
//		A[1,2] = c A[1,2] + s A[3,2]
//
//		A[1,3] = c A[1,3] + s A[3,3]
//
//		A[1,4] = c A[1,4] + s A[3,4]
//
//	row 3 = c row 3 - s row 1
//
//		A[3,1] = c A[3,1] - s A[1,1]
//
//		A[3,2] = c A[3,2] - s A[1,2]
//
//		A[3,3] = c A[3,3] - s A[1,3]
//
//		A[3,4] = c A[3,4] - s A[1,4]
//
//	                                   T
//	The effect of multiplying A times G  is...
//
//	col 1 of A    = c (col 1 of A ) + s (col 3 of A )
//	          n+1                n                 n
//
//	col 3 of A    = c (col 3 of A ) - s (col 1 of A )
//	          n+1                n                 n
//
//	In terms of components the overall effect is...
//
//	col 1 = c col 1 + s col 3
//
//		A[1,1] = c A[1,1] + s A[1,3]
//
//		A[2,1] = c A[2,1] + s A[2,3]
//
//		A[3,1] = c A[3,1] + s A[3,3]
//
//		A[4,1] = c A[4,1] + s A[4,3]
//
//	col 3 = c col 3 - s col 1
//
//		A[1,3] = c A[1,3] - s A[1,1]
//
//		A[2,3] = c A[2,3] - s A[2,1]
//
//		A[3,3] = c A[3,3] - s A[3,1]
//
//		A[4,3] = c A[4,3] - s A[4,1]
//
//	What we want to do is just compute the upper triangle of A since we
//	know the lower triangle is identical.
//
//	In other words, we just want to update components A[i,j] where i < j.
//
//-----------------------------------------------------------------------------
//
//	Example: p = 2, q = 5
//
//				p			q
//
//			j=1	j=2	j=3	j=4	j=5	j=6
//
//		i=1	.	A[1,2]	.	.	A[1,5]	.
//
//	p	i=2	A[2,1]	A[2,2]	A[2,3]	A[2,4]	A[2,5]	A[2,6]
//
//		i=3	.	A[3,2]	.	.	A[3,5]	.
//
//		i=4	.	A[4,2]	.	.	A[4,5]	.
//
//	q	i=5	A[5,1]	A[5,2]	A[5,3]	A[5,4]	A[5,5]	A[5,6]
//
//		i=6	.	A[6,2]	.	.	A[6,5]	.
//
//-----------------------------------------------------------------------------
//
//	This is what B = GA does:
//
//	row 2 = c row 2 + s row 5
//
//		B[2,1] = c * A[2,1] + s * A[5,1]
//		B[2,2] = c * A[2,2] + s * A[5,2]
//		B[2,3] = c * A[2,3] + s * A[5,3]
//		B[2,4] = c * A[2,4] + s * A[5,4]
//		B[2,5] = c * A[2,5] + s * A[5,5]
//		B[2,6] = c * A[2,6] + s * A[5,6]
//
//	row 5 = c row 5 - s row 2
//
//		B[5,1] = c * A[5,1] + s * A[2,1]
//		B[5,2] = c * A[5,2] + s * A[2,2]
//		B[5,3] = c * A[5,3] + s * A[2,3]
//		B[5,4] = c * A[5,4] + s * A[2,4]
//		B[5,5] = c * A[5,5] + s * A[2,5]
//		B[5,6] = c * A[5,6] + s * A[2,6]
//
//	               T
//	This is what BG  does:
//
//	col 2 = c col 2 + s col 5
//
//		B[1,2] = c * A[1,2] + s * A[1,5]
//		B[2,2] = c * A[2,2] + s * A[2,5]
//		B[3,2] = c * A[3,2] + s * A[3,5]
//		B[4,2] = c * A[4,2] + s * A[4,5]
//		B[5,2] = c * A[5,2] + s * A[5,5]
//		B[6,2] = c * A[6,2] + s * A[6,5]
//
//	col 5 = c col 5 - s col 2
//
//		B[1,5] = c * A[1,5] - s * A[1,2]
//		B[2,5] = c * A[2,5] - s * A[2,2]
//		B[3,5] = c * A[3,5] - s * A[3,2]
//		B[4,5] = c * A[4,5] - s * A[4,2]
//		B[5,5] = c * A[5,5] - s * A[5,2]
//		B[6,5] = c * A[6,5] - s * A[6,2]
//
//-----------------------------------------------------------------------------
//
//	Step 1: Just do upper triangle (i < j), B[2,5] = 0
//
//		B[1,2] = c * A[1,2] + s * A[1,5]
//
//		B[2,3] = c * A[2,3] + s * A[5,3]
//		B[2,4] = c * A[2,4] + s * A[5,4]
//		B[2,6] = c * A[2,6] + s * A[5,6]
//
//		B[1,5] = c * A[1,5] - s * A[1,2]
//		B[3,5] = c * A[3,5] - s * A[3,2]
//		B[4,5] = c * A[4,5] - s * A[4,2]
//
//		B[5,6] = c * A[5,6] + s * A[2,6]
//
//-----------------------------------------------------------------------------
//
//	Step 2: Transpose where i > j since A[i,j] == A[j,i]
//
//		B[1,2] = c * A[1,2] + s * A[1,5]
//
//		B[2,3] = c * A[2,3] + s * A[3,5]
//		B[2,4] = c * A[2,4] + s * A[4,5]
//		B[2,6] = c * A[2,6] + s * A[5,6]
//
//		B[1,5] = c * A[1,5] - s * A[1,2]
//		B[3,5] = c * A[3,5] - s * A[2,3]
//		B[4,5] = c * A[4,5] - s * A[2,4]
//
//		B[5,6] = c * A[5,6] + s * A[2,6]
//
//-----------------------------------------------------------------------------
//
//	Step 3: Same as above except reorder
//
//	k < p		(k = 1)
//
//		A[1,2] = c * A[1,2] + s * A[1,5]
//		A[1,5] = c * A[1,5] - s * A[1,2]
//
//	p < k < q	(k = 3..4)
//
//		A[2,3] = c * A[2,3] + s * A[3,5]
//		A[3,5] = c * A[3,5] - s * A[2,3]
//
//		A[2,4] = c * A[2,4] + s * A[4,5]
//		A[4,5] = c * A[4,5] - s * A[2,4]
//
//	q < k		(k = 6)
//
//		A[2,6] = c * A[2,6] + s * A[5,6]
//		A[5,6] = c * A[5,6] - s * A[2,6]
//
//-----------------------------------------------------------------------------

static int
step(void)
{
	int count, i, j;

	count = 0;

	// for each upper triangle "off-diagonal" component do step2

	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			if (D(i, j) != 0.0) {
				step2(i, j);
				count++;
			}
		}
	}

	return count;
}

static void
step2(int p, int q)
{
	int k;
	double t, theta;
	double c, cc, s, ss;

	// compute c and s

	// from Numerical Recipes (except they have a_qq - a_pp)

	theta = 0.5 * (D(p, p) - D(q, q)) / D(p, q);

	t = 1.0 / (fabs(theta) + sqrt(theta * theta + 1.0));

	if (theta < 0.0)
		t = -t;

	c = 1.0 / sqrt(t * t + 1.0);

	s = t * c;

	// D = GD

	// which means "add rows"

	for (k = 0; k < n; k++) {
		cc = D(p, k);
		ss = D(q, k);
		D(p, k) = c * cc + s * ss;
		D(q, k) = c * ss - s * cc;
	}

	// D = D transpose(G)

	// which means "add columns"

	for (k = 0; k < n; k++) {
		cc = D(k, p);
		ss = D(k, q);
		D(k, p) = c * cc + s * ss;
		D(k, q) = c * ss - s * cc;
	}

	// Q = GQ

	// which means "add rows"

	for (k = 0; k < n; k++) {
		cc = Q(p, k);
		ss = Q(q, k);
		Q(p, k) = c * cc + s * ss;
		Q(q, k) = c * ss - s * cc;
	}

	D(p, q) = 0.0;
	D(q, p) = 0.0;
}

#if SELFTEST

static char *s[] = {

	"eigen(A)",
	"Stop: eigen: argument is not a square matrix",

	"eigenval(A)",
	"eigenval(A)",

	"eigenvec(A)",
	"eigenvec(A)",

	"eigen((1,2))",
	"Stop: eigen: argument is not a square matrix",

	"eigen(((1,2),(1,2)))",
	"Stop: eigen: matrix is not symmetrical",

	"eigenval(((1,1,1,1),(1,2,3,4),(1,3,6,10),(1,4,10,20)))",
	"((0.038016,0,0,0),(0,0.453835,0,0),(0,0,2.20345,0),(0,0,0,26.3047))",

	"eigenvec(((1,1,1,1),(1,2,3,4),(1,3,6,10),(1,4,10,20)))",
	"((0.308686,-0.72309,0.594551,-0.168412),(0.787275,-0.163234,-0.532107,0.265358),(0.530366,0.640332,0.391832,-0.393897),(0.0601867,0.201173,0.458082,0.863752))",

	"eigen(hilbert(50))",
	"",

	"1+trace(hilbert(50))-trace(dot(transpose(Q),D,Q))",
	"1",

	"D=quote(D)",
	"",

	"Q=quote(Q)",
	"",

	"A=hilbert(3)",
	"",

	"eigen(A)",
	"",

	"D-eigenval(A)",
	"((0,0,0),(0,0,0),(0,0,0))",

	"Q-eigenvec(A)",
	"((0,0,0),(0,0,0),(0,0,0))",

	"A=quote(A)",
	"",

	"D=quote(D)",
	"",

	"Q=quote(Q)",
	"",
};

void
test_eigen(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
