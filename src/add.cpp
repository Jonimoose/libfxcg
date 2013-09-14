/* Symbolic addition

	Terms in a sum are combined if they are identical modulo rational
	coefficients.

	For example, A + 2A becomes 3A.

	However, the sum A + sqrt(2) A is not modified.

	Combining terms can lead to second-order effects.

	For example, consider the case of

		1/sqrt(2) A + 3/sqrt(2) A + sqrt(2) A

	The first two terms are combined to yield 2 sqrt(2) A.

	This result can now be combined with the third term to yield

		3 sqrt(2) A
*/

#include "stdafx.h"
#include "defs.h"

static int flag;

void
eval_add(void)
{
	int h = tos;
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		p2 = pop();
		push_terms(p2);
		p1 = cdr(p1);
	}
	add_terms(tos - h);
}

/* Add n terms, returns one expression on the stack. */

void
add_terms(int n)
{
	int i, h;
	U **s;

	h = tos - n;

	s = stack + h;

	/* ensure no infinite loop, use "for" */

	for (i = 0; i < 10; i++) {

		if (n < 2)
			break;

		flag = 0;

		qsort(s, n, sizeof (U *), cmp_terms);

		if (flag == 0)
			break;

		n = combine_terms(s, n);
	}

	tos = h + n;

	switch (n) {
	case 0:
		push_integer(0);
		break;
	case 1:
		break;
	default:
		list(n);
		p1 = pop();
		push_symbol(ADD);
		push(p1);
		cons();
		break;
	}
}

/* Compare terms for order, clobbers p1 and p2. */

int
cmp_terms(const void *q1, const void *q2)
{
	int i, t;

	p1 = *((U **) q1);
	p2 = *((U **) q2);

	/* numbers can be combined */

	if (isnum(p1) && isnum(p2)) {
		flag = 1;
		return 0;
	}

	/* congruent tensors can be combined */

	if (istensor(p1) && istensor(p2)) {
		if (p1->u.tensor->ndim < p2->u.tensor->ndim)
			return -1;
		if (p1->u.tensor->ndim > p2->u.tensor->ndim)
			return 1;
		for (i = 0; i < p1->u.tensor->ndim; i++) {
			if (p1->u.tensor->dim[i] < p2->u.tensor->dim[i])
				return -1;
			if (p1->u.tensor->dim[i] > p2->u.tensor->dim[i])
				return 1;
		}
		flag = 1;
		return 0;
	}

	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		if (isnum(car(p1))) {
			p1 = cdr(p1);
			if (cdr(p1) == symbol(NIL))
				p1 = car(p1);
		}
	}

	if (car(p2) == symbol(MULTIPLY)) {
		p2 = cdr(p2);
		if (isnum(car(p2))) {
			p2 = cdr(p2);
			if (cdr(p2) == symbol(NIL))
				p2 = car(p2);
		}
	}

	t = cmp_expr(p1, p2);

	if (t == 0)
		flag = 1;

	return t;
}

/* Compare adjacent terms in s[] and combine if possible.

	Returns the number of terms remaining in s[].

	n	number of terms in s[] initially
*/

int
combine_terms(U **s, int n)
{
	int i, j, t;

	for (i = 0; i < n - 1; i++) {
		check_esc_flag();

		p3 = s[i];
		p4 = s[i + 1];

		if (istensor(p3) && istensor(p4)) {
			push(p3);
			push(p4);
			tensor_plus_tensor();
			p1 = pop();
			if (p1 != symbol(NIL)) {
				s[i] = p1;
				for (j = i + 1; j < n - 1; j++)
					s[j] = s[j + 1];
				n--;
				i--;
			}
			continue;
		}

		if (istensor(p3) || istensor(p4))
			continue;

		if (isnum(p3) && isnum(p4)) {
			push(p3);
			push(p4);
			add_numbers();
			p1 = pop();
			if (iszero(p1)) {
				for (j = i; j < n - 2; j++)
					s[j] = s[j + 2];
				n -= 2;
			} else {
				s[i] = p1;
				for (j = i + 1; j < n - 1; j++)
					s[j] = s[j + 1];
				n--;
			}
			i--;
			continue;
		}

		if (isnum(p3) || isnum(p4))
			continue;

		p1 = one;
		p2 = one;

		t = 0;

		if (car(p3) == symbol(MULTIPLY)) {
			p3 = cdr(p3);
			t = 1; /* p3 is now denormal */
			if (isnum(car(p3))) {
				p1 = car(p3);
				p3 = cdr(p3);
				if (cdr(p3) == symbol(NIL)) {
					p3 = car(p3);
					t = 0;
				}
			}
		}

		if (car(p4) == symbol(MULTIPLY)) {
			p4 = cdr(p4);
			if (isnum(car(p4))) {
				p2 = car(p4);
				p4 = cdr(p4);
				if (cdr(p4) == symbol(NIL))
					p4 = car(p4);
			}
		}

		if (!equal(p3, p4))
			continue;

		push(p1);
		push(p2);
		add_numbers();

		p1 = pop();

		if (iszero(p1)) {
			for (j = i; j < n - 2; j++)
				s[j] = s[j + 2];
			n -= 2;
			i--;
			continue;
		}

		push(p1);

		if (t) {
			push(symbol(MULTIPLY));
			push(p3);
			cons();
		} else
			push(p3);

		multiply();

		s[i] = pop();

		for (j = i + 1; j < n - 1; j++)
			s[j] = s[j + 1];

		n--;
		i--;
	}

	return n;
}

void
push_terms(U *p)
{
	if (car(p) == symbol(ADD)) {
		p = cdr(p);
		while (iscons(p)) {
			push(car(p));
			p = cdr(p);
		}
	} else if (!iszero(p))
		push(p);
}

/* add two expressions */

void
add()
{
	int h;
	save();
	p2 = pop();
	p1 = pop();
	h = tos;
	push_terms(p1);
	push_terms(p2);
	add_terms(tos - h);
	restore();
}

void
add_all(int k)
{
	int h, i;
	U **s;
	save();
	s = stack + tos - k;
	h = tos;
	for (i = 0; i < k; i++)
		push_terms(s[i]);
	add_terms(tos - h);
	p1 = pop();
	tos -= k;
	push(p1);
	restore();
}

void
subtract(void)
{
	negate();
	add();
}
