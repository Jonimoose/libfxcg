#ifndef __FXCG_ASSERT_H
#define __FXCG_ASSERT_H

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
//#error assert() not with DEBUG.
#endif

#endif
