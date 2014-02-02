//-----------------------------------------------------------------------------
//
//	Bignum root
//
//	Returns null pointer if not perfect root.
//
//	The sign of the radicand is ignored.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

unsigned int *
mroot(unsigned int *n, unsigned int index)
{
	int i, j, k;
	unsigned int m, *x, *y;

	if (index == 0)
		stop("root index is zero");

	// count number of bits

	k = 32 * (MLENGTH(n) - 1);

	m = n[MLENGTH(n) - 1];

	while (m) {
		m >>= 1;
		k++;
	}

	if (k == 0)
		return mint(0);

	// initial guess

	k = (k - 1) / index;

	j = k / 32 + 1;
	x = mnew(j);
	MSIGN(x) = 1;
	MLENGTH(x) = j;
	for (i = 0; i < j; i++)
		x[i] = 0;

	while (k >= 0) {
		mp_set_bit(x, k);
		y = mpow(x, index);
		switch (mcmp(y, n)) {
		case -1:
			break;
		case 0:
			mfree(y);
			return x;
		case 1:
			mp_clr_bit(x, k);
			break;
		}
		mfree(y);
		k--;
	}

	mfree(x);

	return 0;
}

#if SELFTEST

void
test_mroot(void)
{
	int i, j, mem;
	unsigned int *a, *b, *c;

	logout("testing mroot\n");

	mem = mtotal;

	// small numbers

	for (i = 0; i < 10; i++) {
		a = mint(i);
		for (j = 1; j < 10; j++) {
			b = mpow(a, j);
			c = mroot(b, j);
			if (c == 0 || mcmp(a, c) != 0) {
				sprintf(logbuf, "failed a=%d b=%d c=%d\n", a[0], b[0], c[0]);
				logout(logbuf);
				errout();
			}
			mfree(b);
			mfree(c);
		}
		mfree(a);
	}

	a = mint(12345);

	for (i = 1; i < 10; i++) {
		b = mpow(a, i);
		c = mroot(b, i);
		if (c == 0 || mcmp(a, c) != 0) {
			logout("failed\n");
			errout();
		}
		mfree(b);
		mfree(c);
	}

	mfree(a);

	if (mtotal != mem) {
		logout("memory leak\n");
		errout();
	}

	logout("ok\n");
}

#endif
