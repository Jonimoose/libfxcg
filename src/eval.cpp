// Evaluate an expression, for example...
//
//	push(p1)
//	eval()
//	p2 = pop()

#include "stdafx.h"
#include "defs.h"

void
eval(void)
{
	check_esc_flag();
	save();
	p1 = pop();
	switch (p1->k) {
	case CONS:
		eval_cons();
		break;
	case NUM:
		push(p1);
		break;
	case DOUBLE:
		push(p1);
		break;
	case STR:
		push(p1);
		break;
	case TENSOR:
		eval_tensor();
		break;
	case SYM:
		eval_sym();
		break;
	default:
		stop("atom?");
		break;
	}
	restore();
}

void
eval_sym(void)
{
	// bare keyword?

	if (iskeyword(p1)) {
		push(p1);
		push(symbol(LAST));
		list(2);
		eval();
		return;
	}

	// evaluate symbol's binding

	p2 = get_binding(p1);
	push(p2);
	if (p1 != p2)
		eval();
}

void
eval_cons(void)
{
	if (!issymbol(car(p1)))
		stop("cons?");

	switch (symnum(car(p1))) {
	case ABS:		eval_abs();		break;
	case ADD:		eval_add();		break;
	case ADJ:		eval_adj();		break;
	case AND:		eval_and();		break;
	case ARCCOS:		eval_arccos();		break;
	case ARCCOSH:		eval_arccosh();		break;
	case ARCSIN:		eval_arcsin();		break;
	case ARCSINH:		eval_arcsinh();		break;
	case ARCTAN:		eval_arctan();		break;
	case ARCTANH:		eval_arctanh();		break;
	case ARG:		eval_arg();		break;
	case ATOMIZE:		eval_atomize();		break;
	case BESSELJ:		eval_besselj();		break;
	case BESSELY:		eval_bessely();		break;
	case BINDING:		eval_binding();		break;
	case BINOMIAL:		eval_binomial();	break;
	case CEILING:		eval_ceiling();		break;
	case CHECK:		eval_check();		break;
	case CHOOSE:		eval_choose();		break;
	case CIRCEXP:		eval_circexp();		break;
	case CLEAR:		eval_clear();		break;
	case CLOCK:		eval_clock();		break;
	case COEFF:		eval_coeff();		break;
	case COFACTOR:		eval_cofactor();	break;
	case CONDENSE:		eval_condense();	break;
	case CONJ:		eval_conj();		break;
	case CONTRACT:		eval_contract();	break;
	case COS:		eval_cos();		break;
	case COSH:		eval_cosh();		break;
	case DECOMP:		eval_decomp();		break;
	case DEGREE:		eval_degree();		break;
	case DEFINT:		eval_defint();		break;
	case DENOMINATOR:	eval_denominator();	break;
	case DERIVATIVE:	eval_derivative();	break;
	case DET:		eval_det();		break;
	case DIM:		eval_dim();		break;
	case DIRAC:		eval_dirac();		break;
	case DISPLAY:		eval_display();		break;
	case DIVISORS:		eval_divisors();	break;
	case DO:		eval_do();		break;
	case DOT:		eval_inner();		break;
	case DRAW:		eval_draw();		break;
	case DSOLVE:		eval_dsolve();		break;
	case EIGEN:		eval_eigen();		break;
	case EIGENVAL:		eval_eigenval();	break;
	case EIGENVEC:		eval_eigenvec();	break;
	case ERF:		eval_erf();		break;
	case ERFC:		eval_erfc();		break;
	case EVAL:		eval_eval();		break;
	case EXP:		eval_exp();		break;
	case EXPAND:		eval_expand();		break;
	case EXPCOS:		eval_expcos();		break;
	case EXPSIN:		eval_expsin();		break;
	case FACTOR:		eval_factor();		break;
	case FACTORIAL:		eval_factorial();	break;
	case FACTORPOLY:	eval_factorpoly();	break;
	case FILTER:		eval_filter();		break;
	case FLOATF:		eval_float();		break;
	case FLOOR:		eval_floor();		break;
	case FOR:		eval_for();		break;
	case GAMMA:		eval_gamma();		break;
	case GCD:		eval_gcd();		break;
	case HERMITE:		eval_hermite();		break;
	case HILBERT:		eval_hilbert();		break;
	case IMAG:		eval_imag();		break;
	case INDEX:		eval_index();		break;
	case INNER:		eval_inner();		break;
	case INTEGRAL:		eval_integral();	break;
	case INV:		eval_inv();		break;
	case INVG:		eval_invg();		break;
	case ISINTEGER:		eval_isinteger();	break;
	case ISPRIME:		eval_isprime();		break;
	case LAGUERRE:		eval_laguerre();	break;
//	case LAPLACE:		eval_laplace();		break;
	case LCM:		eval_lcm();		break;
	case LEADING:		eval_leading();		break;
	case LEGENDRE:		eval_legendre();	break;
	case LOG:		eval_log();		break;
	case MAG:		eval_mag();		break;
	case MOD:		eval_mod();		break;
	case MULTIPLY:		eval_multiply();	break;
	case NOT:		eval_not();		break;
	case NROOTS:		eval_nroots();		break;
	case NUMBER:		eval_number();		break;
	case NUMERATOR:		eval_numerator();	break;
	case OPERATOR:		eval_operator();	break;
	case OR:		eval_or();		break;
	case OUTER:		eval_outer();		break;
	case POLAR:		eval_polar();		break;
	case POWER:		eval_power();		break;
	case PRIME:		eval_prime();		break;
	case PRINT:		eval_display();		break;
	case PRODUCT:		eval_product();		break;
	case QUOTE:		eval_quote();		break;
	case QUOTIENT:		eval_quotient();	break;
        case RANDOM:              eval_random();            break; // by gbl08ma
	case RANK:		eval_rank();		break;
	case RATIONALIZE:	eval_rationalize();	break;
	case REAL:		eval_real();		break;
	case YYRECT:		eval_rect();		break;
	case ROOTS:		eval_roots();		break;
	case SETQ:		eval_setq();		break;
	case SGN:		eval_sgn();		break;
	case SIMPLIFY:		eval_simplify();	break;
	case SIN:		eval_sin();		break;
	case SINH:		eval_sinh();		break;
	case SQRT:		eval_sqrt();		break;
	case STOP:		eval_stop();		break;
	case SUBST:		eval_subst();		break;
	case SUM:		eval_sum();		break;
	case TAN:		eval_tan();		break;
	case TANH:		eval_tanh();		break;
	case TAYLOR:		eval_taylor();		break;
	case TEST:		eval_test();		break;
	case TESTEQ:		eval_testeq();		break;
	case TESTGE:		eval_testge();		break;
	case TESTGT:		eval_testgt();		break;
	case TESTLE:		eval_testle();		break;
	case TESTLT:		eval_testlt();		break;
	case TRANSPOSE:		eval_transpose();	break;
	case UNIT:		eval_unit();		break;
	case ZERO:		eval_zero();		break;
	default:		eval_user_function();	break;
	}	
}

void
eval_binding(void)
{
	push(get_binding(cadr(p1)));
}

// checks a predicate, i.e. check(A = B)

void
eval_check(void)
{
	push(cadr(p1));
	eval_predicate();
	p1 = pop();
	if (iszero(p1))
		stop("check(arg): arg is zero");
	push(symbol(NIL)); // no result is printed
}

void
eval_det(void)
{
	push(cadr(p1));
	eval();
	det();
}

void
eval_dim(void)
{
	int n;
	push(cadr(p1));
	eval();
	p2 = pop();
	if (iscons(cddr(p1))) {
		push(caddr(p1));
		eval();
		n = pop_integer();
	} else
		n = 1;
	if (!istensor(p2))
		push_integer(1); // dim of scalar is 1
	else if (n < 1 || n > p2->u.tensor->ndim)
		push(p1);
	else
		push_integer(p2->u.tensor->dim[n - 1]);
}

void
eval_divisors(void)
{
	push(cadr(p1));
	eval();
	divisors();
}

void
eval_do(void)
{
	push(car(p1));
	p1 = cdr(p1);
	while (iscons(p1)) {
		pop();
		push(car(p1));
		eval();
		p1 = cdr(p1);
	}
}

void
eval_dsolve(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	dsolve();
}

// for example, eval(f,x,2)

void
eval_eval(void)
{
	push(cadr(p1));
	eval();
	p1 = cddr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		push(cadr(p1));
		eval();
		subst();
		p1 = cddr(p1);
	}
	eval();
}

void
eval_exp(void)
{
	push(cadr(p1));
	eval();
	exponential();
}

void
eval_factorial(void)
{
	push(cadr(p1));
	eval();
	factorial();
}

void
eval_factorpoly(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	push(car(p1));
	eval();
	factorpoly();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		factorpoly();
		p1 = cdr(p1);
	}
}

void
eval_hermite(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	hermite();
}

void
eval_hilbert(void)
{
	push(cadr(p1));
	eval();
	hilbert();
}

void
eval_index(void)
{
	int h;
	h = tos;
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		p1 = cdr(p1);
	}
	index_function(tos - h);
}

void
eval_inv(void)
{
	push(cadr(p1));
	eval();
	inv();
}

void
eval_invg(void)
{
	push(cadr(p1));
	eval();
	invg();
}

void
eval_isinteger(void)
{
	int n;
	push(cadr(p1));
	eval();
	p1 = pop();
	if (isrational(p1)) {
		if (isinteger(p1))
			push(one);
		else
			push(zero);
		return;
	}
	if (isdouble(p1)) {
		n = (int) p1->u.d;
		if (n == p1->u.d)
			push(one);
		else
			push(zero);
		return;
	}
	push_symbol(ISINTEGER);
	push(p1);
	list(2);
}

void
eval_multiply(void)
{
	push(cadr(p1));
	eval();
	p1 = cddr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		multiply();
		p1 = cdr(p1);
	}
}

void
eval_number(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (p1->k == NUM || p1->k == DOUBLE)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_operator(void)
{
	int h = tos;
	push_symbol(OPERATOR);
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		p1 = cdr(p1);
	}
	list(tos - h);
}

void
eval_print(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		if (equaln(get_binding(symbol(TTY)), 1))
			printline(pop());
		else
			display(pop());
		p1 = cdr(p1);
	}
	push(symbol(NIL));
}

void
eval_quote(void)
{
	push(cadr(p1));
}

void
eval_rank(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (istensor(p1))
		push_integer(p1->u.tensor->ndim);
	else
		push(zero);
}

//-----------------------------------------------------------------------------
//
//	Example: a[1] = b
//
//	p1	*-------*-----------------------*
//		|	|			|
//		setq	*-------*-------*	b
//			|	|	|
//			index	a	1
//
//	cadadr(p1) -> a
//
//-----------------------------------------------------------------------------

void
setq_indexed(void)
{
	int h;
	p4 = cadadr(p1);
	if (!issymbol(p4))
		stop("indexed assignment: error in symbol");
	h = tos;
	push(caddr(p1));
	eval();
	p2 = cdadr(p1);
	while (iscons(p2)) {
		push(car(p2));
		eval();
		p2 = cdr(p2);
	}
	set_component(tos - h);
	p3 = pop();
	set_binding(p4, p3);
	push(symbol(NIL));
}

void
eval_setq(void)
{
	if (caadr(p1) == symbol(INDEX)) {
		setq_indexed();
		return;
	}

	if (iscons(cadr(p1))) {
		define_user_function();
		return;
	}

	if (!issymbol(cadr(p1)))
		stop("symbol assignment: error in symbol");

	push(caddr(p1));
	eval();
	p2 = pop();
	set_binding(cadr(p1), p2);

	push(symbol(NIL));
}

void
eval_sqrt(void)
{
	push(cadr(p1));
	eval();
	push_rational(1, 2);
	power();
}

void
eval_stop(void)
{
	stop("user stop");
}

void
eval_subst(void)
{
	push(cadddr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadr(p1));
	eval();
	subst();
	eval(); // normalize
}

void
eval_unit(void)
{
	int i, n;
	push(cadr(p1));
	eval();
	n = pop_integer();
	if (n < 2) {
		push(p1);
		return;
	}
	p1 = alloc_tensor(n * n);
	p1->u.tensor->ndim = 2;
	p1->u.tensor->dim[0] = n;
	p1->u.tensor->dim[1] = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->elem[n * i + i] = one;
	push(p1);
}

void
eval_noexpand(void)
{
	int x = expanding;
	expanding = 0;
	eval();
	expanding = x;
}

// like eval() except "=" is evaluated as "=="

void
eval_predicate(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(SETQ))
		eval_testeq();
	else {
		push(p1);
		eval();
	}
	restore();
}
