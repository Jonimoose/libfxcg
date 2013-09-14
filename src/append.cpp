// Append one list to another.

#include "stdafx.h"
#include "defs.h"

void
append(void)
{
	int h;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	while (iscons(p1)) {
		push(car(p1));
		p1 = cdr(p1);
	}

	while (iscons(p2)) {
		push(car(p2));
		p2 = cdr(p2);
	}

	list(tos - h);

	restore();
}
