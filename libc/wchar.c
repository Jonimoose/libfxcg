#include <wchar.h>
wchar_t* wcsncpy (wchar_t* dst, const wchar_t* src, size_t num){
	wchar_t *d=dst;
	while(num--){
		if(*src)
			*d++=*src++;
		else{
			while(num--)
				*d++=0;
			break;
		}
	}
	return dst;
}
