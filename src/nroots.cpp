// find the roots of a polynomial numerically

#include "stdafx.h"
#include "defs.h"

#define YMAX 101
#define DELTA 1.0e-6
#define EPSILON 1.0e-9
#define ABS(z) sqrt((z).r * (z).r + (z).i * (z).i)
#define RANDOM (4.0 * (double) rand() / (double) RAND_MAX - 2.0)

static struct {
	double r, i;
} a, b, x, y, fa, fb, dx, df, c[YMAX];

void
eval_nroots(void)
{
	int h, i, k, n;

	push(cadr(p1));
	eval();

	push(caddr(p1));
	eval();
	p2 = pop();
	if (p2 == symbol(NIL))
		guess();
	else
		push(p2);

	p2 = pop();
	p1 = pop();

	if (!ispoly(p1, p2))
		stop("nroots: polynomial?");

	// mark the stack

	h = tos;

	// get the coefficients

	push(p1);
	push(p2);
	n = coeff();
	if (n > YMAX)
		stop("nroots: degree?");

	// convert the coefficients to real and imaginary doubles

	for (i = 0; i < n; i++) {
		push(stack[h + i]);
		real();
		yyfloat();
		eval();
		p1 = pop();
		push(stack[h + i]);
		imag();
		yyfloat();
		eval();
		p2 = pop();
		if (!isdouble(p1) || !isdouble(p2))
			stop("nroots: coefficients?");
		c[i].r = p1->u.d;
		c[i].i = p2->u.d;
	}

	// pop the coefficients

	tos = h;

	// n is the number of coefficients, n = deg(p) + 1

	monic(n);

	for (k = n; k > 1; k--) {
		findroot(k);
		if (fabs(a.r) < DELTA)
			a.r = 0.0;
		if (fabs(a.i) < DELTA)
			a.i = 0.0;
		push_double(a.r);
		push_double(a.i);
		push(imaginaryunit);
		multiply();
		add();
		divpoly(k);
	}

	// now make n equal to the number of roots

	n = tos - h;

	if (n > 1) {
		sort_stack(n);
		p1 = alloc_tensor(n);
		p1->u.tensor->ndim = 1;
		p1->u.tensor->dim[0] = n;
		for (i = 0; i < n; i++)
			p1->u.tensor->elem[i] = stack[h + i];
		tos = h;
		push(p1);
	}
}

// divide the polynomial by its leading coefficient

void
monic(int n)
{
	int k;
	double t;
	y = c[n - 1];
	t = y.r * y.r + y.i * y.i;
	for (k = 0; k < n - 1; k++) {
		c[k].r = (c[k].r * y.r + c[k].i * y.i) / t;
		c[k].i = (c[k].i * y.r - c[k].r * y.i) / t;
	}
	c[n - 1].r = 1.0;
	c[n - 1].i = 0.0;
}

// uses the secant method

void
findroot(int n)
{
	int j, k;
	double t;

	if (ABS(c[0]) < DELTA) {
		a.r = 0.0;
		a.i = 0.0;
		return;
	}

	for (j = 0; j < 100; j++) {

		a.r = RANDOM;
		a.i = RANDOM;

		compute_fa(n);

		b = a;
		fb = fa;

		a.r = RANDOM;
		a.i = RANDOM;

		for (k = 0; k < 1000; k++) {

			compute_fa(n);

			if (ABS(fa) < EPSILON)
				return;

			if (ABS(fa) < ABS(fb)) {
				x = a;
				a = b;
				b = x;
				x = fa;
				fa = fb;
				fb = x;
			}

			// dx = b - a

			dx.r = b.r - a.r;
			dx.i = b.i - a.i;

			// df = fb - fa

			df.r = fb.r - fa.r;
			df.i = fb.i - fa.i;

			// y = dx / df

			t = df.r * df.r + df.i * df.i;

			if (t == 0.0)
				break;

			y.r = (dx.r * df.r + dx.i * df.i) / t;
			y.i = (dx.i * df.r - dx.r * df.i) / t;

			// a = b - y * fb

			a.r = b.r - (y.r * fb.r - y.i * fb.i);
			a.i = b.i - (y.r * fb.i + y.i * fb.r);
		}
	}

	stop("nroots: convergence error");
}

void
compute_fa(int n)
{
	int k;
	double t;

	// x = a

	x.r = a.r;
	x.i = a.i;

	// fa = c0 + c1 * x

	fa.r = c[0].r + c[1].r * x.r - c[1].i * x.i;
	fa.i = c[0].i + c[1].r * x.i + c[1].i * x.r;

	for (k = 2; k < n; k++) {

		// x = a * x

		t = a.r * x.r - a.i * x.i;
		x.i = a.r * x.i + a.i * x.r;
		x.r = t;

		// fa += c[k] * x

		fa.r += c[k].r * x.r - c[k].i * x.i;
		fa.i += c[k].r * x.i + c[k].i * x.r;
	}
}

// divide the polynomial by x - a

void
divpoly(int n)
{
	int k;
	for (k = n - 1; k > 0; k--) {
		c[k - 1].r += c[k].r * a.r - c[k].i * a.i;
		c[k - 1].i += c[k].i * a.r + c[k].r * a.i;
	}
	if (ABS(c[0]) > DELTA)
		stop("nroots: residual error");
	for (k = 0; k < n - 1; k++) {
		c[k].r = c[k + 1].r;
		c[k].i = c[k + 1].i;
	}
}

#if SELFTEST

static char *s[] = {

	"nroots(x)",
	"0",

	"nroots((1+i)*x^2+1)",
	"(-0.17178-0.727673*i,0.17178+0.727673*i)",

	"nroots(sqrt(2)*exp(i*pi/4)*x^2+1)",
	"(-0.17178-0.727673*i,0.17178+0.727673*i)",

//	"nroots(x^4+1)",
//	"(-0.707107+0.707107*i,-0.707107-0.707107*i,0.707107+0.707107*i,0.707107-0.707107*i)",
};

void
test_nroots(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
