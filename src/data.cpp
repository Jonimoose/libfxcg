#include "stdafx.h"
#include "defs.h"

int endian = 1;

U *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;

U *zero, *one, *imaginaryunit;

U symtab[NSYM], *binding[NSYM], *arglist[NSYM];

int expanding;
int verbosing;
int esc_flag;
int test_flag;
int draw_flag;
int trigmode;
char logbuf[256];
