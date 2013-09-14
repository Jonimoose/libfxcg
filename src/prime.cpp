//-----------------------------------------------------------------------------
//
//	Look up the nth prime
//
//	Input:		n on stack (0 < n < 10001)
//
//	Output:		nth prime on stack
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

void
eval_prime(void)
{
	push(cadr(p1));
	eval();
	prime();
}

void
prime(void)
{
	int n;
	n = pop_integer();
	if (n < 1 || n > MAXPRIMETAB)
		stop("prime: Argument out of range.");
	n = primetab[n - 1];
	push_integer(n);
}
