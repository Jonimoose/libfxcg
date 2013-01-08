#ifndef _FXCG_TIME_H
#define _FXCG_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned time_t;

typedef int clock_t;
#define CLOCKS_PER_SEC 128
clock_t clock(void);

#ifdef __cplusplus
}
#endif

#endif
