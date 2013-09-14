// Bignum square root

#include "stdafx.h"
#include "defs.h"

#if 0 // not used anymore

unsigned int *
msqrt(unsigned int *n)
{
	int i, k, kk;
	unsigned int m, *x, *y;

	if (MLENGTH(n) == 1 && n[0] == 0) {
		x = mint(0);
		return x;
	}

	// count number of bits

	k = 32 * (MLENGTH(n) - 1);

	m = n[MLENGTH(n) - 1];

	while (m) {
		m >>= 1;
		k++;
	}

	k = (k - 1) / 2;

	// initial guess

	kk = k / 32 + 1;
	x = mnew(kk);
	MSIGN(x) = 1;
	MLENGTH(x) = kk;
	for (i = 0; i < kk; i++)
		x[i] = 0;
	mp_set_bit(x, k);

	while (--k >= 0) {
		mp_set_bit(x, k);
		y = mmul(x, x);
		if (mcmp(y, n) == 1)
			mp_clr_bit(x, k);
		mfree(y);
	}

	return x;
}

void
test_msqrt(void)
{
	int i;
	unsigned int *n, *x, *y;
	logout("testing msqrt\n");
	for (i = 0; i < 1000000; i++) {
		n = mint(i);
		x = msqrt(n);
		y = mint((int) (sqrt((double) i) + 1e-10));
		if (mcmp(x, y) != 0) {
			sprintf(logbuf, "failed for %d got %u\n", i, x[0]);
			logout(logbuf);
			errout();
		}
		mfree(n);
		mfree(x);
		mfree(y);
	}
	logout("ok\n");
}

#endif
