#include "stdafx.h"
#include "defs.h"

void
init(void)
{
	int i;
	static int flag;

	tos = 0;
	esc_flag = 0;
	draw_flag = 0;
	frame = stack + TOS;

	p0 = symbol(NIL);
	p1 = symbol(NIL);
	p2 = symbol(NIL);
	p3 = symbol(NIL);
	p4 = symbol(NIL);
	p5 = symbol(NIL);
	p6 = symbol(NIL);
	p7 = symbol(NIL);
	p8 = symbol(NIL);
	p9 = symbol(NIL);

	if (flag)
		return;		// already initted

	flag = 1;

	for (i = 0; i < NSYM; i++) {
		symtab[i].k = SYM;
		binding[i] = symtab + i;
		arglist[i] = symbol(NIL);
	}

	std_symbol("abs", ABS);
	std_symbol("add", ADD);
	std_symbol("adj", ADJ);
	std_symbol("and", AND);
	std_symbol("arccos", ARCCOS);
	std_symbol("arccosh", ARCCOSH);
	std_symbol("arcsin", ARCSIN);
	std_symbol("arcsinh", ARCSINH);
	std_symbol("arctan", ARCTAN);
	std_symbol("arctanh", ARCTANH);
	std_symbol("arg", ARG);
	std_symbol("atomize", ATOMIZE);
	std_symbol("besselj", BESSELJ);
	std_symbol("bessely", BESSELY);
	std_symbol("binding", BINDING);
	std_symbol("binomial", BINOMIAL);
	std_symbol("ceiling", CEILING);
	std_symbol("check", CHECK);
	std_symbol("choose", CHOOSE);
	std_symbol("circexp", CIRCEXP);
	std_symbol("clear", CLEAR);
	std_symbol("clock", CLOCK);
	std_symbol("coeff", COEFF);
	std_symbol("cofactor", COFACTOR);
	std_symbol("condense", CONDENSE);
	std_symbol("conj", CONJ);
	std_symbol("contract", CONTRACT);
	std_symbol("cos", COS);
	std_symbol("cosh", COSH);
	std_symbol("decomp", DECOMP);
	std_symbol("defint", DEFINT);
	std_symbol("deg", DEGREE);
	std_symbol("denominator", DENOMINATOR);
	std_symbol("det", DET);
	std_symbol("derivative", DERIVATIVE);
	std_symbol("dim", DIM);
	std_symbol("dirac", DIRAC);
	std_symbol("display", DISPLAY);
	std_symbol("divisors", DIVISORS);
	std_symbol("do", DO);
	std_symbol("dot", DOT);
	std_symbol("draw", DRAW);
	std_symbol("dsolve", DSOLVE);
	std_symbol("erf", ERF);
	std_symbol("erfc", ERFC);
	std_symbol("eigen", EIGEN);
	std_symbol("eigenval", EIGENVAL);
	std_symbol("eigenvec", EIGENVEC);
	std_symbol("eval", EVAL);
	std_symbol("exp", EXP);
	std_symbol("expand", EXPAND);
	std_symbol("expcos", EXPCOS);
	std_symbol("expsin", EXPSIN);
	std_symbol("factor", FACTOR);
	std_symbol("factorial", FACTORIAL);
	std_symbol("factorpoly", FACTORPOLY);
	std_symbol("filter", FILTER);
	std_symbol("float", FLOATF);
	std_symbol("floor", FLOOR);
	std_symbol("for", FOR);
	std_symbol("Gamma", GAMMA);
	std_symbol("gcd", GCD);
	std_symbol("hermite", HERMITE);
	std_symbol("hilbert", HILBERT);
	std_symbol("imag", IMAG);
	std_symbol("component", INDEX);
	std_symbol("inner", INNER);
	std_symbol("integral", INTEGRAL);
	std_symbol("inv", INV);
	std_symbol("invg", INVG);
	std_symbol("isinteger", ISINTEGER);
	std_symbol("isprime", ISPRIME);
	std_symbol("laguerre", LAGUERRE);
//	std_symbol("laplace", LAPLACE);
	std_symbol("lcm", LCM);
	std_symbol("leading", LEADING);
	std_symbol("legendre", LEGENDRE);
	std_symbol("log", LOG);
	std_symbol("mag", MAG);
	std_symbol("mod", MOD);
	std_symbol("multiply", MULTIPLY);
	std_symbol("not", NOT);
	std_symbol("nroots", NROOTS);
	std_symbol("number", NUMBER);
	std_symbol("numerator", NUMERATOR);
	std_symbol("operator", OPERATOR);
	std_symbol("or", OR);
	std_symbol("outer", OUTER);
	std_symbol("polar", POLAR);
	std_symbol("power", POWER);
	std_symbol("prime", PRIME);
	std_symbol("print", PRINT);
	std_symbol("product", PRODUCT);
	std_symbol("quote", QUOTE);
	std_symbol("quotient", QUOTIENT);
        std_symbol("random", RANDOM);
	std_symbol("rank", RANK);
	std_symbol("rationalize", RATIONALIZE);
	std_symbol("real", REAL);
	std_symbol("rect", YYRECT);
	std_symbol("roots", ROOTS);
	std_symbol("equals", SETQ);
	std_symbol("sgn", SGN);
	std_symbol("simplify", SIMPLIFY);
	std_symbol("sin", SIN);
	std_symbol("sinh", SINH);
	std_symbol("sqrt", SQRT);
	std_symbol("stop", STOP);
	std_symbol("subst", SUBST);
	std_symbol("sum", SUM);
	std_symbol("tan", TAN);
	std_symbol("tanh", TANH);
	std_symbol("taylor", TAYLOR);
	std_symbol("test", TEST);
	std_symbol("testeq", TESTEQ);
	std_symbol("testge", TESTGE);
	std_symbol("testgt", TESTGT);
	std_symbol("testle", TESTLE);
	std_symbol("testlt", TESTLT);
	std_symbol("transpose", TRANSPOSE);
	std_symbol("unit", UNIT);
	std_symbol("zero", ZERO);

	std_symbol("nil", NIL);

	// each symbol needs a unique name because equal() compares printnames

	std_symbol("autoexpand", AUTOEXPAND);
	std_symbol("bake", BAKE);
	std_symbol("last", LAST);
	std_symbol("trace", TRACE);
	std_symbol("tty", TTY);

	std_symbol("~", YYE);	// tilde so sort puts it after other symbols

	std_symbol("$DRAWX", DRAWX);	// special purpose internal symbols
	std_symbol("$METAA", METAA);
	std_symbol("$METAB", METAB);
	std_symbol("$METAX", METAX);
	std_symbol("$SECRETX", SECRETX);

	std_symbol("pi", PI);
	std_symbol("a", SYMBOL_A);
	std_symbol("b", SYMBOL_B);
	std_symbol("c", SYMBOL_C);
	std_symbol("d", SYMBOL_D);
	std_symbol("i", SYMBOL_I);
	std_symbol("j", SYMBOL_J);
	std_symbol("n", SYMBOL_N);
	std_symbol("r", SYMBOL_R);
	std_symbol("s", SYMBOL_S);
	std_symbol("t", SYMBOL_T);
	std_symbol("x", SYMBOL_X);
	std_symbol("y", SYMBOL_Y);
	std_symbol("z", SYMBOL_Z);

	std_symbol("$C1", C1);
	std_symbol("$C2", C2);
	std_symbol("$C3", C3);
	std_symbol("$C4", C4);
	std_symbol("$C5", C5);
	std_symbol("$C6", C6);

	push_integer(0);
	zero = pop();		// must be untagged in gc

	push_integer(1);
	one = pop();		// must be untagged in gc

	push_symbol(POWER);
	push_integer(-1);
	push_rational(1, 2);
	list(3);
	imaginaryunit = pop();	// must be untagged in gc

	defn();
}

static char *defn_str[] = {
	"e=exp(1)",
	"i=sqrt(-1)",
	"autoexpand=1",
	"trange=(-pi,pi)",
	"xrange=(-10,10)",
	"yrange=(-10,10)",
	"last=0",
	"trace=0",
	"tty=0",
	"cross(u,v)=(u[2]*v[3]-u[3]*v[2],u[3]*v[1]-u[1]*v[3],u[1]*v[2]-u[2]*v[1])",
	"curl(v)=(d(v[3],y)-d(v[2],z),d(v[1],z)-d(v[3],x),d(v[2],x)-d(v[1],y))",
	"div(v)=d(v[1],x)+d(v[2],y)+d(v[3],z)",
	"ln(x)=log(x)",
};

void
defn(void)
{
	int i, n;
	n = sizeof defn_str / sizeof (char *);
	for (i = 0; i < n; i++) {
		scan(defn_str[i]);
		eval();
		pop();
	}
}
