#include "stdafx.h"
#include "defs.h"

void
eval_decomp(void)
{
	int h = tos;
	push(symbol(NIL));
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p1 = pop();
	if (p1 == symbol(NIL))
		guess();
	else
		push(p1);
	decomp();
	list(tos - h);
}

// returns constant expresions on the stack

void
decomp(void)
{
	save();

	p2 = pop();
	p1 = pop();

	// is the entire expression constant?

	if (find(p1, p2) == 0) {
		push(p1);
		//push(p1);	// may need later for pushing both +a, -a
		//negate();
		restore();
		return;
	}

	// sum?

	if (isadd(p1)) {
		decomp_sum();
		restore();
		return;
	}

	// product?

	if (car(p1) == symbol(MULTIPLY)) {
		decomp_product();
		restore();
		return;
	}

	// naive decomp if not sum or product

	p3 = cdr(p1);
	while (iscons(p3)) {
		push(car(p3));
		push(p2);
		decomp();
		p3 = cdr(p3);
	}

	restore();
}

void
decomp_sum(void)
{
	int h;

	// decomp terms involving x

	p3 = cdr(p1);

	while (iscons(p3)) {
		if (find(car(p3), p2)) {
			push(car(p3));
			push(p2);
			decomp();
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
decomp_product(void)
{
	int h;

	// decomp factors involving x

	p3 = cdr(p1);

	while (iscons(p3)) {
		if (find(car(p3), p2)) {
			push(car(p3));
			push(p2);
			decomp();
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
