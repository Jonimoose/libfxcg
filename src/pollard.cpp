// Factor using the Pollard rho method

#include "stdafx.h"
#include "defs.h"

static unsigned int *n;

void
factor_number(void)
{
	int h;

	save();

	p1 = pop();

	// 0 or 1?

	if (equaln(p1, 0) || equaln(p1, 1) || equaln(p1, -1)) {
		push(p1);
		restore();
		return;
	}

	n = mcopy(p1->u.q.a);

	h = tos;

	factor_a();

	if (tos - h > 1) {
		list(tos - h);
		push_symbol(MULTIPLY);
		swap();
		cons();
	}

	restore();
}

// factor using table look-up, then switch to rho method if necessary

// From TAOCP Vol. 2 by Knuth, p. 380 (Algorithm A)

void
factor_a(void)
{
	int k;

	if (MSIGN(n) == -1) {
		MSIGN(n) = 1;
		push_integer(-1);
	}

	for (k = 0; k < 10000; k++) {

		try_kth_prime(k);

		// if n is 1 then we're done

		if (MLENGTH(n) == 1 && n[0] == 1) {
			mfree(n);
			return;
		}
	}

	factor_b();
}

void
try_kth_prime(int k)
{
	int count;
	unsigned int *d, *q, *r;

	d = mint(primetab[k]);

	count = 0;

	while (1) {

		// if n is 1 then we're done

		if (MLENGTH(n) == 1 && n[0] == 1) {
			if (count)
				push_factor(d, count);
			else
				mfree(d);
			return;
		}

		mdivrem(&q, &r, n, d);

		// continue looping while remainder is zero

		if (MLENGTH(r) == 1 && r[0] == 0) {
			count++;
			mfree(r);
			mfree(n);
			n = q;
		} else {
			mfree(r);
			break;
		}
	}

	if (count)
		push_factor(d, count);

	// q = n/d, hence if q < d then n < d^2 so n is prime

	if (mcmp(q, d) == -1) {
		push_factor(n, 1);
		n = mint(1);
	}

	if (count == 0)
		mfree(d);

	mfree(q);
}

// From TAOCP Vol. 2 by Knuth, p. 385 (Algorithm B)

int
factor_b(void)
{
	int k, l;
	unsigned int *g, *one, *t, *x, *xprime;

	one = mint(1);
	x = mint(5);
	xprime = mint(2);

	k = 1;
	l = 1;

	while (1) {

		if (mprime(n)) {
			push_factor(n, 1);
			mfree(one);
			mfree(x);
			mfree(xprime);
			return 0;
		}

		while (1) {

			if (esc_flag) {
				mfree(one);
				mfree(n);
				mfree(x);
				mfree(xprime);
				stop("esc");
			}

			// g = gcd(x' - x, n)

			t = msub(xprime, x);
			MSIGN(t) = 1;
			g = mgcd(t, n);
			mfree(t);

			if (MEQUAL(g, 1)) {
				mfree(g);
				if (--k == 0) {
					mfree(xprime);
					xprime = mcopy(x);
					l *= 2;
					k = l;
				}

				// x = (x ^ 2 + 1) mod n

				t = mmul(x, x);
				mfree(x);
				x = madd(t, one);
				mfree(t);
				t = mmod(x, n);
				mfree(x);
				x = t;

				continue;
			}

			push_factor(g, 1);

			if (mcmp(g, n) == 0) {
				mfree(one);
				mfree(n);
				mfree(x);
				mfree(xprime);
				return -1;
			}

			// n = n / g

			t = mdiv(n, g);
			mfree(n);
			n = t;

			// x = x mod n

			t = mmod(x, n);
			mfree(x);
			x = t;

			// xprime = xprime mod n

			t = mmod(xprime, n);
			mfree(xprime);
			xprime = t;

			break;
		}
	}
}

void
push_factor(unsigned int *d, int count)
{
	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = d;
	p1->u.q.b = mint(1);
	push(p1);
	if (count > 1) {
		push_symbol(POWER);
		swap();
		p1 = alloc();
		p1->k = NUM;
		p1->u.q.a = mint(count);
		p1->u.q.b = mint(1);
		push(p1);
		list(3);
	}
}
