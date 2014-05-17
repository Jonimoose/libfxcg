#include <fxcg/heap.h>
#include <fxcg/display.h>
#include <fxcg/misc.h>
#include <fxcg/keyboard.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
// External linkage
int errno;

int putenv(char *string){
	fprintf(stderr,"putenv %s\n",string);
	return -1;
}
const char * FatalErrorStr="alloc error\n";
void *malloc(size_t sz) {
	void *ret=sys_malloc(sz);
    if(!ret){
		fprintf(stderr,"m%s",FatalErrorStr);
	}
	return ret;
}

void* calloc (size_t num, size_t size){
	void * ret=sys_malloc(num*size);
	if(ret)
		memset(ret,0,num*size);
	return ret;
}

void *realloc(void *ptr, size_t sz) {
	void * ret=sys_realloc(ptr, sz);
	 if(!ret){
		fprintf(stderr,"re%s",FatalErrorStr);
	}
    return ret;
}

void free(void *ptr){
    sys_free(ptr);
}
void _exit(int stat){
	fprintf(stderr, "TERMINATED (%i)\n", stat);
    puts("To exit press the menu key");
    puts("Before running this program again please launch another program before running this one again");
    int key;
	while(1)
		GetKey(&key);
}

#if __WORDSIZE == 64
#define ABS_LONG_MIN 9223372036854775808UL
#else
#define ABS_LONG_MIN 2147483648UL
#endif
long int strtol(const char *nptr, char **endptr, int base)//From dietlibc
{
  int neg=0;
  unsigned long int v;
  const char*orig=nptr;

  while(isspace(*nptr)) nptr++;

  if (*nptr == '-' && isalnum(nptr[1])) { neg=-1; ++nptr; }
  v=strtoul(nptr,endptr,base);
  if (endptr && *endptr==nptr) *endptr=(char *)orig;
  if (v>=ABS_LONG_MIN) {
    if (v==ABS_LONG_MIN && neg) {
      errno=0;
      return v;
    }
    errno=ERANGE;
    return (neg?LONG_MIN:LONG_MAX);
  }
  return (neg?-v:v);
}


double strtod(const char *s, char **str_end) {
    // TODO handle exponential format, hex format, inf, nan
    double r = 0.0;
    int negative = 0;
    if (!isdigit(*s) && *s != '-' && *s != '+' && *s != '.') {
        if (str_end != NULL)
            *str_end = (char *)s;
        return 0;
    }

    switch (*s)
    {
        case '-':
            negative = 1;
            // Deliberate fallthrough
        case '+':
            s++;
            break;
    }

    while (isdigit(*s))
    {
        r *= 10.0;
        r += *s++ - '0';
    }
    if (*s == '.')
    {
        float f = 10.0f;
        s++;
        while (isdigit(*s))
        {
            r += (*s - '0')/f;
            f *= 10.0f;
            s++;
        }
    }

    if (str_end != NULL)
        *str_end = (char *)s;

    // Portable? Nope. Fast? Yup.
    union {
        double r;
        unsigned long long l;
    } raw;
    raw.r = r;
    raw.l |= (unsigned long long)negative << 63;
    return raw.r;
}

int abs(int i) {
    return i<0?-i:i;
}
const char * XtermConstStr="xterm";
const char * FxcgTermLines="64";
const char * FxcgTermColums="24";
int getenv(const char * name){
	if(strcmp(name,"LINES")==0)
		return FxcgTermLines;
	else if(strcmp(name,"COLUMNS")==0)
		return FxcgTermColums;
	else if(strcmp(name,"TERMINFO")==0)
		return XtermConstStr;
	else if(strcmp(name,"TERM")==0)
		return XtermConstStr;
	fprintf(stderr,"getenv %s\n",name);
	return 0;
}
void abort(void){
	int x=0;
	int y=0;
	PrintMini(&x,&y,"Abort called press MENU",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
	while (1)
		GetKey(&x);
}
int system (const char* command){
	fprintf(stderr,"system not yet supported %s\n",command);
	return -1;
}
