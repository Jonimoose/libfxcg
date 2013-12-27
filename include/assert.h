#ifndef __FXCG_ASSERT_H
#define __FXCG_ASSERT_H

#ifdef NDEBUG
void assert(int ex);
#else
//#error assert() not with DEBUG.
#define assert(x)
#endif

#endif
