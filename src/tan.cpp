// Tangent function of numerical and symbolic arguments

#include "stdafx.h"
#include "defs.h"

void
eval_tan(void)
{
	push(cadr(p1));
	eval();
	tangent();
}

void
tangent(void)
{
	save();
	yytangent();
	restore();
}

void
yytangent(void)
{
	int n;
	double d;

	p1 = pop();

	if (car(p1) == symbol(ARCTAN)) {
		push(cadr(p1));
		return;
	}

	if (isdouble(p1)) {
		d = tan(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}

	// tan function is antisymmetric, tan(-x) = -tan(x)

	if (isnegative(p1)) {
		push(p1);
		negate();
		tangent();
		negate();
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
		push(symbol(TAN));
		push(p1);
		list(2);
		return;
	}

	switch (n % 360) {
	case 0:
	case 180:
		push_integer(0);
		break;
	case 30:
	case 210:
		push_rational(1, 3);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 150:
	case 330:
		push_rational(-1, 3);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 45:
	case 225:
		push_integer(1);
		break;
	case 135:
	case 315:
		push_integer(-1);
		break;
	case 60:
	case 240:
		push_integer(3);
		push_rational(1, 2);
		power();
		break;
	case 120:
	case 300:
		push_integer(3);
		push_rational(1, 2);
		power();
		negate();
		break;
	default:
		push(symbol(TAN));
		push(p1);
		list(2);
		break;
	}
}

#if SELFTEST

static char *s[] = {

	"tan(x)",
	"tan(x)",

	"tan(-x)",
	"-tan(x)",

	"tan(b-a)",
	"-tan(a-b)",

	// check against the floating point math library

	"f(a,x)=1+tan(float(a/360*2*pi))-float(x)+tan(a/360*2*pi)-x",
	"",

	"f(0,0)",			// 0
	"1",

	"f(180,0)",			// 180
	"1",

	"f(360,0)",			// 360
	"1",

	"f(-180,0)",			// -180
	"1",

	"f(-360,0)",			// -360
	"1",

	"f(45,1)",			// 45
	"1",

	"f(135,-1)",			// 135
	"1",

	"f(225,1)",			// 225
	"1",

	"f(315,-1)",			// 315
	"1",

	"f(-45,-1)",			// -45
	"1",

	"f(-135,1)",			// -135
	"1",

	"f(-225,-1)",			// -225
	"1",

	"f(-315,1)",			// -315
	"1",

	"f(30,sqrt(3)/3)",		// 30
	"1",

	"f(150,-sqrt(3)/3)",		// 150
	"1",

	"f(210,sqrt(3)/3)",		// 210
	"1",

	"f(330,-sqrt(3)/3)",		// 330
	"1",

	"f(-30,-sqrt(3)/3)",		// -30
	"1",

	"f(-150,sqrt(3)/3)",		// -150
	"1",

	"f(-210,-sqrt(3)/3)",		// -210
	"1",

	"f(-330,sqrt(3)/3)",		// -330
	"1",

	"f(60,sqrt(3))",		// 60
	"1",

	"f(120,-sqrt(3))",		// 120
	"1",

	"f(240,sqrt(3))",		// 240
	"1",

	"f(300,-sqrt(3))",		// 300
	"1",

	"f(-60,-sqrt(3))",		// -60
	"1",

	"f(-120,sqrt(3))",		// -120
	"1",

	"f(-240,-sqrt(3))",		// -240
	"1",

	"f(-300,sqrt(3))",		// -300
	"1",

	"f=quote(f)",
	"",

	"tan(arctan(x))",
	"x",

	// check the default case

	"tan(1/12*pi)",
	"tan(1/12*pi)",
};

void
test_tan(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
