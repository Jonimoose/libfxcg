// function for returning random numbers. added by gbl08ma
#include "stdafx.h"
#include "defs.h"

#include <fxcg/rtc.h>

void
eval_random(void)
{
	randomnum();
}

void
randomnum(void)
{
	save();
    
        unsigned int rv = ( 0x41C64E6D*RTC_GetTicks() ) + 0x3039;
	push_integer(rv >> 16);

	restore();
}