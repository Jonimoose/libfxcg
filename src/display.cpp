//-----------------------------------------------------------------------------
//
//	Examples:
//
//	   012345678
//	-2 .........
//	-1 .........
//	 0 ..hello..	x=2, y=0, h=1, w=5
//	 1 .........
//	 2 .........
//
//	   012345678
//	-2 .........
//	-1 ..355....
//	 0 ..---....	x=2, y=-1, h=3, w=3
//	 1 ..113....
//	 2 .........
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

#define YMAX 10000
struct glyph {
	unsigned char c; int x, y;
} chartab[YMAX];

static void emit_expr(U *);
static void emit_term(U *);
static void emit_multiply(U *, int);
static void emit_factor(U *);
static void emit_power(U *);
static void emit_denominator(U *, int);
static void emit_subexpr(U *);
static void fixup_power(int, int);
static void move(int, int, int, int);
static void get_size(int, int, int *, int *, int *);
static void emit_function(U *);
static void emit_symbol(U *);
static void emit_string(U *);
static void __emit_char(int);
static void __emit_str(char *);
static void print_it();
static int count_denominators(U *);
static int __is_negative(U *);
static void emit_fraction(U *, int);
static void emit_numerical_fraction(U *);
static void emit_tensor(U *);
static int isdenominator(U *p);
static void emit_flat_tensor(U *);
static void emit_tensor_inner(U *, int, int *);
static void emit_top_expr(U *);
static void emit_index_function(U *);
static void emit_factorial_function(U *);
static void emit_numerators(U *);
static void emit_denominators(U *);

static int yindex, level, emit_x;
static int expr_level;
int display_flag;

/*#define NGREEK 34

static struct {
        char *s;
        unsigned char* n;
} greek[NGREEK] = {
        {"Gamma",       (unsigned char*)"\xe5\x42"},
        {"Delta",       (unsigned char*)"\xe5\x43"},
        {"Theta",       (unsigned char*)"\xe5\x47"},
        {"Lambda",      (unsigned char*)"\xe5\x4a"},
        {"Xi",          (unsigned char*)"\xe5\x4d"},
        {"Pi",          (unsigned char*)"\xe5\x4f"},
        {"Sigma",       (unsigned char*)"\xe5\x51"},
        {"Upsilon",     (unsigned char*)"\xe5\x54"},
        {"Phi",         (unsigned char*)"\xe5\x55"},
        {"Psi",         (unsigned char*)"\xe5\x57"},
        {"Omega",       (unsigned char*)"\xe5\x58"},
        {"alpha",       (unsigned char*)"\xe6\x40"},
        {"beta",        (unsigned char*)"\xe6\x41"},
        {"gamma",       (unsigned char*)"\xe6\x42"},
        {"delta",       (unsigned char*)"\xe6\x43"},
        {"epsilon",     (unsigned char*)"\xe6\x44"},
        {"zeta",        (unsigned char*)"\xe6\x45"},
        {"eta",         (unsigned char*)"\xe6\x46"},
        {"theta",       (unsigned char*)"\xe6\x47"},
        {"iota",        (unsigned char*)"\xe6\x48"},
        {"kappa",       (unsigned char*)"\xe6\x49"},
        {"lambda",      (unsigned char*)"\xe6\x4a"},
        {"mu",          (unsigned char*)"\xe6\x4b"},
        {"nu",          (unsigned char*)"\xe6\x4c"},
        {"xi",          (unsigned char*)"\xe6\x4d"},
        {"pi",          (unsigned char*)"\xe6\x4f"},
        {"rho",         (unsigned char*)"\xe6\x50"},
        {"sigma",       (unsigned char*)"\xe6\x51"},
        {"tau",         (unsigned char*)"\xe6\x53"},
        {"upsilon",     (unsigned char*)"\xe6\x54"},
        {"phi",         (unsigned char*)"\xe6\x55"},
        {"chi",         (unsigned char*)"\xe6\x56"},
        {"psi",         (unsigned char*)"\xe6\x57"},
        {"omega",       (unsigned char*)"\xe6\x58"},
};

static int
isgreek(char *s)
{
        int i;
        for (i = 0; i < NGREEK; i++)
                if (strncmp(greek[i].s, s, strlen(greek[i].s)) == 0)
                        return i;
        return -1;
}*/

void
display(U *p)
{
	int h, w, y;

	save();

	yindex = 0;
	level = 0;
	emit_x = 0;
	emit_top_expr(p);

	// if too wide then print flat

	get_size(0, yindex, &h, &w, &y);

	if (w > 32 || h > 10) {
		printline(p);
		restore();
		return;
	}

	print_it();

	restore();
}

static void
emit_top_expr(U *p)
{
	if (car(p) == symbol(SETQ)) {
		emit_expr(cadr(p));
		__emit_str(" = ");
		emit_expr(caddr(p));
		return;
	}

	if (istensor(p))
		emit_tensor(p);
	else
		emit_expr(p);
}

static int
will_be_displayed_as_fraction(U *p)
{
	if (level > 0)
		return 0;
	if (isfraction(p))
		return 1;
	if (car(p) != symbol(MULTIPLY))
		return 0;
	if (isfraction(cadr(p)))
		return 1;
	while (iscons(p)) {
		if (isdenominator(car(p)))
			return 1;
		p = cdr(p);
	}
	return 0;
}

static void
emit_expr(U *p)
{
//	if (level > 0) {
//		printexpr(p);
//		return;
//	}
	expr_level++;
	if (car(p) == symbol(ADD)) {
		p = cdr(p);
		if (__is_negative(car(p))) {
			__emit_char('-');
			if (will_be_displayed_as_fraction(car(p)))
				__emit_char(' ');
		}
		emit_term(car(p));
		p = cdr(p);
		while (iscons(p)) {
			if (__is_negative(car(p))) {
				//if (expr_level == 1)
					__emit_char(' ');
				__emit_char('-');
				//if (expr_level == 1)
					__emit_char(' ');
			} else {
				//if (expr_level == 1)
					__emit_char(' ');
				__emit_char('+');
				//if (expr_level == 1)
					__emit_char(' ');
			}
			emit_term(car(p));
			p = cdr(p);
		}
	} else {
		if (__is_negative(p)) {
			__emit_char('-');
			if (will_be_displayed_as_fraction(p))
				__emit_char(' ');
		}
		emit_term(p);
	}
	expr_level--;
}

static void
emit_unsigned_expr(U *p)
{
	if (car(p) == symbol(ADD)) {
		p = cdr(p);
//		if (__is_negative(car(p)))
//			__emit_char('-');
		emit_term(car(p));
		p = cdr(p);
		while (iscons(p)) {
			if (__is_negative(car(p))) {
				__emit_char(' ');
				__emit_char('-');
				__emit_char(' ');
			} else {
				__emit_char(' ');
				__emit_char('+');
				__emit_char(' ');
			}
			emit_term(car(p));
			p = cdr(p);
		}
	} else {
//		if (__is_negative(p))
//			__emit_char('-');
		emit_term(p);
	}
}

static int
__is_negative(U *p)
{
	if (isnegativenumber(p))
		return 1;
	if (car(p) == symbol(MULTIPLY) && isnegativenumber(cadr(p)))
		return 1;
	return 0;
}

static void
emit_term(U *p)
{
	int n;
	if (car(p) == symbol(MULTIPLY)) {
		n = count_denominators(p);
		if (n && level == 0)
			emit_fraction(p, n);
		else
			emit_multiply(p, n);
	} else
		emit_factor(p);
}

static int
isdenominator(U *p)
{
	if (car(p) == symbol(POWER) && cadr(p) != symbol(E) && __is_negative(caddr(p)))
		return 1;
	else
		return 0;
}

static int
count_denominators(U *p)
{
	int count = 0;
	U *q;
	p = cdr(p);
//	if (isfraction(car(p))) {
//		count++;
//		p = cdr(p);
//	}
	while (iscons(p)) {
		q = car(p);
		if (isdenominator(q))
			count++;
		p = cdr(p);
	}
	return count;
}

// n is the number of denominators, not counting a fraction like 1/2

static void
emit_multiply(U *p, int n)
{
	if (n == 0) {
		p = cdr(p);
		if (isplusone(car(p)) || isminusone(car(p)))
			p = cdr(p);
		emit_factor(car(p));
		p = cdr(p);
		while (iscons(p)) {
			//__emit_char(' ');
			__emit_char(31); // show a little dot instead of a space. rest of the magic resides on PrintMiniFix
			emit_factor(car(p));
			p = cdr(p);
		}
	} else {
		emit_numerators(p);
		__emit_char('/');
		// need grouping if more than one denominator
		if (n > 1 || isfraction(cadr(p))) {
			__emit_char('(');
			emit_denominators(p);
			__emit_char(')');
		} else
			emit_denominators(p);
	}
}

#define A p3
#define B p4

// sign of term has already been emitted

static void
emit_fraction(U *p, int d)
{
	int count, k1, k2, n, x;

	save();

	A = one;
	B = one;

	// handle numerical coefficient

	if (isrational(cadr(p))) {
		push(cadr(p));
		mp_numerator();
		absval();
		A = pop();
		push(cadr(p));
		mp_denominator();
		B = pop();
	}

	if (isdouble(cadr(p))) {
		push(cadr(p));
		absval();
		A = pop();
	}

	// count numerators

	if (isplusone(A))
		n = 0;
	else
		n = 1;
	p1 = cdr(p);
	if (isnum(car(p1)))
		p1 = cdr(p1);
	while (iscons(p1)) {
		p2 = car(p1);
		if (isdenominator(p2))
			;
		else
			n++;
		p1 = cdr(p1);
	}

	// emit numerators

	x = emit_x;

	k1 = yindex;

	count = 0;

	// emit numerical coefficient

	if (!isplusone(A)) {
		emit_number(A, 0);
		count++;
	}

	// skip over "multiply"

	p1 = cdr(p);

	// skip over numerical coefficient, already handled

	if (isnum(car(p1)))
		p1 = cdr(p1);

	while (iscons(p1)) {
		p2 = car(p1);
		if (isdenominator(p2))
			;
		else {
			if (count > 0)
				__emit_char(' ');
			if (n == 1)
				emit_expr(p2);
			else
				emit_factor(p2);
			count++;
		}
		p1 = cdr(p1);
	}

	if (count == 0)
		__emit_char('1');

	// emit denominators

	k2 = yindex;

	count = 0;

	if (!isplusone(B)) {
		emit_number(B, 0);
		count++;
		d++;
	}

	p1 = cdr(p);

	if (isrational(car(p1)))
		p1 = cdr(p1);

	while (iscons(p1)) {
		p2 = car(p1);
		if (isdenominator(p2)) {
			if (count > 0)
				__emit_char(' ');
			emit_denominator(p2, d);
			count++;
		}
		p1 = cdr(p1);
	}

	fixup_fraction(x, k1, k2);

	restore();
}

// p points to a multiply

static void
emit_numerators(U *p)
{
	int n;

	save();

	p1 = one;

	p = cdr(p);

	if (isrational(car(p))) {
		push(car(p));
		mp_numerator();
		absval();
		p1 = pop();
		p = cdr(p);
	} else if (isdouble(car(p))) {
		push(car(p));
		absval();
		p1 = pop();
		p = cdr(p);
	}

	n = 0;

	if (!isplusone(p1)) {
		emit_number(p1, 0);
		n++;
	}

	while (iscons(p)) {
		if (isdenominator(car(p)))
			;
		else {
			if (n > 0)
				__emit_char(' ');
			emit_factor(car(p));
			n++;
		}
		p = cdr(p);
	}

	if (n == 0)
		__emit_char('1');

	restore();
}

// p points to a multiply

static void
emit_denominators(U *p)
{
	int n;

	save();

	n = 0;

	p = cdr(p);

	if (isfraction(car(p))) {
		push(car(p));
		mp_denominator();
		p1 = pop();
		emit_number(p1, 0);
		n++;
		p = cdr(p);
	}

	while (iscons(p)) {
		if (isdenominator(car(p))) {
			if (n > 0)
				__emit_char(' ');
			emit_denominator(car(p), 0);
			n++;
		}
		p = cdr(p);
	}

	restore();
}

static void
emit_factor(U *p)
{
	if (istensor(p)) {
		if (level == 0)
			//emit_tensor(p);
			emit_flat_tensor(p);
		else
			emit_flat_tensor(p);
		return;
	}

	if (isdouble(p)) {
		emit_number(p, 0);
		return;
	}

	if (car(p) == symbol(ADD) || car(p) == symbol(MULTIPLY)) {
		emit_subexpr(p);
		return;
	}

	if (car(p) == symbol(POWER)) {
		emit_power(p);
		return;
	}

	if (iscons(p)) {
		//if (car(p) == symbol(FORMAL) && cadr(p)->k == SYM)
		//	emit_symbol(cadr(p));
		//else
			emit_function(p);
		return;
	}

	if (isnum(p)) {
		if (level == 0)
			emit_numerical_fraction(p);
		else
			emit_number(p, 0);
		return;
	}

	if (issymbol(p)) {
		emit_symbol(p);
		return;
	}

	if (isstr(p)) {
		emit_string(p);
		return;
	}
}

static void
emit_numerical_fraction(U *p)
{
	int k1, k2, x;

	save();

	push(p);
	mp_numerator();
	absval();
	A = pop();

	push(p);
	mp_denominator();
	B = pop();

	if (isplusone(B)) {
		emit_number(A, 0);
		restore();
		return;
	}

	x = emit_x;

	k1 = yindex;

	emit_number(A, 0);

	k2 = yindex;

	emit_number(B, 0);

	fixup_fraction(x, k1, k2);

	restore();
}

// if it's a factor then it doesn't need parens around it, i.e. 1/sin(theta)^2

static int
isfactor(U *p)
{
	if (iscons(p) && car(p) != symbol(ADD) && car(p) != symbol(MULTIPLY) && car(p) != symbol(POWER))
		return 1;
	if (issymbol(p))
		return 1;
	if (isfraction(p))
		return 0;
	if (isnegativenumber(p))
		return 0;
	if (isnum(p))
		return 1;
	return 0;
}

static void
emit_power(U *p)
{
	int k1, k2, x;

	if (cadr(p) == symbol(E)) {
		__emit_str("exp(");
		emit_expr(caddr(p));
		__emit_char(')');
		return;
	}

	if (level > 0) {
		if (isminusone(caddr(p))) {
			__emit_char('1');
			__emit_char('/');
			if (isfactor(cadr(p)))
				emit_factor(cadr(p));
			else
				emit_subexpr(cadr(p));
		} else {
			if (isfactor(cadr(p)))
				emit_factor(cadr(p));
			else
				emit_subexpr(cadr(p));
			__emit_char('^');
			if (isfactor(caddr(p)))
				emit_factor(caddr(p));
			else
				emit_subexpr(caddr(p));
		}
		return;
	}

	// special case: 1 over something

	if (__is_negative(caddr(p))) {
		x = emit_x;
		k1 = yindex;
		__emit_char('1');
		k2 = yindex;
		//level++;
		emit_denominator(p, 1);
		//level--;
		fixup_fraction(x, k1, k2);
		return;
	}

	k1 = yindex;
	if (isfactor(cadr(p)))
		emit_factor(cadr(p));
	else
		emit_subexpr(cadr(p));
	k2 = yindex;
	level++;
	emit_expr(caddr(p));
	level--;
	fixup_power(k1, k2);
}

// if n == 1 then emit as expr (no parens)

// p is a power

static void
emit_denominator(U *p, int n)
{
	int k1, k2;

	// special case: 1 over something

	if (isminusone(caddr(p))) {
		if (n == 1)
			emit_expr(cadr(p));
		else
			emit_factor(cadr(p));
		return;
	}

	k1 = yindex;

	// emit base

	if (isfactor(cadr(p)))
		emit_factor(cadr(p));
	else
		emit_subexpr(cadr(p));

	k2 = yindex;

	// emit exponent, don't emit minus sign

	level++;

	emit_unsigned_expr(caddr(p));

	level--;

	fixup_power(k1, k2);
}

static void
emit_function(U *p)
{
	if (car(p) == symbol(INDEX) && issymbol(cadr(p))) {
		emit_index_function(p);
		return;
	}

	if (car(p) == symbol(FACTORIAL)) {
		emit_factorial_function(p);
		return;
	}

	if (car(p) == symbol(DERIVATIVE))
		__emit_char('d');
	else
		emit_symbol(car(p));
	__emit_char('(');
	p = cdr(p);
	if (iscons(p)) {
		emit_expr(car(p));
		p = cdr(p);
		while (iscons(p)) {
			__emit_char(',');
			//__emit_char(' ');
			emit_expr(car(p));
			p = cdr(p);
		}
	}
	__emit_char(')');
}

static void
emit_index_function(U *p)
{
	p = cdr(p);
	if (caar(p) == symbol(ADD) || caar(p) == symbol(MULTIPLY) || caar(p) == symbol(POWER) || caar(p) == symbol(FACTORIAL))
		emit_subexpr(car(p));
	else
		emit_expr(car(p));
	__emit_char('[');
	p = cdr(p);
	if (iscons(p)) {
		emit_expr(car(p));
		p = cdr(p);
		while(iscons(p)) {
			__emit_char(',');
			emit_expr(car(p));
			p = cdr(p);
		}
	}
	__emit_char(']');
}

static void
emit_factorial_function(U *p)
{
	p = cadr(p);
	if (car(p) == symbol(ADD) || car(p) == symbol(MULTIPLY) || car(p) == symbol(POWER) || car(p) == symbol(FACTORIAL))
		emit_subexpr(p);
	else
		emit_expr(p);
	__emit_char('!');
}

static void
emit_subexpr(U *p)
{
	__emit_char('(');
	emit_expr(p);
	__emit_char(')');
}

static void
emit_symbol(U *p)
{
	char *s;

	if (p == symbol(E)) {
		__emit_str("exp(1)");
		return;
	}

	s = get_printname(p);
	while (*s)
		__emit_char(*s++);
}

static void
emit_string(U *p)
{
	char *s;
	s = p->u.str;
	while (*s)
		__emit_char(*s++);
}

void
fixup_fraction(int x, int k1, int k2)
{
	int dx, dy, i, w, y;
	int h1, w1, y1;
	int h2, w2, y2;

	get_size(k1, k2, &h1, &w1, &y1);
	get_size(k2, yindex, &h2, &w2, &y2);

	if (w2 > w1)
		dx = (w2 - w1) / 2;	// shift numerator right
	else
		dx = 0;
dx++;
	// this is how much is below the baseline

	y = y1 + h1 - 1;

	dy = -y - 1;

	move(k1, k2, dx, dy);

	if (w2 > w1)
		dx = -w1;
	else
		dx = -w1 + (w1 - w2) / 2;
dx++;
	dy = -y2 + 1;

	move(k2, yindex, dx, dy);

	if (w2 > w1)
		w = w2;
	else
		w = w1;
w+=2;
	emit_x = x;

	for (i = 0; i < w; i++)
		//__emit_char('-');
                __emit_char(9);
}

static void
fixup_power(int k1, int k2)
{
	int dy;
	int h1, w1, y1;
	int h2, w2, y2;

	get_size(k1, k2, &h1, &w1, &y1);
	get_size(k2, yindex, &h2, &w2, &y2);

	// move superscript to baseline

	dy = -y2 - h2 + 1;

	// now move above base

	dy += y1 - 1;

	move(k2, yindex, 0, dy);
}

static void
move(int j, int k, int dx, int dy)
{
	int i;
	for (i = j; i < k; i++) {
		chartab[i].x += dx;
		chartab[i].y += dy;
	}
}

// finds the bounding rectangle and vertical position

static void
get_size(int j, int k, int *h, int *w, int *y)
{
	int i;
	int min_x, max_x, min_y, max_y;
	min_x = chartab[j].x;
	max_x = chartab[j].x;
	min_y = chartab[j].y;
	max_y = chartab[j].y;
	for (i = j + 1; i < k; i++) {
		if (chartab[i].x < min_x)
			min_x = chartab[i].x;
		if (chartab[i].x > max_x)
			max_x = chartab[i].x;
		if (chartab[i].y < min_y)
			min_y = chartab[i].y;
		if (chartab[i].y > max_y)
			max_y = chartab[i].y;
	}
	*h = max_y - min_y + 1;
	*w = max_x - min_x + 1;
	*y = min_y;
}

void
displaychar(int c)
{
	__emit_char(c);
}

static void
__emit_char(int c)
{
	if (yindex == YMAX)
		return;
	chartab[yindex].c = c;
	chartab[yindex].x = emit_x;
	chartab[yindex].y = 0;
	yindex++;
	emit_x++;
}

static void
__emit_str(char *s)
{
	while (*s)
		__emit_char(*s++);
}

void
emit_number(U *p, int emit_sign)
{
	char *s;
	static char buf[100];
	switch (p->k) {
	case NUM:
		s = mstr(p->u.q.a);
		if (*s == '-' && emit_sign == 0)
			s++;
		while (*s)
			__emit_char(*s++);
		s = mstr(p->u.q.b);
		if (strcmp(s, "1") == 0)
			break;
		__emit_char('/');
		while (*s)
			__emit_char(*s++);
		break;
	case DOUBLE:
		sprintf(buf, "%.10g", p->u.d);
		s = buf;
		if (*s == '-' && emit_sign == 0)
			s++;
		while (*s)
			__emit_char(*s++);
		break;
	default:
		break;
	}
}

static int
__cmp(const void *aa, const void *bb)
{
	struct glyph *a, *b;

	a = (struct glyph *) aa;
	b = (struct glyph *) bb;

	if (a->y < b->y)
		return -1;

	if (a->y > b->y)
		return 1;

	if (a->x < b->x)
		return -1;

	if (a->x > b->x)
		return 1;

	return 0;
}

static void
print_it(void)
{
	int i, x, y;

	qsort(chartab, yindex, sizeof (struct glyph), __cmp);

	x = 0;

	y = chartab[0].y;

	for (i = 0; i < yindex; i++) {

		while (chartab[i].y > y) {
			printchar('\n');
			x = 0;
			y++;
		}

		while (chartab[i].x > x) {
			printchar_nowrap(' ');
			x++;
		}

		printchar_nowrap(chartab[i].c);

		x++;
	}

	printchar('\n');
}

static void fill_buf(void);

static char buffer[10000];

char *
getdisplaystr(void)
{
	yindex = 0;
	level = 0;
	emit_x = 0;
	emit_expr(pop());
	fill_buf();
	return buffer;
}

static void
fill_buf(void)
{
	int i, x, y;
	char *s = buffer;

	qsort(chartab, yindex, sizeof (struct glyph), __cmp);

	x = 0;

	y = chartab[0].y;

	for (i = 0; i < yindex; i++) {

		while (chartab[i].y > y) {
			*s++ = '\n';
			x = 0;
			y++;
		}

		while (chartab[i].x > x) {
			*s++ = ' ';
			x++;
		}

		*s++ = chartab[i].c;

		x++;
	}

	*s++ = '\n';

	*s++ = '\0';
}

#define N 100

struct elem {
	int x, y, h, w, index, count;
} elem[N];

#define SPACE_BETWEEN_COLUMNS 3
#define SPACE_BETWEEN_ROWS 1

static void
emit_tensor(U *p)
{
	int i, n, nrow, ncol;
	int x, y;
	int h, w;
	int dx, dy;
	int eh, ew;
	int row, col;

	if (p->u.tensor->ndim > 2) {
		emit_flat_tensor(p);
		return;
	}

	nrow = p->u.tensor->dim[0];

	if (p->u.tensor->ndim == 2)
		ncol = p->u.tensor->dim[1];
	else
		ncol = 1;

	n = nrow * ncol;

	if (n > N) {
		emit_flat_tensor(p);
		return;
	}

	// horizontal coordinate of the matrix

#if 0
	emit_x += 2; // make space for left paren
#endif

	x = emit_x;

	// emit each element

	for (i = 0; i < n; i++) {
		elem[i].index = yindex;
		elem[i].x = emit_x;
		emit_expr(p->u.tensor->elem[i]);
		elem[i].count = yindex - elem[i].index;
		get_size(elem[i].index, yindex, &elem[i].h, &elem[i].w, &elem[i].y);
	}

	// find element height and width

	eh = 0;
	ew = 0;

	for (i = 0; i < n; i++) {
		if (elem[i].h > eh)
			eh = elem[i].h;
		if (elem[i].w > ew)
			ew = elem[i].w;
	}

	// this is the overall height of the matrix

	h = nrow * eh + (nrow - 1) * SPACE_BETWEEN_ROWS;

	// this is the overall width of the matrix

	w = ncol * ew + (ncol - 1) * SPACE_BETWEEN_COLUMNS;

	// this is the vertical coordinate of the matrix

	y = -(h / 2);

	// move elements around

	for (row = 0; row < nrow; row++) {
		for (col = 0; col < ncol; col++) {

			i = row * ncol + col;

			// first move to upper left corner of matrix

			dx = x - elem[i].x;
			dy = y - elem[i].y;

			move(elem[i].index, elem[i].index + elem[i].count, dx, dy);

			// now move to official position

			dx = 0;

			if (col > 0)
				dx = col * (ew + SPACE_BETWEEN_COLUMNS);

			dy = 0;

			if (row > 0)
				dy = row * (eh + SPACE_BETWEEN_ROWS);

			// small correction for horizontal centering

			dx += (ew - elem[i].w) / 2;

			// small correction for vertical centering

			dy += (eh - elem[i].h) / 2;

			move(elem[i].index, elem[i].index + elem[i].count, dx, dy);
		}
	}

	emit_x = x + w;

#if 0

	// left brace

	for (i = 0; i < h; i++) {
		if (yindex == YMAX)
			break;
		chartab[yindex].c = '|';
		chartab[yindex].x = x - 2;
		chartab[yindex].y = y + i;
		yindex++;
	}

	// right brace

	emit_x++;

	for (i = 0; i < h; i++) {
		if (yindex == YMAX)
			break;
		chartab[yindex].c = '|';
		chartab[yindex].x = emit_x;
		chartab[yindex].y = y + i;
		yindex++;
	}

	emit_x++;

#endif

}

static void
emit_flat_tensor(U *p)
{
	int k = 0;
	emit_tensor_inner(p, 0, &k);
}

static void
emit_tensor_inner(U *p, int j, int *k)
{
	int i;
	__emit_char('(');
	for (i = 0; i < p->u.tensor->dim[j]; i++) {
		if (j + 1 == p->u.tensor->ndim) {
			emit_expr(p->u.tensor->elem[*k]);
			*k = *k + 1;
		} else
			emit_tensor_inner(p, j + 1, k);
		if (i + 1 < p->u.tensor->dim[j])
			__emit_char(',');
	}
	__emit_char(')');
}

#if SELFTEST

static char *s[] = {

	"format=1",
	"",

	"((a,b),(c,d))",
	"a   b\n"
	"\n"
	"c   d",

	"1/sqrt(-15)",

	"       i\n"
	"- -----------\n"
	"    1/2  1/2\n"
	"   3    5",

	"x^(1/a)",

	" 1/a\n"
	"x",

	"x^(a/b)",

	" a/b\n"
	"x",

	"x^(a/2)",

	" 1/2 a\n"
	"x",

	"x^(1/(a+b))",

	" 1/(a + b)\n"
	"x",
};

void
test_display(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
