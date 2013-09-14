/* Remove terms that involve a given symbol or expression. For example...

	filter(x^2 + x + 1, x)		=>	1

	filter(x^2 + x + 1, x^2)	=>	x + 1
*/

#include "stdafx.h"
#include "defs.h"

void
eval_filter(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		filter();
		p1 = cdr(p1);
	}
}

/* For example...

	push(F)
	push(X)
	filter()
	F = pop()
*/

void
filter(void)
{
	save();
	p2 = pop();
	p1 = pop();
	filter_main();
	restore();
}

void
filter_main(void)
{
	if (car(p1) == symbol(ADD))
		filter_sum();
	else if (istensor(p1))
		filter_tensor();
	else if (find(p1, p2))
		push_integer(0);
	else
		push(p1);
}

void
filter_sum(void)
{
	push_integer(0);
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		filter();
		add();
		p1 = cdr(p1);
	}
}

void
filter_tensor(void)
{
	int i, n;
	n = p1->u.tensor->nelem;
	p3 = alloc_tensor(n);
	p3->u.tensor->ndim = p1->u.tensor->ndim;
	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];
	for (i = 0; i < n; i++) {
		push(p1->u.tensor->elem[i]);
		push(p2);
		filter();
		p3->u.tensor->elem[i] = pop();
	}
	push(p3);
}
