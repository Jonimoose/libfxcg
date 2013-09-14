#include "stdafx.h"

#include "defs.h"

//	q(x)y' + p(x)*y = g(x)
//
//	u(x) = exp(integral(p))
//
//	y = (integral(u*g) + c) / u(x)


#define f p1
#define y p2
#define x p3

#define p p4
#define g p5
#define q p6

#define mu p7

void
dsolve(void)
{
	int n;

	save();

	x = pop();
	y = pop();
	f = pop();

	push(f);
	push(y);
	push(x);

	n = distilly();

	if (n != 3)
		stop("error in dsolve");

	q=pop();

	p = pop();

	negate();
	g = pop();

/*	print(g);
	print(p);
	print(p);
*/
	push(p);
	push(q);
	divide();
	push(x);
	integral();
	exponential();
	mu = pop();

	push(mu);
	push(g);
	push(q);
	divide();
	multiply();
	push(x);
	integral();
	scan("C");
	add();
	push(mu);
	divide();

	restore();
}

//	n	p1		p2	p3	p4	p5	stack

//	1	4y'+3xy+2x+1	y	x	1	2x+1	2x+1

//	2	4y'+3xy		y'	x	y	3xy	3x

//	3	4y'		y''	x	y'	4y'	4

int distilly()
{
	int n = 0;
	save();
	p4 = one;
	p3 = pop();
	p2 = pop();
	p1 = pop();
	while (1) {
		n++;
		push(p1);
		push(p2);
		push(zero);
		subst();
		eval();
		p5 = pop();
		push(p5);
		push(p4);
		divide();
		push(p1);
		push(p5);
		subtract();
		p1 = pop();
		if (equal(p1, zero))
			break;
		p4 = p2;
		push(p2);
		push(p3);
		derivative();
		p2 = pop();
	}
	restore();
	return n;
}
