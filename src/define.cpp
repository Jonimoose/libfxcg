// Store a function definition
//
// Example:
//
//      f(x,y)=x^y
//
// For this definition, p1 points to the following structure.
//
//     p1
//      |
//   ___v__    ______                        ______ 
//  |CONS  |->|CONS  |--------------------->|CONS  |
//  |______|  |______|                      |______|
//      |         |                             |
//   ___v__    ___v__    ______    ______    ___v__    ______    ______
//  |SETQ  |  |CONS  |->|CONS  |->|CONS  |  |CONS  |->|CONS  |->|CONS  |
//  |______|  |______|  |______|  |______|  |______|  |______|  |______|
//                |         |         |         |         |         |
//             ___v__    ___v__    ___v__    ___v__    ___v__    ___v__
//            |SYM f |  |SYM x |  |SYM y |  |POWER |  |SYM x |  |SYM y |
//            |______|  |______|  |______|  |______|  |______|  |______|
//
// We have
//
//	caadr(p1) points to f
//	cdadr(p1) points to the list (x y)
//	caddr(p1) points to (power x y)

#include "stdafx.h"
#include "defs.h"

#define F p3 // F points to the function name
#define A p4 // A points to the argument list
#define B p5 // B points to the function body

void
define_user_function(void)
{
	F = caadr(p1);
	A = cdadr(p1);
	B = caddr(p1);

	if (!issymbol(F))
		stop("function name?");

	// evaluate function body (maybe)

	if (car(B) == symbol(EVAL)) {
		push(cadr(B));
		eval();
		B = pop();
	}

	set_binding_and_arglist(F, B, A);

	// return value is nil

	push_symbol(NIL);
}
