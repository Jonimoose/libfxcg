// Bignum addition and subtraction

#include "stdafx.h"
#include "defs.h"

static unsigned int *addf(unsigned int *, unsigned int *);
static unsigned int *subf(unsigned int *, unsigned int *);
static int ucmp(unsigned int *, unsigned int *);

unsigned int *
madd(unsigned int *a, unsigned int *b)
{
	if (MSIGN(a) == MSIGN(b))
		return addf(a, b);	// same sign, add together
	else
		return subf(a, b);	// opposite sign, find difference
}

unsigned int *
msub(unsigned int *a, unsigned int *b)
{
	if (MSIGN(a) == MSIGN(b))
		return subf(a, b);	// same sign, find difference
	else
		return addf(a, b);	// opposite sign, add together
}

static unsigned int *
addf(unsigned int *a, unsigned int *b)
{
	int i, sign;
	unsigned int c, *x;

	sign = MSIGN(a);

	if (MLENGTH(a) < MLENGTH(b)) {
		x = a;
		a = b;
		b = x;
	}

	x = mnew(MLENGTH(a) + 1);

	c = 0;

	for (i = 0; i < MLENGTH(b); i++) {
		x[i] = a[i] + b[i] + c;
		if (c)
			if (a[i] >= x[i])
				c = 1;
			else
				c = 0;
		else
			if (a[i] > x[i])
				c = 1;
			else
				c = 0;
	}

	for (i = MLENGTH(b); i < MLENGTH(a); i++) {
		x[i] = a[i] + c;
		if (a[i] > x[i])
			c = 1;
		else
			c = 0;
	}

	x[MLENGTH(a)] = c;

	for (i = MLENGTH(a); i > 0; i--)
		if (x[i])
			break;

	MLENGTH(x) = i + 1;
	MSIGN(x) = sign;

	return x;
}

static unsigned int *
subf(unsigned int *a, unsigned int *b)
{
	int i, sign = 0;
	unsigned int c, *x;

	switch (ucmp(a, b)) {

	case 0:
		return mint(0);

	case 1:
		sign = MSIGN(a);	/* |a| > |b| */
		break;

	case -1:
		sign = -MSIGN(a);	/* |a| < |b| */
		x = a;
		a = b;
		b = x;
		break;
	}

	x = mnew(MLENGTH(a));

	c = 0;

	for (i = 0; i < MLENGTH(b); i++) {
		x[i] = a[i] - b[i] - c;
		if (c)
			if (a[i] <= x[i])
				c = 1;
			else
				c = 0;
		else
			if (a[i] < x[i])
				c = 1;
			else
				c = 0;
	}

	for (i = MLENGTH(b); i < MLENGTH(a); i++) {
		x[i] = a[i] - c;
		if (a[i] < x[i])
			c = 1;
		else
			c = 0;
	}

	for (i = MLENGTH(a) - 1; i > 0; i--)
		if (x[i])
			break;

	MLENGTH(x) = i + 1;
	MSIGN(x) = sign;

	return x;
}

// unsigned compare

static int
ucmp(unsigned int *a, unsigned int *b)
{
	int i;

	if (MLENGTH(a) < MLENGTH(b))
		return -1;

	if (MLENGTH(a) > MLENGTH(b))
		return 1;

	for (i = MLENGTH(a) - 1; i > 0; i--)
		if (a[i] != b[i])
			break;

	if (a[i] < b[i])
		return -1;

	if (a[i] > b[i])
		return 1;

	return 0;
}

#if SELFTEST

// small integer tests

void
test_madd(void)
{
	int i, j, m;
	logout("test madd\n");
	m = mtotal;
	for (i = -100; i <= 100; i++)
		for (j = -100; j <= 100; j++)
			test_maddf(i, j, i + j);
	if (m != mtotal) {
		logout("memory leak\n");
		errout();
	}
	logout("ok\n");
}

void
test_maddf(int na, int nb, int nc)
{
	unsigned int *a, *b, *c, *d;

	a = mint(na);
	b = mint(nb);
	c = mint(nc);

	d = madd(a, b);

	if (mcmp(c, d) == 0) {
		mfree(a);
		mfree(b);
		mfree(c);
		mfree(d);
		return;
	}

	sprintf(logbuf, "%d %d %d %d\n", na, nb, nc, *d * MSIGN(d));
	logout(logbuf);
	errout();
}

void
test_msub(void)
{
	int i, j, m;
	logout("test msub\n");
	m = mtotal;
	for (i = -100; i <= 100; i++)
		for (j = -100; j <= 100; j++)
			test_msubf(i, j, i - j);
	if (m != mtotal) {
		logout("memory leak\n");
		errout();
	}
	logout("ok\n");
}

void
test_msubf(int na, int nb, int nc)
{
	unsigned int *a, *b, *c, *d;

	a = mint(na);
	b = mint(nb);
	c = mint(nc);

	d = msub(a, b);

	if (mcmp(c, d) == 0) {
		mfree(a);
		mfree(b);
		mfree(c);
		mfree(d);
		return;
	}

	sprintf(logbuf, "%d %d %d %d\n", na, nb, nc, *d * MSIGN(d));
	logout(logbuf);
	errout();
}

#endif
