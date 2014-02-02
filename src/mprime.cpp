// Bignum prime test (returns 1 if prime, 0 if not)

// Uses Algorithm P (probabilistic primality test) from p. 395 of
// "The Art of Computer Programming, Volume 2" by Donald E. Knuth.

#include "stdafx.h"
#include "defs.h"

static int mprimef(unsigned int *, unsigned int *, int);

int
mprime(unsigned int *n)
{
	int i, k;
	unsigned int *q;

	// 1?

	if (MLENGTH(n) == 1 && n[0] == 1)
		return 0;

	// 2?

	if (MLENGTH(n) == 1 && n[0] == 2)
		return 1;

	// even?

	if ((n[0] & 1) == 0)
		return 0;

	// n = 1 + (2 ^ k) q

	q = mcopy(n);

	k = 0;
	do {
		mshiftright(q);
		k++;
	} while ((q[0] & 1) == 0);

	// try 25 times

	for (i = 0; i < 25; i++)
		if (mprimef(n, q, k) == 0)
			break;

	mfree(q);

	if (i < 25)
		return 0;
	else
		return 1;
}

//-----------------------------------------------------------------------------
//
//	This is the actual implementation of Algorithm P.
//
//	Input:		n		The number in question.
//
//			q		n = 1 + (2 ^ k) q
//
//			k
//
//	Output:		1		when n is probably prime
//
//			0		when n is definitely not prime
//
//-----------------------------------------------------------------------------

static int
mprimef(unsigned int *n, unsigned int *q, int k)
{
	int i, j;
	unsigned int *t, *x, *y;

	// generate x

	t = mcopy(n);

	while (1) {
		for (i = 0; i < MLENGTH(t); i++)
			t[i] = rand();
		x = mmod(t, n);
		if (!MZERO(x) && !MEQUAL(x, 1))
			break;
		mfree(x);
	}

	mfree(t);

	// exponentiate

	y = mmodpow(x, q, n);

	// done?

	if (MEQUAL(y, 1)) {
		mfree(x);
		mfree(y);
		return 1;
	}

	j = 0;

	while (1) {

		// y = n - 1?

		t = msub(n, y);

		if (MEQUAL(t, 1)) {
			mfree(t);
			mfree(x);
			mfree(y);
			return 1;
		}

		mfree(t);

		if (++j == k) {
			mfree(x);
			mfree(y);
			return 0;
		}

		// y = (y ^ 2) mod n

		t = mmul(y, y);
		mfree(y);
		y = mmod(t, n);
		mfree(t);

		// y = 1?

		if (MEQUAL(y, 1)) {
			mfree(x);
			mfree(y);
			return 0;
		}
	}
}

#if SELFTEST

void
test_mprime(void)
{
	int i, k, m, t;
	unsigned int *n;
	logout("test mprime\n");
	m = mtotal;
	k = 0;
	for (i = 0; i < 10000; i++) {
		n = mint(i);
		t = mprime(n);
		mfree(n);
		if (i == primetab[k]) {
			if (t == 0) {
				sprintf(logbuf, "failed for prime number %d\n", i);
				logout(logbuf);
				errout();
			}
			k++;
		} else if (t == 1) {
			sprintf(logbuf, "failed for composite number %d\n", i);
			logout(logbuf);
			errout();
		}
	}
	if (m != mtotal) {
		logout("memory leak\n");
		errout();
	}
	logout("ok\n");
}

#endif
