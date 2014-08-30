#include <time.h>
#include <fxcg/rtc.h>

clock_t clock() {
    return RTC_GetTicks();
}
