// Cosine function of numerical and symbolic arguments

#include "stdafx.h"
#include "defs.h"

void
eval_cos(void)
{
	push(cadr(p1));
	eval();
	cosine();
}

void
cosine(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(ADD))
		cosine_of_angle_sum();
	else
		cosine_of_angle();
	restore();
}

// Use angle sum formula for special angles.

#define A p3
#define B p4

void
cosine_of_angle_sum(void)
{
	p2 = cdr(p1);
	while (iscons(p2)) {
		B = car(p2);
		if (isnpi(B)) {
			push(p1);
			push(B);
			subtract();
			A = pop();
			push(A);
			cosine();
			push(B);
			cosine();
			multiply();
			push(A);
			sine();
			push(B);
			sine();
			multiply();
			subtract();
			return;
		}
		p2 = cdr(p2);
	}
	cosine_of_angle();
}

void
cosine_of_angle(void)
{
	int n;
	double d;

	if (car(p1) == symbol(ARCCOS)) {
		push(cadr(p1));
		return;
	}

	if (isdouble(p1)) {
		d = cos(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}

	// cosine function is symmetric, cos(-x) = cos(x)

	if (isnegative(p1)) {
		push(p1);
		negate();
		p1 = pop();
	}

	// cos(arctan(x)) = 1 / sqrt(1 + x^2)

	// see p. 173 of the CRC Handbook of Mathematical Sciences

	if (car(p1) == symbol(ARCTAN)) {
		push_integer(1);
		push(cadr(p1));
		push_integer(2);
		power();
		add();
		push_rational(-1, 2);
		power();
		return;
	}

	// multiply by 180/pi

	push(p1);
	push_integer(180);
	multiply();
	push_symbol(PI);
	divide();

	n = pop_integer();

	if (n < 0) {
		push(symbol(COS));
		push(p1);
		list(2);
		return;
	}

	switch (n % 360) {
	case 90:
	case 270:
		push_integer(0);
		break;
	case 60:
	case 300:
		push_rational(1, 2);
		break;
	case 120:
	case 240:
		push_rational(-1, 2);
		break;
	case 45:
	case 315:
		push_rational(1, 2);
		push_integer(2);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 135:
	case 225:
		push_rational(-1, 2);
		push_integer(2);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 30:
	case 330:
		push_rational(1, 2);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 150:
	case 210:
		push_rational(-1, 2);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 0:
		push_integer(1);
		break;
	case 180:
		push_integer(-1);
		break;
	default:
		push(symbol(COS));
		push(p1);
		list(2);
		break;
	}
}

#if SELFTEST

static char *s[] = {

	"cos(x)",
	"cos(x)",

	"cos(-x)",
	"cos(x)",

	"cos(b-a)",
	"cos(a-b)",

	// check against the floating point math library

	"f(a,x)=1+cos(float(a/360*2*pi))-float(x)+cos(a/360*2*pi)-x",
	"",

	"f(0,1)",			// 0
	"1",

	"f(90,0)",			// 90
	"1",

	"f(180,-1)",			// 180
	"1",

	"f(270,0)",			// 270
	"1",

	"f(360,1)",			// 360
	"1",

	"f(-90,0)",			// -90
	"1",

	"f(-180,-1)",			// -180
	"1",

	"f(-270,0)",			// -270
	"1",

	"f(-360,1)",			// -360
	"1",

	"f(45,sqrt(2)/2)",		// 45
	"1",

	"f(135,-sqrt(2)/2)",		// 135
	"1",

	"f(225,-sqrt(2)/2)",		// 225
	"1",

	"f(315,sqrt(2)/2)",		// 315
	"1",

	"f(-45,sqrt(2)/2)",		// -45
	"1",

	"f(-135,-sqrt(2)/2)",		// -135
	"1",

	"f(-225,-sqrt(2)/2)",		// -225
	"1",

	"f(-315,sqrt(2)/2)",		// -315
	"1",

	"f(30,sqrt(3)/2)",		// 30
	"1",

	"f(150,-sqrt(3)/2)",		// 150
	"1",

	"f(210,-sqrt(3)/2)",		// 210
	"1",

	"f(330,sqrt(3)/2)",		// 330
	"1",

	"f(-30,sqrt(3)/2)",		// -30
	"1",

	"f(-150,-sqrt(3)/2)",		// -150
	"1",

	"f(-210,-sqrt(3)/2)",		// -210
	"1",

	"f(-330,sqrt(3)/2)",		// -330
	"1",

	"f(60,1/2)",			// 60
	"1",

	"f(120,-1/2)",			// 120
	"1",

	"f(240,-1/2)",			// 240
	"1",

	"f(300,1/2)",			// 300
	"1",

	"f(-60,1/2)",			// -60
	"1",

	"f(-120,-1/2)",			// -120
	"1",

	"f(-240,-1/2)",			// -240
	"1",

	"f(-300,1/2)",			// -300
	"1",

	"f=quote(f)",
	"",

	"cos(arccos(x))",
	"x",

	// bug fix for version 119

	"cos(1/12*pi)",
	"cos(1/12*pi)",

	"cos(arctan(4/3))",
	"3/5",

	"cos(-arctan(4/3))",
	"3/5",

	// phase

	"cos(x-8/2*pi)",
	"cos(x)",

	"cos(x-7/2*pi)",
	"-sin(x)",

	"cos(x-6/2*pi)",
	"-cos(x)",

	"cos(x-5/2*pi)",
	"sin(x)",

	"cos(x-4/2*pi)",
	"cos(x)",

	"cos(x-3/2*pi)",
	"-sin(x)",

	"cos(x-2/2*pi)",
	"-cos(x)",

	"cos(x-1/2*pi)",
	"sin(x)",

	"cos(x+0/2*pi)",
	"cos(x)",

	"cos(x+1/2*pi)",
	"-sin(x)",

	"cos(x+2/2*pi)",
	"-cos(x)",

	"cos(x+3/2*pi)",
	"sin(x)",

	"cos(x+4/2*pi)",
	"cos(x)",

	"cos(x+5/2*pi)",
	"-sin(x)",

	"cos(x+6/2*pi)",
	"-cos(x)",

	"cos(x+7/2*pi)",
	"sin(x)",

	"cos(x+8/2*pi)",
	"cos(x)",
};

void
test_cos(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
