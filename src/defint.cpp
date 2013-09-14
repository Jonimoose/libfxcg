// definite integral

#include "stdafx.h"
#include "defs.h"

#define F p2
#define X p3
#define A p4
#define B p5

void
eval_defint(void)
{
	push(cadr(p1));
	eval();
	F = pop();

	p1 = cddr(p1);

	while (iscons(p1)) {

		push(car(p1));
		p1 = cdr(p1);
		eval();
		X = pop();

		push(car(p1));
		p1 = cdr(p1);
		eval();
		A = pop();

		push(car(p1));
		p1 = cdr(p1);
		eval();
		B = pop();

		push(F);
		push(X);
		integral();
		F = pop();

		push(F);
		push(X);
		push(B);
		subst();
		eval();

		push(F);
		push(X);
		push(A);
		subst();
		eval();

		subtract();
		F = pop();
	}

	push(F);
}

#if SELFTEST

static char *s[] = {
	"defint(x^2,y,0,sqrt(1-x^2),x,-1,1)",
	"1/8*pi",

	// from the eigenmath manual

	"z=2",
	"",

	"P=(x,y,z)",
	"",

	"a=abs(cross(d(P,x),d(P,y)))",
	"",

	"defint(a,y,-sqrt(1-x^2),sqrt(1-x^2),x,-1,1)",
	"pi",

	// from the eigenmath manual

	"z=x^2+2y",
	"",

	"P=(x,y,z)",
	"",

	"a=abs(cross(d(P,x),d(P,y)))",
	"",

	"defint(a,x,0,1,y,0,1)",
	"3/2+5/8*log(5)",

	// from the eigenmath manual

	"x=u*cos(v)",
	"",

	"y=u*sin(v)",
	"",

	"z=v",
	"",

	"S=(x,y,z)",
	"",

	"a=abs(cross(d(S,u),d(S,v)))",
	"",

	"defint(a,u,0,1,v,0,3pi)",
	"3/2*pi*log(1+2^(1/2))+3*pi/(2^(1/2))",
};

void
test_defint(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
