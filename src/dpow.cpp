// power function for double precision floating point

#include "stdafx.h"
#include "defs.h"

void
dpow(void)
{
	double a, b, base, expo, result, theta;

	expo = pop_double();
	base = pop_double();

	// divide by zero?

	if (base == 0.0 && expo < 0.0)
		stop("divide by zero");

	// nonnegative base or integer power?

	if (base >= 0.0 || fmod(expo, 1.0) == 0.0) {
		result = pow(base, expo);
		push_double(result);
		return;
	}

	result = pow(fabs(base), expo);

	theta = M_PI * expo;

	// this ensures the real part is 0.0 instead of a tiny fraction

	if (fmod(expo, 0.5) == 0.0) {
		a = 0.0;
		b = sin(theta);
	} else {
		a = cos(theta);
		b = sin(theta);
	}

	push_double(a * result);
	push_double(b * result);
	push(imaginaryunit);
	multiply();
	add();
}
