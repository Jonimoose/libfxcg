// tty style printing

#include "stdafx.h"
#include "defs.h"

int out_index, out_length;
char *out_str;
static int char_count, last_char;

char *power_str = "^";

void
print(U *p)
{
	print_expr(p);
}

void
printline(U *p)
{
	print_expr(p);
	print_char('\n');
}

void
print_subexpr(U *p)
{
	print_char('(');
	print_expr(p);
	print_char(')');
}

void
print_expr(U *p)
{
	if (isadd(p)) {
		p = cdr(p);
		if (sign_of_term(car(p)) == '-')
			print_str("-");
		print_term(car(p));
		p = cdr(p);
		while (iscons(p)) {
			if (sign_of_term(car(p)) == '+')
				if (test_flag == 0)
					print_str(" + ");
				else
					print_str("+");
			else
				if (test_flag == 0)
					print_str(" - ");
				else
					print_str("-");
			print_term(car(p));
			p = cdr(p);
		}
	} else {
		if (sign_of_term(p) == '-')
			print_str("-");
		print_term(p);
	}
}

int
sign_of_term(U *p)
{
	if (car(p) == symbol(MULTIPLY) && isnum(cadr(p)) && lessp(cadr(p), zero))
		return '-';
	else if (isnum(p) && lessp(p, zero))
		return '-';
	else
		return '+';
}

#define A p3
#define B p4

void
print_a_over_b(U *p)
{
	int flag, n, d;
	U *p1, *p2;

	save();

	// count numerators and denominators

	n = 0;
	d = 0;

	p1 = cdr(p);
	p2 = car(p1);

	if (isrational(p2)) {
		push(p2);
		mp_numerator();
		absval();
		A = pop();
		push(p2);
		mp_denominator();
		B = pop();
		if (!isplusone(A))
			n++;
		if (!isplusone(B))
			d++;
		p1 = cdr(p1);
	} else {
		A = one;
		B = one;
	}

	while (iscons(p1)) {
		p2 = car(p1);
		if (is_denominator(p2))
			d++;
		else
			n++;
		p1 = cdr(p1);
	}

	if (n == 0)
		print_char('1');
	else {
		flag = 0;
		p1 = cdr(p);
		if (isrational(car(p1)))
			p1 = cdr(p1);
		if (!isplusone(A)) {
			print_factor(A);
			flag = 1;
		} 
		while (iscons(p1)) {
			p2 = car(p1);
			if (is_denominator(p2))
				;
			else {
				if (flag)
					print_multiply_sign();
				print_factor(p2);
				flag = 1;
			}
			p1 = cdr(p1);
		}
	}

	if (test_flag == 0)
		print_str(" / ");
	else
		print_str("/");

	if (d > 1)
		print_char('(');


	flag = 0;
	p1 = cdr(p);

	if (isrational(car(p1)))
		p1 = cdr(p1);

	if (!isplusone(B)) {
		print_factor(B);
		flag = 1;
	}

	while (iscons(p1)) {
		p2 = car(p1);
		if (is_denominator(p2)) {
			if (flag)
				print_multiply_sign();
			print_denom(p2, d);
			flag = 1;
		}
		p1 = cdr(p1);
	}

	if (d > 1)
		print_char(')');

	restore();
}

void
print_term(U *p)
{
	if (car(p) == symbol(MULTIPLY) && any_denominators(p)) {
		print_a_over_b(p);
		return;
	}

	if (car(p) == symbol(MULTIPLY)) {
		p = cdr(p);

		// coeff -1?

		if (isminusone(car(p))) {
//			print_char('-');
			p = cdr(p);
		}

		print_factor(car(p));
		p = cdr(p);
		while (iscons(p)) {
			print_multiply_sign();
			print_factor(car(p));
			p = cdr(p);
		}
	} else
		print_factor(p);
}

// prints stuff after the divide symbol "/"

// d is the number of denominators

#define BASE p1
#define EXPO p2

void
print_denom(U *p, int d)
{
	save();

	BASE = cadr(p);
	EXPO = caddr(p);

	// i.e. 1 / (2^(1/3))

	if (d == 1 && !isminusone(EXPO))
		print_char('(');

	if (isfraction(BASE) || car(BASE) == symbol(ADD) || car(BASE) == symbol(MULTIPLY) || car(BASE) == symbol(POWER) || lessp(BASE, zero)) {
			print_char('(');
			print_expr(BASE);
			print_char(')');
	} else
		print_expr(BASE);

	if (isminusone(EXPO)) {
		restore();
		return;
	}

	if (test_flag == 0)
		print_str(power_str);
	else
		print_char('^');

	push(EXPO);
	negate();
	EXPO = pop();

	if (isfraction(EXPO) || car(EXPO) == symbol(ADD) || car(EXPO) == symbol(MULTIPLY) || car(EXPO) == symbol(POWER)) {
		print_char('(');
		print_expr(EXPO);
		print_char(')');
	} else
		print_expr(EXPO);

	if (d == 1)
		print_char(')');

	restore();
}

void
print_factor(U *p)
{
	if (isnum(p)) {
		print_number(p);
		return;
	}

	if (isstr(p)) {
		//print_str("\"");
		print_str(p->u.str);
		//print_str("\"");
		return;
	}

	if (istensor(p)) {
		print_tensor(p);
		return;
	}

	if (isadd(p) || car(p) == symbol(MULTIPLY)) {
		print_str("(");
		print_expr(p);
		print_str(")");
		return;
	}

	if (car(p) == symbol(POWER)) {

		if (cadr(p) == symbol(E)) {
			print_str("exp(");
			print_expr(caddr(p));
			print_str(")");
			return;
		}

		if (isminusone(caddr(p))) {
			if (test_flag == 0)
				print_str("1 / ");
			else
				print_str("1/");
			if (iscons(cadr(p))) {
				print_str("(");
				print_expr(cadr(p));
				print_str(")");
			} else
				print_expr(cadr(p));
			return;
		}

		if (isadd(cadr(p)) || caadr(p) == symbol(MULTIPLY) || caadr(p) == symbol(POWER) || isnegativenumber(cadr(p))) {
			print_str("(");
			print_expr(cadr(p));
			print_str(")");
		} else if (isnum(cadr(p)) && (lessp(cadr(p), zero) || isfraction(cadr(p)))) {
			print_str("(");
			print_factor(cadr(p));
			print_str(")");
		} else
			print_factor(cadr(p));
		if (test_flag == 0)
			//print_str(" ^ ");
			print_str(power_str);
		else
			print_str("^");
		if (iscons(caddr(p)) || isfraction(caddr(p)) || (isnum(caddr(p)) && lessp(caddr(p), zero))) {
			print_str("(");
			print_expr(caddr(p));
			print_str(")");
		} else
			print_factor(caddr(p));
		return;
	}

//	if (car(p) == _list) {
//		print_str("{");
//		p = cdr(p);
//		if (iscons(p)) {
//			print_expr(car(p));
//			p = cdr(p);
//		}
//		while (iscons(p)) {
//			print_str(",");
//			print_expr(car(p));
//			p = cdr(p);
//		}
//		print_str("}");
//		return;
//	}

	if (car(p) == symbol(INDEX) && issymbol(cadr(p))) {
		print_index_function(p);
		return;
	}

	if (car(p) == symbol(FACTORIAL)) {
		print_factorial_function(p);
		return;
	}

	if (iscons(p)) {
		//if (car(p) == symbol(FORMAL) && cadr(p)->k == SYM) {
		//	print_str(((struct symbol *) cadr(p))->name);
		//	return;
		//}
		print_factor(car(p));
		p = cdr(p);
		print_str("(");
		if (iscons(p)) {
			print_expr(car(p));
			p = cdr(p);
			while (iscons(p)) {
				if (test_flag == 0)
					print_str(",");
				else
					print_str(",");
				print_expr(car(p));
				p = cdr(p);
			}
		}
		print_str(")");
		return;
	}

	if (p == symbol(DERIVATIVE))
		print_char('d');
	else if (p == symbol(E))
		print_str("exp(1)");
	else if (p == symbol(PI))
		print_str("pi");
	else
		print_str(get_printname(p));
}

void
print_index_function(U *p)
{
	p = cdr(p);
	if (caar(p) == symbol(ADD) || caar(p) == symbol(MULTIPLY) || caar(p) == symbol(POWER) || caar(p) == symbol(FACTORIAL))
		print_subexpr(car(p));
	else
		print_expr(car(p));
	print_char('[');
	p = cdr(p);
	if (iscons(p)) {
		print_expr(car(p));
		p = cdr(p);
		while(iscons(p)) {
			print_char(',');
			print_expr(car(p));
			p = cdr(p);
		}
	}
	print_char(']');
}

void
print_factorial_function(U *p)
{
	p = cadr(p);
	if (car(p) == symbol(ADD) || car(p) == symbol(MULTIPLY) || car(p) == symbol(POWER) || car(p) == symbol(FACTORIAL))
		print_subexpr(p);
	else
		print_expr(p);
	print_char('!');
}

void
print_tensor(U *p)
{
	int k = 0;
	print_tensor_inner(p, 0, &k);
}

void
print_tensor_inner(U *p, int j, int *k)
{
	int i;
	print_str("(");
	for (i = 0; i < p->u.tensor->dim[j]; i++) {
		if (j + 1 == p->u.tensor->ndim) {
			print_expr(p->u.tensor->elem[*k]);
			*k = *k + 1;
		} else
			print_tensor_inner(p, j + 1, k);
		if (i + 1 < p->u.tensor->dim[j]) {
			if (test_flag == 0)
				print_str(",");
			else
				print_str(",");
		}
	}
	print_str(")");
}

void
print_str(char *s)
{
	while (*s)
		print_char(*s++);
}

void
print_char(int c)
{
	last_char = c;
	char_count++;
//	if (display_flag == 1)
//		displaychar(c);
//	else
		printchar(c);
}

void
print_function_definition(U *p)
{
	print_str(get_printname(p));
	print_arg_list(cadr(get_binding(p)));
	print_str("=");
	print_expr(caddr(get_binding(p)));
	print_str("\n");
}

void
print_arg_list(U *p)
{
	print_str("(");
	if (iscons(p)) {
		print_str(get_printname(car(p)));
		p = cdr(p);
		while (iscons(p)) {
			print_str(",");
			print_str(get_printname(car(p)));
			p = cdr(p);
		}
	}
	print_str(")");
}

void
print_lisp(U *p)
{
	print1(p);
	print_str("\n");
}

void
print1(U *p)
{
	switch (p->k) {
	case CONS:
		print_str("(");
		print1(car(p));
		p = cdr(p);
		while (iscons(p)) {
			print_str(" ");
			print1(car(p));
			p = cdr(p);
		}
		if (p != symbol(NIL)) {
			print_str(" . ");
			print1(p);
		}
		print_str(")");
		break;
	case STR:
		//print_str("\"");
		print_str(p->u.str);
		//print_str("\"");
		break;
	case NUM:
	case DOUBLE:
		print_number(p);
		break;
	case SYM:
		print_str(get_printname(p));
		break;
	default:
		print_str("<tensor>");
		break;
	}
}

void
print_multiply_sign(void)
{
	if (test_flag == 0)
		print_str(" ");
	else
		print_str("*");
}

int
is_denominator(U *p)
{
	if (car(p) == symbol(POWER) && cadr(p) != symbol(E) && isnegativeterm(caddr(p)))
		return 1;
	else
		return 0;
}

// don't consider the leading fraction

// we want 2/3*a*b*c instead of 2*a*b*c/3

int
any_denominators(U *p)
{
	U *q;
	p = cdr(p);
//	if (isfraction(car(p)))
//		return 1;
	while (iscons(p)) {
		q = car(p);
		if (is_denominator(q))
			return 1;
		p = cdr(p);
	}
	return 0;
}
