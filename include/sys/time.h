#ifndef _SYS_TIME_H
#define _SYS_TIME_H 1
typedef unsigned int time_t;
typedef long suseconds_t;

struct timezone {
  int tz_minuteswest;	/* minutes west of Greenwich */
  int tz_dsttime;	/* type of dst correction */
};
struct timeval {
  time_t tv_sec;	/* seconds */
  suseconds_t tv_usec;	/* microseconds */
};
int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif
