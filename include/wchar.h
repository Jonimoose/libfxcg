#include <stddef.h>
#ifndef wchar_t
typedef unsigned char wchar_t;
#endif
wchar_t* wcsncpy (wchar_t* dst, const wchar_t* src, size_t num);
