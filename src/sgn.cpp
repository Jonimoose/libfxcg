#include "defs.h"

void
eval_sgn(void)
{
	push(cadr(p1));
	eval();
	sgn();
}

void
sgn(void)
{
	save();

	p1 = pop();

	if (!isnum(p1)) {
		push_symbol(SGN);
		push(p1);
		list(2);
	} else if (iszero(p1))
		push_integer(0);
	else if (isnegativenumber(p1))
		push_integer(-1);
	else
		push_integer(1);

	restore();
}
