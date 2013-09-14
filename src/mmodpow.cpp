// Bignum modular power (x^n mod m)

#include "stdafx.h"
#include "defs.h"

// could do indexed bit test instead of shift right

unsigned int *
mmodpow(unsigned int *x, unsigned int *n, unsigned int *m)
{
	unsigned int *y, *z;
	x = mcopy(x);
	n = mcopy(n);
	y = mint(1);
	while (1) {
		if (n[0] & 1) {
			z = mmul(y, x);
			mfree(y);
			y = mmod(z, m);
			mfree(z);
		}
		mshiftright(n);
		if (MZERO(n))
			break;
		z = mmul(x, x);
		mfree(x);
		x = mmod(z, m);
		mfree(z);
	}
	mfree(x);
	mfree(n);
	return y;
}

#if SELFTEST

void
test_mmodpow(void)
{
	int mem;
	int x, n, m;
	unsigned int *xx, *nn, *mm, *y;
	mem = mtotal;
	for (x = 1; x < 100; x++) {
		xx = mint(x);
		for (n = 1; n < 100; n++) {
			nn = mint(n);
			for (m = 1; m < 10; m++) {
				mm = mint(m);
				y = mmodpow(xx, nn, mm);
				mfree(y);
				mfree(mm);
			}
			mfree(nn);
		}
		mfree(xx);
	}
	if (mem != mtotal) {
		sprintf(logbuf, "mmodpow memory leak %d %d\n", mem, mtotal);
		logout(logbuf);
		errout();
	}
}

#endif
