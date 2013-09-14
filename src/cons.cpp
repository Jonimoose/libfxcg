// Cons two things on the stack.

#include "stdafx.h"
#include "defs.h"

void
cons(void)
{
	// auto var ok, no opportunity for garbage collection after p = alloc()
	U *p;
	p = alloc();
	p->k = CONS;
	p->u.cons.cdr = pop();
	p->u.cons.car = pop();
	push(p);
}
