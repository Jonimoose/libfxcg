// 'sum' function

#include "stdafx.h"
#include "defs.h"

#define A p3
#define B p4
#define I p5
#define X p6

void
eval_sum(void)
{
	int i, j, k;

	// 1st arg (quoted)

	X = cadr(p1);
	if (!issymbol(X))
		stop("sum: 1st arg?");

	// 2nd arg

	push(caddr(p1));
	eval();
	j = pop_integer();
	if (j == (int) 0x80000000)
		stop("sum: 2nd arg?");

	// 3rd arg

	push(cadddr(p1));
	eval();
	k = pop_integer();
	if (k == (int) 0x80000000)
		stop("sum: 3rd arg?");

	// 4th arg

	p1 = caddddr(p1);

	B = get_binding(X);
	A = get_arglist(X);

	push_integer(0);

	for (i = j; i <= k; i++) {
		push_integer(i);
		I = pop();
		set_binding(X, I);
		push(p1);
		eval();
		add();
	}

	set_binding_and_arglist(X, B, A);
}
