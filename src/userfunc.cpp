// Evaluate a user defined function

#include "stdafx.h"
#include "defs.h"

#define F p3 // F is the function body
#define A p4 // A is the formal argument list
#define B p5 // B is the calling argument list
#define S p6 // S is the argument substitution list

void
eval_user_function(void)
{
	int h;

	// Use "derivative" instead of "d" if there is no user function "d"

	if (car(p1) == symbol(SYMBOL_D) && get_arglist(symbol(SYMBOL_D)) == symbol(NIL)) {
		eval_derivative();
		return;
	}

	F = get_binding(car(p1));
	A = get_arglist(car(p1));
	B = cdr(p1);

	// Undefined function?

	if (F == car(p1)) {
		h = tos;
		push(F);
		p1 = B;
		while (iscons(p1)) {
			push(car(p1));
			eval();
			p1 = cdr(p1);
		}
		list(tos - h);
		return;
	}

	// Create the argument substitution list S

	p1 = A;
	p2 = B;
	h = tos;
	while (iscons(p1) && iscons(p2)) {
		push(car(p1));
		push(car(p2));
		eval();
		p1 = cdr(p1);
		p2 = cdr(p2);
	}
	list(tos - h);
	S = pop();

	// Evaluate the function body

	push(F);
	if (iscons(S)) {
		push(S);
		rewrite_args();
	}
	eval();
}

// Rewrite by expanding symbols that contain args

int
rewrite_args(void)
{
	int h, n = 0;
	save();

	p2 = pop(); // subst. list
	p1 = pop(); // expr

	if (istensor(p1)) {
		n = rewrite_args_tensor();
		restore();
		return n;
	}

	if (iscons(p1)) {
		h = tos;
		push(car(p1)); // Do not rewrite function name
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			n += rewrite_args();
			p1 = cdr(p1);
		}
		list(tos - h);
		restore();
		return n;
	}

	// If not a symbol then done

	if (!issymbol(p1)) {
		push(p1);
		restore();
		return 0;
	}

	// Try for an argument substitution first

	p3 = p2;
	while (iscons(p3)) {
		if (p1 == car(p3)) {
			push(cadr(p3));
			restore();
			return 1;
		}
		p3 = cddr(p3);
	}

	// Get the symbol's binding, try again

	p3 = get_binding(p1);
	push(p3);
	if (p1 != p3) {
		push(p2); // subst. list
		n = rewrite_args();
		if (n == 0) {
			pop();
			push(p1); // restore if not rewritten with arg
		}
	}

	restore();
	return n;
}

int
rewrite_args_tensor(void)
{
	int i, n = 0;
	push(p1);
	copy_tensor();
	p1 = pop();
	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(p1->u.tensor->elem[i]);
		push(p2);
		n += rewrite_args();
		p1->u.tensor->elem[i] = pop();
	}
	push(p1);
	return n;
}
