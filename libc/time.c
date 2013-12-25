#include <time.h>
#include <fxcg/rtc.h>
#include <stdio.h>
clock_t clock() {
    return RTC_GetTicks();
}
time_t mktime (struct tm * timeptr){
	fputs("mktime\n",stderr);
}
