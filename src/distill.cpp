#include "stdafx.h"

#include "defs.h"

static void _distill(void);

//	take expr and push all constant subexpr

//	p1	expr

//	p2	independent variable (like x)

void
distill(void)
{
	save();
	_distill();
	restore();
}

static void
_distill(void)
{
	p2 = pop();
	p1 = pop();

	// is the entire expression constant?

	if (find(p1, p2) == 0) {
		push(p1);
		//push(p1);	// may need later for pushing both +a, -a
		//negate();
		return;
	}

	// sum?

	if (isadd(p1)) {
		distill_sum();
		return;
	}

	// product?

	if (car(p1) == symbol(MULTIPLY)) {
		distill_product();
		return;
	}

	// naive distill if not sum or product

	p3 = cdr(p1);
	while (iscons(p3)) {
		push(car(p3));
		push(p2);
		distill();
		p3 = cdr(p3);
	}
}

void
distill_sum(void)
{
	int h;

	// distill terms involving x

	p3 = cdr(p1);

	while (iscons(p3)) {
		if (find(car(p3), p2)) {
			push(car(p3));
			push(p2);
			distill();
		}
		p3 = cdr(p3);
	}

	// add together all constant terms

	h = tos;

	p3 = cdr(p1);

	while (iscons(p3)) {
		if (find(car(p3), p2) == 0)
			push(car(p3));
		p3 = cdr(p3);
	}

	if (tos - h) {
		add_all(tos - h);
		p3 = pop();
		push(p3);
		push(p3);
		negate();	// need both +a, -a for some integrals
	}
}

void
distill_product(void)
{
	int h;

	// distill factors involving x

	p3 = cdr(p1);

	while (iscons(p3)) {
		if (find(car(p3), p2)) {
			push(car(p3));
			push(p2);
			distill();
		}
		p3 = cdr(p3);
	}

	// multiply together all constant factors

	h = tos;

	p3 = cdr(p1);

	while (iscons(p3)) {
		if (find(car(p3), p2) == 0)
			push(car(p3));
		p3 = cdr(p3);
	}

	if (tos - h) {
		multiply_all(tos - h);
		//p3 = pop();	// may need later for pushing both +a, -a
		//push(p3);
		//push(p3);
		//negate();
	}
}
