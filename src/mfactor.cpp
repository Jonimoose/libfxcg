// For odd n, returns the largest factor less than or equal to sqrt(n)

#include "stdafx.h"
#include "defs.h"

#if 0 // not used anymore

unsigned int *
mfactor(unsigned int *n)
{
	unsigned int *r, *root, *t, *two, *x, *y;

	two = mint(2);

	root = msqrt(n);

	// y = 1;

	y = mint(1);

	// x = 2 isqrt(n) + 1

	t = madd(root, root);
	x = madd(t, y);
	mfree(t);

	// r = isqrt(n) ^ 2 - n

	t = mmul(root, root);
	r = msub(t, n);
	mfree(t);

	mfree(root);

	while (1) {

		if (MZERO(r)) {

			// n = (x - y) / 2

			t = msub(x, y);
			n = mdiv(t, two);
			mfree(t);

			mfree(r);
			mfree(x);
			mfree(y);
			mfree(two);

			return n;
		}

		// r = r + x

		t = madd(r, x);
		mfree(r);
		r = t;

		// x = x + 2

		t = madd(x, two);
		mfree(x);
		x = t;

		while (1) {

			// r = r - y

			t = msub(r, y);
			mfree(r);
			r = t;

			// y = y + 2

			t = madd(y, two);
			mfree(y);
			y = t;

			if (MSIGN(r) == -1 || MZERO(r))
				break;
		}
	}
}

void
test_mfactor(void)
{
	unsigned int *n;
	n = mint(377);
	n = mfactor(n);
	printf("%d\n", n[0]);
}

#endif
