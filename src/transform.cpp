/* Transform an expression using table look-up

The expression and free variable are on the stack.

The argument s is a null terminated list of transform rules.

For example, see itab.cpp

Internally, the following symbols are used:

	F	input expression

	X	free variable, i.e. F of X

	A	template expression

	B	result expression

	C	list of conditional expressions
*/

#include "stdafx.h"
#include "defs.h"

// p1 and p2 are tmps

#define F p3
#define X p4
#define A p5
#define B p6
#define C p7

void
transform(char **s)
{
	int h;

	save();

	X = pop();
	F = pop();

	// save symbol context in case eval(B) below calls transform

	push(get_binding(symbol(METAA)));
	push(get_binding(symbol(METAB)));
	push(get_binding(symbol(METAX)));

	set_binding(symbol(METAX), X);

	// put constants in F(X) on the stack

	h = tos;
	push_integer(1);
	push(F);
	push(X);
	polyform(); // collect coefficients of x, x^2, etc.
	push(X);
	decomp();

	while (*s) {

		scan_meta(*s);
		p1 = pop();

		A = cadr(p1);
		B = caddr(p1);
		C = cdddr(p1);

		if (f_equals_a(h))
			break;

		s++;
	}

	tos = h;

	if (*s) {
		push(B);
		eval();
		p1 = pop();
	} else
		p1 = symbol(NIL);

	set_binding(symbol(METAX), pop());
	set_binding(symbol(METAB), pop());
	set_binding(symbol(METAA), pop());

	push(p1);

	restore();
}

// search for a METAA and METAB such that F = A

int
f_equals_a(int h)
{
	int i, j;
	for (i = h; i < tos; i++) {
		set_binding(symbol(METAA), stack[i]);
		for (j = h; j < tos; j++) {
			set_binding(symbol(METAB), stack[j]);
			p1 = C;				// are conditions ok?
			while (iscons(p1)) {
				push(car(p1));
				eval();
				p2 = pop();
				if (iszero(p2))
					break;
				p1 = cdr(p1);
			}
			if (iscons(p1))			// no, try next j
				continue;
			push(F);			// F = A?
			push(A);
			eval();
			subtract();
			p1 = pop();
			if (iszero(p1))
				return 1;		// yes
		}
	}
	return 0;					// no
}
