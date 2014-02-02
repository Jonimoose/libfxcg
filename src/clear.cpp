#include "stdafx.h"
#include "defs.h"
int run_startup_script_again = 0;
void
eval_clear(void)
{
	if (test_flag == 0)
		clear_term();
	clear_symbols();
	defn();
	push(symbol(NIL));
        run_startup_script_again = 1;
}

// clear from application GUI code

void
clear(void)
{
	run("clear");
}
