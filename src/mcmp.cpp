// Bignum compare
//
//	returns
//
//	-1		a < b
//
//	0		a = b
//
//	1		a > b

#include "stdafx.h"
#include "defs.h"

int
mcmp(unsigned int *a, unsigned int *b)
{
	int i;

	if (MSIGN(a) == -1 && MSIGN(b) == 1)
		return -1;

	if (MSIGN(a) == 1 && MSIGN(b) == -1)
		return 1;

	// same sign

	if (MLENGTH(a) < MLENGTH(b)) {
		if (MSIGN(a) == 1)
			return -1;
		else
			return 1;
	}

	if (MLENGTH(a) > MLENGTH(b)) {
		if (MSIGN(a) == 1)
			return 1;
		else
			return -1;
	}

	// same length

	for (i = MLENGTH(a) - 1; i > 0; i--)
		if (a[i] != b[i])
			break;

	if (a[i] < b[i]) {
		if (MSIGN(a) == 1)
			return -1;
		else
			return 1;
	}

	if (a[i] > b[i]) {
		if (MSIGN(a) == 1)
			return 1;
		else
			return -1;
	}

	return 0;
}

int
mcmpint(unsigned int *a, int n)
{
	int t;
	unsigned int *b;
	b = mint(n);
	t = mcmp(a, b);
	mfree(b);
	return t;
}

#if SELFTEST

void
test_mcmp(void)
{
	int i, j, k;
	unsigned int *x, *y;
	logout("testing mcmp\n");
	for (i = -1000; i < 1000; i++) {
		x = mint(i);
		for (j = -1000; j < 1000; j++) {
			y = mint(j);
			k = mcmp(x, y);
			if (i == j && k != 0) {
				logout("failed\n");
				errout();
			}
			if (i < j && k != -1) {
				logout("failed\n");
				errout();
			}
			if (i > j && k != 1) {
				logout("failed\n");
				errout();
			}
			mfree(y);
		}
		mfree(x);
	}
	logout("ok\n");
}

#endif
