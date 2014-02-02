#include "stdafx.h"
#include "defs.h"

jmp_buf stop_return, draw_stop_return;

void
stop(char *s)
{
	if (draw_flag == 2)
		longjmp(draw_stop_return, 1);
	else {
		printstr("Stop: ");
		printstr(s);
		printstr("\n");
		longjmp(stop_return, 1);
	}
}

void
run(char *s)
{
	int i, n;

	/*if (strncmp(s, "selftest", 8) == 0) {
		selftest();
		return;
	}*/

	if (setjmp(stop_return))
		return;

	init();

	while (1) {

		n = scan(s);

		p1 = pop();
		check_stack();

		if (n == 0)
			break;

		// if debug mode then print the source text

		if (equaln(get_binding(symbol(TRACE)), 1)) {
			for (i = 0; i < n; i++)
				if (s[i] != '\r')
					printchar(s[i]);
			if (s[n - 1] != '\n') // n is not zero, see above
				printchar('\n');
		}

		s += n;

		push(p1);
		top_level_eval();

		p2 = pop();
		check_stack();

		if (p2 == symbol(NIL))
			continue;

		// print string w/o quotes

		if (isstr(p2)) {
			printstr(p2->u.str);
			printstr("\n");
			continue;
		}

		if (equaln(get_binding(symbol(TTY)), 1) || test_flag) // tty mode?
			printline(p2);
		else {
//#ifdef LINUX
			display(p2);
/*#else
			push(p2);
			cmdisplay();
#endif*/
		}
	}
}

void
check_stack(void)
{
	if (tos != 0)
		stop("stack error");
	if (frame != stack + TOS)
		stop("frame error");
}

// cannot reference symbols yet

void
echo_input(char *s)
{
	printstr(s);
	printstr("\n");
}

// returns nil on stack if no result to print

void
top_level_eval(void)
{
	save();

	trigmode = 0;

	p1 = symbol(AUTOEXPAND);

	if (iszero(get_binding(p1)))
		expanding = 0;
	else
		expanding = 1;

	p1 = pop();
	push(p1);
	eval();
	p2 = pop();

	// "draw", "for" and "setq" return "nil", there is no result to print

	if (p2 == symbol(NIL)) {
		push(p2);
		restore();
		return;
	}

	// update "last"

	set_binding(symbol(LAST), p2);

	if (!iszero(get_binding(symbol(BAKE)))) {
		push(p2);
		bake();
		p2 = pop();
	}

	// If we evaluated the symbol "i" or "j" and the result was sqrt(-1)

	// then don't do anything.

	// Otherwise if "j" is an imaginary unit then subst.

	// Otherwise if "i" is an imaginary unit then subst.

	if ((p1 == symbol(SYMBOL_I) || p1 == symbol(SYMBOL_J))
	&& isimaginaryunit(p2))
		;
	else if (isimaginaryunit(get_binding(symbol(SYMBOL_J)))) {
		push(p2);
		push(imaginaryunit);
		push_symbol(SYMBOL_J);
		subst();
		p2 = pop();
	} else if (isimaginaryunit(get_binding(symbol(SYMBOL_I)))) {
		push(p2);
		push(imaginaryunit);
		push_symbol(SYMBOL_I);
		subst();
		p2 = pop();
	}

#ifndef LINUX

	// if we evaluated the symbol "a" and got "b" then print "a=b"

	// do not print "a=a"

	if (issymbol(p1) && !iskeyword(p1) && p1 != p2 && test_flag == 0) {
		push_symbol(SETQ);
		push(p1);
		push(p2);
		list(3);
		p2 = pop();
	}
#endif
	push(p2);

	restore();
}

void
check_esc_flag(void)
{
	if (esc_flag)
		stop("esc key");
}
