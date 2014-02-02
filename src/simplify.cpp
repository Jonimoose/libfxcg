#include "stdafx.h"
#include "defs.h"

extern int trigmode;
static void simplify_tensor(void);
static int count(U *);
static int nterms(U *);
static void f1(void);
static void f2(void);
static void f3(void);
static void f4(void);
static void f5(void);
static void f9(void);

void
eval_simplify(void)
{
	push(cadr(p1));
	eval();
	simplify();
}

void
simplify(void)
{
	save();
	simplify_main();
	restore();
}

void
simplify_main(void)
{
	p1 = pop();

	if (istensor(p1)) {
		simplify_tensor();
		return;
	}

	if (find(p1, symbol(FACTORIAL))) {
		push(p1);
		simfac();
		p2 = pop();
		push(p1);
		rationalize();
		simfac();
		p3 = pop();
		if (count(p2) < count(p3))
			p1 = p2;
		else
			p1 = p3;
	}

	f1();
	f2();
	f3();
	f4();
	f5();
	f9();

	push(p1);
}

static void
simplify_tensor(void)
{
	int i;
	p2 = alloc_tensor(p1->u.tensor->nelem);
	p2->u.tensor->ndim = p1->u.tensor->ndim;
	for (i = 0; i < p1->u.tensor->ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];
	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(p1->u.tensor->elem[i]);
		simplify();
		p2->u.tensor->elem[i] = pop();
	}
	if (iszero(p2))
		p2 = zero; // null tensor becomes scalar zero
	push(p2);
}

static int
count(U *p)
{
	int n;
	if (iscons(p)) {
		n = 0;
		while (iscons(p)) {
			n += count(car(p)) + 1;
			p = cdr(p);
		}
	} else
		n = 1;
	return n;
}

// try rationalizing

static void
f1(void)
{
	if (car(p1) != symbol(ADD))
		return;
	push(p1);
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

// try condensing

static void
f2(void)
{
	if (car(p1) != symbol(ADD))
		return;
	push(p1);
	Condense();
	p2 = pop();
	if (count(p2) <= count(p1))
		p1 = p2;
}

// this simplifies forms like (A-B) / (B-A)

static void
f3(void)
{
	push(p1);
	rationalize();
	negate();
	rationalize();
	negate();
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

// try expanding denominators

static void
f4(void)
{
	if (iszero(p1))
		return;
	push(p1);
	rationalize();
	inverse();
	rationalize();
	inverse();
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

// simplifies trig forms

void
simplify_trig(void)
{
	save();
	p1 = pop();
	f5();
	push(p1);
	restore();
}

static void
f5(void)
{
	if (find(p1, symbol(SIN)) == 0 && find(p1, symbol(COS)) == 0)
		return;

	p2 = p1;

	trigmode = 1;
	push(p2);
	eval();
	p3 = pop();

	trigmode = 2;
	push(p2);
	eval();
	p4 = pop();

	trigmode = 0;

	if (count(p4) < count(p3) || nterms(p4) < nterms(p3))
		p3 = p4;

	if (count(p3) < count(p1) || nterms(p3) < nterms(p1))
		p1 = p3;
}

// if it's a sum then try to simplify each term

static void
f9(void)
{
	if (car(p1) != symbol(ADD))
		return;
	push_integer(0);
	p2 = cdr(p1);
	while (iscons(p2)) {
		push(car(p2));
		simplify();
		add();
		p2 = cdr(p2);
	}
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

static int
nterms(U *p)
{
	if (car(p) != symbol(ADD))
		return 1;
	else
		return length(p) - 1;
}

#if SELFTEST

static char *s[] = {

	"simplify(A)",
	"A",

	"simplify(A+B)",
	"A+B",

	"simplify(A B)",
	"A*B",

	"simplify(A^B)",
	"A^B",

	"simplify(A/(A+B)+B/(A+B))",
	"1",

	"simplify((A-B)/(B-A))",
	"-1",

	"A=((A11,A12),(A21,A22))",
	"",

	"simplify(det(A) inv(A) - adj(A))",
	"0",

	"A=quote(A)",
	"",

	// this shows need for <= in try_factoring

//	"x*(1+a)",
//	"x+a*x",

//	"simplify(last)",
//	"x*(1+a)",

	"simplify(-3 exp(-1/3 r + i phi) cos(theta) / sin(theta)"
	" + 3 exp(-1/3 r + i phi) cos(theta) sin(theta)"
	" + 3 exp(-1/3 r + i phi) cos(theta)^3 / sin(theta))",
	"0",

	"simplify((A^2 C^2 + A^2 D^2 + B^2 C^2 + B^2 D^2)/(A^2+B^2)/(C^2+D^2))",
	"1",

	"simplify(d(arctan(y/x),y))",
	"x/(x^2+y^2)",

	"simplify(d(arctan(y/x),x))",
	"-y/(x^2+y^2)",

	"simplify(1-sin(x)^2)",
	"cos(x)^2",

	"simplify(1-cos(x)^2)",
	"sin(x)^2",

	"simplify(sin(x)^2-1)",
	"-cos(x)^2",

	"simplify(cos(x)^2-1)",
	"-sin(x)^2",
/*
	"simfac(n!/n)-(n-1)!",
	"0",

	"simfac(n/n!)-1/(n-1)!",
	"0",

	"simfac(rationalize((n+k+1)/(n+k+1)!))-1/(n+k)!",
	"0",

	"simfac(condense((n+1)*n!))-(n+1)!",
	"0",

	"simfac(1/((n+1)*n!))-1/(n+1)!",
	"0",

	"simfac((n+1)!/n!)-n-1",
	"0",

	"simfac(n!/(n+1)!)-1/(n+1)",
	"0",

	"simfac(binomial(n+1,k)/binomial(n,k))",
	"(1+n)/(1-k+n)",

	"simfac(binomial(n,k)/binomial(n+1,k))",
	"(1-k+n)/(1+n)",

	"F(nn,kk)=kk*binomial(nn,kk)",
	"",

	"simplify(simfac((F(n,k)+F(n,k-1))/F(n+1,k))-n/(n+1))",
	"0",

	"F=quote(F)",
	"",
*/
	"simplify(n!/n)-(n-1)!",
	"0",

	"simplify(n/n!)-1/(n-1)!",
	"0",

	"simplify(rationalize((n+k+1)/(n+k+1)!))-1/(n+k)!",
	"0",

	"simplify(condense((n+1)*n!))-(n+1)!",
	"0",

	"simplify(1/((n+1)*n!))-1/(n+1)!",
	"0",

	"simplify((n+1)!/n!)-n-1",
	"0",

	"simplify(n!/(n+1)!)-1/(n+1)",
	"0",

	"simplify(binomial(n+1,k)/binomial(n,k))",
	"(1+n)/(1-k+n)",

	"simplify(binomial(n,k)/binomial(n+1,k))",
	"(1-k+n)/(1+n)",

	"F(nn,kk)=kk*binomial(nn,kk)",
	"",

	"simplify((F(n,k)+F(n,k-1))/F(n+1,k))-n/(n+1)",
	"0",

	"F=quote(F)",
	"",

	"simplify((n+1)/(n+1)!)-1/n!",
	"0",

	"simplify(a*b+a*c)",
	"a*(b+c)",

// Symbol's binding is evaluated, undoing simplify

	"x=simplify(a*b+a*c)",
	"",

	"x",
	"a*b+a*c",
};

void
test_simplify(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
