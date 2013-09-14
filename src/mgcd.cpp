//-----------------------------------------------------------------------------
//
//	Bignum GCD
//
//	Uses the binary GCD algorithm.
//
//	See "The Art of Computer Programming" p. 338.
//
//	mgcd always returns a positive value
//
//	mgcd(0, 0) = 0
//
//	mgcd(u, 0) = |u|
//
//	mgcd(0, v) = |v|
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

unsigned int *
mgcd(unsigned int *u, unsigned int *v)
{
	int i, k, n;
	unsigned int *t;

	if (MZERO(u)) {
		t = mcopy(v);
		MSIGN(t) = 1;
		return t;
	}

	if (MZERO(v)) {
		t = mcopy(u);
		MSIGN(t) = 1;
		return t;
	}

	u = mcopy(u);
	v = mcopy(v);

	MSIGN(u) = 1;
	MSIGN(v) = 1;

	k = 0;

	while ((u[0] & 1) == 0 && (v[0] & 1) == 0) {
		mshiftright(u);
		mshiftright(v);
		k++;
	}

	if (u[0] & 1) {
		t = mcopy(v);
		MSIGN(t) *= -1;
	} else
		t = mcopy(u);

	while (1) {

		while ((t[0] & 1) == 0)
			mshiftright(t);

		if (MSIGN(t) == 1) {
			mfree(u);
			u = mcopy(t);
		} else {
			mfree(v);
			v = mcopy(t);
			MSIGN(v) *= -1;
		}

		mfree(t);

		t = msub(u, v);

		if (MZERO(t)) {
			mfree(t);
			mfree(v);
			n = (k / 32) + 1;
			v = mnew(n);
			MSIGN(v) = 1;
			MLENGTH(v) = n;
			for (i = 0; i < n; i++)
				v[i] = 0;
			mp_set_bit(v, k);
			t = mmul(u, v);
			mfree(u);
			mfree(v);
			return t;
		}
	}
}

#if SELFTEST

static unsigned int *egcd(unsigned int *, unsigned int *);

void
test_mgcd(void)
{
	int i, j, n;
	unsigned int *a, *b, *c, *d;
	logout("testing mgcd\n");
	n = mtotal;
	for (i = 1; i < 100; i++) {
		a = mint(i);
		for (j = 1; j < 100; j++) {
			b = mint(j);
			c = mgcd(a, b);
			d = egcd(a, b);
			if (mcmp(c, d) != 0) {
				logout("failed\n");
				errout();
			}
			mfree(b);
			mfree(c);
			mfree(d);
		}
		mfree(a);
	}
	if (n != mtotal) {
		logout("memory leak\n");
		errout();
	}
	logout("ok\n");
}

// Euclid's algorithm

static unsigned int *
egcd(unsigned int *a, unsigned int *b)
{
	int sign;
	unsigned int *c;
	if (MZERO(b))
		stop("divide by zero");
	b = mcopy(b);
	if (MZERO(a))
		return b;
	sign = MSIGN(b);
	a = mcopy(a);
	while (!MZERO(b)) {
		c = mmod(a, b);
		mfree(a);
		a = b;
		b = c;
	}
	mfree(b);
	MSIGN(a) = sign;
	return a;
}

#endif
