// bignum scanner

#include "stdafx.h"
#include "defs.h"

static unsigned int *addf(unsigned int *, int);
static unsigned int *mulf(unsigned int *, int);

unsigned int *
mscan(char *s)
{
	int sign;
	unsigned int *a, *b, *c;

	sign = 1;

	if (*s == '-') {
		sign = -1;
		s++;
	}

	a = mint(0);

	while (*s) {
		b = mulf(a, 10);
		c = addf(b, *s - '0');
		mfree(a);
		mfree(b);
		a = c;
		s++;
	}

	if (!MZERO(a))
		MSIGN(a) *= sign;

	return a;
}

static unsigned int *
addf(unsigned int *a, int n)
{
	unsigned int *b, *c;
	b = mint(n);
	c = madd(a, b);
	mfree(b);
	return c;
}

static unsigned int *
mulf(unsigned int *a, int n)
{
	unsigned int *b, *c;
	b = mint(n);
	c = mmul(a, b);
	mfree(b);
	return c;
}
