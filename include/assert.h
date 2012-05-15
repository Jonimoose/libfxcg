#ifndef _FXCG_ASSERT_H
#define _FXCG_ASSERT_H

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#error assert() not with DEBUG.
#endif

#endif
