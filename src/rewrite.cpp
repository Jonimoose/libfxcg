// Rewrite by expanding all symbols

#include "stdafx.h"
#include "defs.h"

void
rewrite(void)
{
	int h;
	save();

	p1 = pop();

	if (istensor(p1)) {
		rewrite_tensor();
		restore();
		return;
	}

	if (iscons(p1)) {
		h = tos;
		push(car(p1)); // Do not rewrite function name
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			rewrite();
			p1 = cdr(p1);
		}
		list(tos - h);
		restore();
		return;
	}

	// If not a symbol then done

	if (!issymbol(p1)) {
		push(p1);
		restore();
		return;
	}

	// Get the symbol's binding, try again

	p2 = get_binding(p1);
	push(p2);
	if (p1 != p2)
		rewrite();

	restore();
}

void
rewrite_tensor(void)
{
	int i;
	push(p1);
	copy_tensor();
	p1 = pop();
	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(p1->u.tensor->elem[i]);
		rewrite();
		p1->u.tensor->elem[i] = pop();
	}
	push(p1);
}
