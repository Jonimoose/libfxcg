#include "stdafx.h"
#include "defs.h"

#define F p3
#define X p4
#define N p5

void
eval_integral(void)
{
	int i, n;

	// evaluate 1st arg to get function F

	p1 = cdr(p1);
	push(car(p1));
	eval();

	// evaluate 2nd arg and then...

	// example		result of 2nd arg	what to do
	//
	// integral(f)		nil			guess X, N = nil
	// integral(f,2)	2			guess X, N = 2
	// integral(f,x)	x			X = x, N = nil
	// integral(f,x,2)	x			X = x, N = 2
	// integral(f,x,y)	x			X = x, N = y

	p1 = cdr(p1);
	push(car(p1));
	eval();

	p2 = pop();
	if (p2 == symbol(NIL)) {
		guess();
		push(symbol(NIL));
	} else if (isnum(p2)) {
		guess();
		push(p2);
	} else {
		push(p2);
		p1 = cdr(p1);
		push(car(p1));
		eval();
	}

	N = pop();
	X = pop();
	F = pop();

	while (1) {

		// N might be a symbol instead of a number

		if (isnum(N)) {
			push(N);
			n = pop_integer();
			if (n == (int) 0x80000000)
				stop("nth integral: check n");
		} else
			n = 1;

		push(F);

		if (n >= 0) {
			for (i = 0; i < n; i++) {
				push(X);
				integral();
			}
		} else {
			n = -n;
			for (i = 0; i < n; i++) {
				push(X);
				derivative();
			}
		}

		F = pop();

		// if N is nil then arglist is exhausted

		if (N == symbol(NIL))
			break;

		// otherwise...

		// N		arg1		what to do
		//
		// number	nil		break
		// number	number		N = arg1, continue
		// number	symbol		X = arg1, N = arg2, continue
		//
		// symbol	nil		X = N, N = nil, continue
		// symbol	number		X = N, N = arg1, continue
		// symbol	symbol		X = N, N = arg1, continue

		if (isnum(N)) {
			p1 = cdr(p1);
			push(car(p1));
			eval();
			N = pop();
			if (N == symbol(NIL))
				break;		// arglist exhausted
			if (isnum(N))
				;		// N = arg1
			else {
				X = N;		// X = arg1
				p1 = cdr(p1);
				push(car(p1));
				eval();
				N = pop();	// N = arg2
			}
		} else {
			X = N;			// X = N
			p1 = cdr(p1);
			push(car(p1));
			eval();
			N = pop();		// N = arg1
		}
	}

	push(F);	// final result
}

void
integral(void)
{
	save();
	p2 = pop();
	p1 = pop();
	if (car(p1) == symbol(ADD))
		integral_of_sum();
	else if (car(p1) == symbol(MULTIPLY))
		integral_of_product();
	else
		integral_of_form();
	p1 = pop();
	if (find(p1, symbol(INTEGRAL)))
		stop("integral: sorry, could not find a solution");
	push(p1);
	simplify();	// polish the result
	eval();		// normalize the result
	restore();
}

void
integral_of_sum(void)
{
	p1 = cdr(p1);
	push(car(p1));
	push(p2);
	integral();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		integral();
		add();
		p1 = cdr(p1);
	}
}

void
integral_of_product(void)
{
	push(p1);
	push(p2);
	partition();
	p1 = pop();			// pop variable part
	integral_of_form();
	multiply();			// multiply constant part
}

extern char *itab[];

void
integral_of_form(void)
{
	push(p1);
	push(p2);
	transform(itab);
	p3 = pop();
	if (p3 == symbol(NIL)) {
		push_symbol(INTEGRAL);
		push(p1);
		push(p2);
		list(3);
	} else
		push(p3);
}
