// This scanner uses the recursive descent method.
//
// The char pointers token_str and scan_str are pointers to the input string as
// in the following example.
//
//	| g | a | m | m | a |   | a | l | p | h | a |
//	  ^                   ^
//	  token_str           scan_str
//
// The char pointer token_buf points to a malloc buffer.
//
//	| g | a | m | m | a | \0 |
//	  ^
//	  token_buf

#include "stdafx.h"
#include "defs.h"
#include <fxcg/system.h> // NOTE WARNING /// REMOVE, for debugging gbl08ma

#define T_INTEGER 1001
#define T_DOUBLE 1002
#define T_SYMBOL 1003
#define T_FUNCTION 1004
#define T_NEWLINE 1006
#define T_STRING 1007
#define T_GTEQ 1008
#define T_LTEQ 1009
#define T_EQ 1010

static int token, newline_flag, meta_mode;
static char *input_str, *scan_str, *token_str, *token_buf;

// Returns number of chars scanned and expr on stack.

// Returns zero when nothing left to scan.

int
scan(char *s)
{
	meta_mode = 0;
	expanding++;
	input_str = s;
	scan_str = s;
	get_next_token();
	if (token == 0) {
		push(symbol(NIL));
		expanding--;
		return 0;
	}
	scan_stmt();
	expanding--;
	return (int) (token_str - input_str);
}

int
scan_meta(char *s)
{
	meta_mode = 1;
	expanding++;
	input_str = s;
	scan_str = s;
	get_next_token();
	if (token == 0) {
		push(symbol(NIL));
		expanding--;
		return 0;
	}
	scan_stmt();
	expanding--;
	return (int) (token_str - input_str);
}

void
scan_stmt(void)
{
	scan_relation();
	if (token == '=') {
		get_next_token();
		push_symbol(SETQ);
		swap();
		scan_relation();
		list(3);
	}
}

void
scan_relation(void)
{
	scan_expression();
	switch (token) {
	case T_EQ:
		push_symbol(TESTEQ);
		swap();
		get_next_token();
		scan_expression();
		list(3);
		break;
	case T_LTEQ:
		push_symbol(TESTLE);
		swap();
		get_next_token();
		scan_expression();
		list(3);
		break;
	case T_GTEQ:
		push_symbol(TESTGE);
		swap();
		get_next_token();
		scan_expression();
		list(3);
		break;
	case '<':
		push_symbol(TESTLT);
		swap();
		get_next_token();
		scan_expression();
		list(3);
		break;
	case '>':
		push_symbol(TESTGT);
		swap();
		get_next_token();
		scan_expression();
		list(3);
		break;
	default:
		break;
	}
}

void
scan_expression(void)
{
	int h = tos;
	switch (token) {
	case '+':
		get_next_token();
		scan_term();
		break;
	case '-':
		get_next_token();
		scan_term();
		negate();
		break;
	default:
		scan_term();
		break;
	}
	while (newline_flag == 0 && (token == '+' || token == '-')) {
		if (token == '+') {
			get_next_token();
			scan_term();
		} else {
			get_next_token();
			scan_term();
			negate();
		}
	}
	if (tos - h > 1) {
		list(tos - h);
		push_symbol(ADD);
		swap();
		cons();
	}
}

int
is_factor(void)
{
	switch (token) {
	case '*':
	case '/':
		return 1;
	case '(':
	case T_SYMBOL:
	case T_FUNCTION:
	case T_INTEGER:
	case T_DOUBLE:
	case T_STRING:
		if (newline_flag) {	// implicit mul can't cross line
			scan_str = token_str;	// better error display
			return 0;
		} else
			return 1;
	default:
		break;
	}
	return 0;
}

void
scan_term(void)
{
	int h = tos;

	scan_power();

	// discard integer 1

	if (tos > h && isrational(stack[tos - 1]) && equaln(stack[tos - 1], 1))
		pop();

	while (is_factor()) {
		if (token == '*') {
			get_next_token();
			scan_power();
		} else if (token == '/') {
			get_next_token();
			scan_power();
			inverse();
		} else
			scan_power();

		// fold constants

		if (tos > h + 1 && isnum(stack[tos - 2]) && isnum(stack[tos - 1]))
			multiply();

		// discard integer 1

		if (tos > h && isrational(stack[tos - 1]) && equaln(stack[tos - 1], 1))
			pop();
	}

	if (h == tos)
		push_integer(1);
	else if (tos - h > 1) {
		list(tos - h);
		push_symbol(MULTIPLY);
		swap();
		cons();
	}
}

void
scan_power(void)
{
	scan_factor();
	if (token == '^') {
		get_next_token();
		push_symbol(POWER);
		swap();
		scan_power();
		list(3);
	}
}

void
scan_factor(void)
{
	int h;

	h = tos;

	if (token == '(')
		scan_subexpr();
	else if (token == T_SYMBOL)
		scan_symbol();
	else if (token == T_FUNCTION)
		scan_function_call();
	else if (token == T_INTEGER) {
		bignum_scan_integer(token_buf);
		get_next_token();
	} else if (token == T_DOUBLE) {
		bignum_scan_float(token_buf);
		get_next_token();
	} else if (token == T_STRING)
		scan_string();
	else
		error("syntax error");

	// index

	if (token == '[') {
		get_next_token();
		push_symbol(INDEX);
		swap();
		scan_expression();
		while (token == ',') {
			get_next_token();
			scan_expression();
		}
		if (token != ']')
			error("] expected");
		get_next_token();
		list(tos - h);
	}

	while (token == '!') {
		get_next_token();
		push_symbol(FACTORIAL);
		swap();
		list(2);
	}
}

void
scan_symbol(void)
{
	if (token != T_SYMBOL)
		error("symbol expected");
	if (meta_mode && strlen(token_buf) == 1)
		switch (token_buf[0]) {
		case 'a':
			push(symbol(METAA));
			break;
		case 'b':
			push(symbol(METAB));
			break;
		case 'x':
			push(symbol(METAX));
			break;
		default:
			push(usr_symbol(token_buf));
			break;
		}
	else
		push(usr_symbol(token_buf));
	get_next_token();
}

void
scan_string(void)
{
	new_string(token_buf);
	get_next_token();
}

void
scan_function_call(void)
{
	int n = 1;
	U *p;
	p = usr_symbol(token_buf);
	push(p);
	get_next_token();	// function name
	get_next_token();	// left paren
	if (token != ')') {
		scan_stmt();
		n++;
		while (token == ',') {
			get_next_token();
			scan_stmt();
			n++;
		}
	}
	if (token != ')')
		error(") expected");
	get_next_token();
	list(n);
}

// scan subexpression

void
scan_subexpr(void)
{
	int n;
	if (token != '(')
		error("( expected");
	get_next_token();
	scan_stmt();
	if (token == ',') {
		n = 1;
		while (token == ',') {
			get_next_token();
			scan_stmt();
			n++;
		}
		build_tensor(n);
	}
	if (token != ')')
		error(") expected");
	get_next_token();
}

void
error(char *errmsg)
{
	printchar('\n');

	// try not to put question mark on orphan line

	while (input_str != scan_str) {
		if ((*input_str == '\n' || *input_str == '\r') && input_str + 1 == scan_str)
			break;
		printchar(*input_str++);
	}

	printstr(" ? ");

	while (*input_str && (*input_str != '\n' && *input_str != '\r'))
		printchar(*input_str++);

	printchar('\n');

	stop(errmsg);
}

// There are n expressions on the stack, possibly tensors.
//
// This function assembles the stack expressions into a single tensor.
//
// For example, at the top level of the expression ((a,b),(c,d)), the vectors
// (a,b) and (c,d) would be on the stack.

void
build_tensor(int n)
{
	// int i, j, k, ndim, nelem;

	int i;

	U **s;

	save();

	s = stack + tos - n;

	p2 = alloc_tensor(n);
	p2->u.tensor->ndim = 1;
	p2->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++)
		p2->u.tensor->elem[i] = s[i];

	tos -= n;

	push(p2);

	restore();
}

void
get_next_token()
{
	newline_flag = 0;
	while (1) {
		get_token();
		if (token != T_NEWLINE)
			break;
		newline_flag = 1;
	}
}

void
get_token(void)
{
	// skip spaces

	while (isspace(*scan_str)) {
		if (*scan_str == '\n' || *scan_str == '\r') {
			token = T_NEWLINE;
			scan_str++;
			return;
		}
		scan_str++;
	}

	token_str = scan_str;

	// end of string?

	if (*scan_str == 0) {
		token = 0;
		return;
	}

	// number?

	if (isdigit(*scan_str) || *scan_str == '.') {
		while (isdigit(*scan_str))
			scan_str++;
		if (*scan_str == '.') {
			scan_str++;
			while (isdigit(*scan_str))
				scan_str++;
			if (*scan_str == 'e' && (scan_str[1] == '+' || scan_str[1] == '-' || isdigit(scan_str[1]))) {
				scan_str += 2;
				while (isdigit(*scan_str))
					scan_str++;
			}
			token = T_DOUBLE;
		} else
			token = T_INTEGER;
		update_token_buf(token_str, scan_str);
		return;
	}

	// symbol?

	if (isalpha(*scan_str)) {
		while (isalnum(*scan_str))
			scan_str++;
		if (*scan_str == '(')
			token = T_FUNCTION;
		else
			token = T_SYMBOL;
		update_token_buf(token_str, scan_str);
		return;
	}

	// string ?

	if (*scan_str == '"') {
		scan_str++;
		while (*scan_str != '"') {
			if (*scan_str == 0 || *scan_str == '\n' || *scan_str == '\r')
				error("runaway string");
			scan_str++;
		}
		scan_str++;
		token = T_STRING;
		update_token_buf(token_str + 1, scan_str - 1);
		return;
	}

	// comment?

	if (*scan_str == '#' || (*scan_str == '-' && scan_str[1] == '-')) {
		while (*scan_str && *scan_str != '\n' && *scan_str != '\r')
			scan_str++;
		if (*scan_str)
			scan_str++;
		token = T_NEWLINE;
		return;
	}

	// relational operator?

	if (*scan_str == '=' && scan_str[1] == '=') {
		scan_str += 2;
		token = T_EQ;
		return;
	}

	if (*scan_str == '<' && scan_str[1] == '=') {
		scan_str += 2;
		token = T_LTEQ;
		return;
	}

	if (*scan_str == '>' && scan_str[1] == '=') {
		scan_str += 2;
		token = T_GTEQ;
		return;
	}

	// single char token

	token = *scan_str++;
}

void
update_token_buf(char *a, char *b)
{
	int n;

	if (token_buf)
		free(token_buf);

	n = (int) (b - a);

	token_buf = (char *) malloc(n + 1);

	if (token_buf == 0)
		stop("malloc failure");

	strncpy(token_buf, a, n);

	token_buf[n] = 0;
}

#if SELFTEST

static char *s[] = {

	"a^^b",
	"a^^ ? b\nStop: syntax error",

	"(a+b",
	"(a+b ? \nStop: ) expected",

	"quote(1/(x*log(a*x)))",	// test case A
	"1/(x*log(a*x))",

	"\"hello",
	"\"hello ? \nStop: runaway string",

#if 0 // does not work anymore because of "tensor expected" error

	// implicit mul cannot cross newline

	"a[b\nc]",
	"a[b ? \n] expected",

	"a[b*\nc]",
	"a[b*c]",

	"a[b\n*c]",
	"a[b*c]",

#endif

	// make sure question mark can appear after newlines

	"a+\nb+\nc+",
	"a+\nb+\nc+ ? \nStop: syntax error",

	// this bug fixed in version 30 (used to give one result, 14)

	"2+2\n(3+3)",
	"4\n6",

	// plus and minus cannot cross newline

	"1\n-1",
	"1\n-1",

	"1\n+1",
	"1\n1",
};

void
test_scan(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif

//	Notes:
//
//	Formerly add() and multiply() were used to construct expressions but
//	this preevaluation caused problems.
//
//	For example, suppose A has the floating point value inf.
//
//	Before, the expression A/A resulted in 1 because the scanner would
//	divide the symbols.
//
//	After removing add() and multiply(), A/A results in nan which is the
//	correct result.
//
//	The functions negate() and inverse() are used but they do not cause
//	problems with preevaluation of symbols.
