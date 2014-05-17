#include <stdio.h>
#include <locale.h>
struct lconv *localeconv(void){
	fputs("localeconv\n",stderr);
	return 0;
}
char* setlocale (int category, const char* locale){
	fprintf(stderr,"setlocale not supported %d %s\n",category,locale);
	return 0;
}
