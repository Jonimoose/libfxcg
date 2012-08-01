#ifndef _FXCG_STDDEF_H
#define _FXCG_STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

// Python requires that NULL be a macro, so humor it.
#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void*)0)
// We don't do unicode of any sort.
typedef unsigned char wchar_t;
#endif

#define offsetof(type, member) __builtin_offsetof(type, member)

#ifndef size_t
typedef unsigned size_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
