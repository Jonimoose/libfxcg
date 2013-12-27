#ifndef _FXCG_TIME_H
#define _FXCG_TIME_H 1
#include <sys/time.h>
#ifdef __cplusplus
extern "C" {
#endif
#define __time_t_defined
#ifndef time_t
typedef unsigned int time_t;
#endif
struct tm
{
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
};
typedef int clock_t;
#define CLOCKS_PER_SEC 128
clock_t clock(void);
time_t mktime (struct tm * timeptr);
time_t time (time_t* timer);
#ifdef __cplusplus
}
#endif

#endif
