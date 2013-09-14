// 'for' function

#include "stdafx.h"
#include "defs.h"

#define A p3
#define B p4
#define I p5
#define X p6

void
eval_for(void)
{
	int i, j, k;

	// 1st arg (quoted)

	X = cadr(p1);
	if (!issymbol(X))
		stop("for: 1st arg?");

	// 2nd arg

	push(caddr(p1));
	eval();
	j = pop_integer();
	if (j == (int) 0x80000000)
		stop("for: 2nd arg?");

	// 3rd arg

	push(cadddr(p1));
	eval();
	k = pop_integer();
	if (k == (int) 0x80000000)
		stop("for: 3rd arg?");

	// remaining args

	p1 = cddddr(p1);

	B = get_binding(X);
	A = get_arglist(X);

	for (i = j; i <= k; i++) {
		push_integer(i);
		I = pop();
		set_binding(X, I);
		p2 = p1;
		while (iscons(p2)) {
			push(car(p2));
			eval();
			pop();
			p2 = cdr(p2);
		}
	}

	set_binding_and_arglist(X, B, A);

	// return value

	push_symbol(NIL);
}

