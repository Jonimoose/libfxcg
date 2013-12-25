#include <stdio.h>
#include <locale.h>
struct lconv *localeconv(void){
	fputs("localeconv\n",stderr);
	return 0;
}
