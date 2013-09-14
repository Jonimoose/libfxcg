#include "stdafx.h"
#include "defs.h"

void
new_string(char *s)
{
	save();
	p1 = alloc();
	p1->k = STR;
	p1->u.str = strdup(s);
	push(p1);
	restore();
}

void
out_of_memory(void)
{
	stop("out of memory");
}

void
push_zero_matrix(int i, int j)
{
	push(alloc_tensor(i * j));
	stack[tos - 1]->u.tensor->ndim = 2;
	stack[tos - 1]->u.tensor->dim[0] = i;
	stack[tos - 1]->u.tensor->dim[1] = j;
}

void
push_identity_matrix(int n)
{
	int i;
	push_zero_matrix(n, n);
	for (i = 0; i < n; i++)
		stack[tos - 1]->u.tensor->elem[i * n + i] = one;
}

void
push_cars(U *p)
{
	while (iscons(p)) {
		push(car(p));
		p = cdr(p);
	}
}

void
peek(void)
{
	save();
	p1 = pop();
	push(p1);
	printline(p1);
	restore();
}

void
peek2(void)
{
	print_lisp(stack[tos - 2]);
	print_lisp(stack[tos - 1]);
}

int
equal(U *p1, U *p2)
{
	if (cmp_expr(p1, p2) == 0)
		return 1;
	else
		return 0;
}

int
lessp(U *p1, U *p2)
{
	if (cmp_expr(p1, p2) < 0)
		return 1;
	else
		return 0;
}

int
sign(int n)
{
	if (n < 0)
		return -1;
	else if (n > 0)
		return 1;
	else
		return 0;
}

int
cmp_expr(U *p1, U *p2)
{
	int n;

	if (p1 == p2)
		return 0;

	if (p1 == symbol(NIL))
		return -1;

	if (p2 == symbol(NIL))
		return 1;

	if (isnum(p1) && isnum(p2))
		return sign(compare_numbers(p1, p2));

	if (isnum(p1))
		return -1;

	if (isnum(p2))
		return 1;

	if (isstr(p1) && isstr(p2))
		return sign(strcmp(p1->u.str, p2->u.str));

	if (isstr(p1))
		return -1;

	if (isstr(p2))
		return 1;

	if (issymbol(p1) && issymbol(p2))
		return sign(strcmp(get_printname(p1), get_printname(p2)));

	if (issymbol(p1))
		return -1;

	if (issymbol(p2))
		return 1;

	if (istensor(p1) && istensor(p2))
		return compare_tensors(p1, p2);

	if (istensor(p1))
		return -1;

	if (istensor(p2))
		return 1;

	while (iscons(p1) && iscons(p2)) {
		n = cmp_expr(car(p1), car(p2));
		if (n != 0)
			return n;
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	if (iscons(p2))
		return -1;

	if (iscons(p1))
		return 1;

	return 0;
}

int
length(U *p)
{
	int n = 0;
	while (iscons(p)) {
		p = cdr(p);
		n++;
	}
	return n;
}

static void unique_f(U *);

U *
unique(U *p)
{
	save();
	p1 = symbol(NIL);
	p2 = symbol(NIL);
	unique_f(p);
	if (p2 != symbol(NIL))
		p1 = symbol(NIL);
	p = p1;
	restore();
	return p;
}

static void
unique_f(U *p)
{
	if (isstr(p)) {
		if (p1 == symbol(NIL))
			p1 = p;
		else if (p != p1)
			p2 = p;
		return;
	}
	while (iscons(p)) {
		unique_f(car(p));
		if (p2 != symbol(NIL))
			return;
		p = cdr(p);
	}
}

#if 0

void
check_endianess(void)
{
	int tmp = 1;
	if (((char *) &tmp)[0] == 1 && Y_LITTLE_ENDIAN == 0) {
		printf("Please change Y_LITTLE_ENDIAN to 1 in defs.h and recompile.\n");
		Exit(1);
	}
	if (((char *) &tmp)[0] == 0 && Y_LITTLE_ENDIAN != 0) {
		printf("Please change Y_LITTLE_ENDIAN to 0 in defs.h and recompile.\n");
		Exit(1);
	}
}

#endif

void
ssqrt(void)
{
	push_rational(1, 2);
	power();
}

void
yyexpand(void)
{
	int x;
	x = expanding;
	expanding = 1;
	eval();
	expanding = x;
}

void
exponential(void)
{
	push_symbol(E);
	swap();
	power();
}

void
square(void)
{
	push_integer(2);
	power();
}

static int
__cmp(const void *p1, const void *p2)
{
	return cmp_expr(*((U **) p1), *((U **) p2));
}

void
sort_stack(int n)
{
	qsort(stack + tos - n, n, sizeof (U *), __cmp);
}
