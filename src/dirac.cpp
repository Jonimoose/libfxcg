//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	Dirac function dirac(x)
//	dirac(-x)=dirac(x)
//  dirac(b-a)=dirac(a-b)
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void ydirac(void);

void
eval_dirac(void)
{
	push(cadr(p1));
	eval();
	dirac();
}

void
dirac(void)
{
	save();
	ydirac();
	restore();
}

#define X p1

static void
ydirac(void)
{

	X = pop();


	
	if (isdouble(X)) {
		if (X->u.d == 0) 
			{push_integer(1);
			 return;}
		else 
			{push_integer(0);
			 return;}
	}

	if (isrational(X)) {
		if (MZERO(mmul(X->u.q.a,X->u.q.b))) 
			{push_integer(1);
			 return;}
		else 
			{push_integer(0);
			 return;}
		
	}
	
	if (car(X) == symbol(POWER)) {
		push_symbol(DIRAC);
		push(cadr(X));
		list(2);
		return;
	}
	
	if (isnegativeterm(X)) {
		push_symbol(DIRAC);
		push(X);
		negate();
		list(2);
		return;
	}
	
	if (isnegativeterm(p1) || (car(p1) == symbol(ADD) && isnegativeterm(cadr(p1)))) {
		push(p1);
		negate();
		p1 = pop();
	}
	
		 
	push_symbol(DIRAC);
	push(X);
	list(2);
}

#if SELFTEST

static char *s[] = {

	
	"dirac(-x)",
	"dirac(x)",
};

void
test_dirac(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
