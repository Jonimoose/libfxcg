// Bignum power

#include "stdafx.h"
#include "defs.h"

unsigned int *
mpow(unsigned int *a, unsigned int n)
{
	unsigned int *aa, *t;

	a = mcopy(a);

	aa = mint(1);

	for (;;) {

		if (n & 1) {
			t = mmul(aa, a);
			mfree(aa);
			aa = t;
		}

		n >>= 1;

		if (n == 0)
			break;

		t = mmul(a, a);
		mfree(a);
		a = t;
	}

	mfree(a);

	return aa;
}

#if SELFTEST

void
test_mpow(void)
{
	int i, j, mem, x;
	unsigned int *a, *b, *c;

	logout("testing mpow\n");

	mem = mtotal;

	// small numbers

	for (i = -10; i < 10; i++) {
		a = mint(i);
		x = 1;
		for (j = 0; j < 10; j++) {
			b = mpow(a, j);
			c = mint(x);
			if (mcmp(b, c) != 0) {
				sprintf(logbuf, "failed a=%d b=%d c=%d\n", a[0], b[0], c[0]);
				logout(logbuf);
				errout();
			}
			mfree(b);
			mfree(c);
			x *= i;
		}
		mfree(a);
	}

	if (mem != mtotal) {
		logout("memory leak\n");
		errout();
	}

	logout("ok\n");
}

#endif
