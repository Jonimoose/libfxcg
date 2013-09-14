// Complex conjugate

#include "stdafx.h"
#include "defs.h"

void
eval_conj(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	push(p1);
	if (!find(p1, imaginaryunit)) { // example: (-1)^(1/3)
		polar();
		conjugate();
		clockform();
	} else
		conjugate();
}


void
conjugate(void)
{
	push(imaginaryunit);
	push(imaginaryunit);
	negate();
	subst();
	eval();
}
