//#define SELFTEST 0

// size of the symbol table

#define NSYM 500

// Symbolic expressions are built by connecting U structs.
//
// For example, (a b + c) is built like this:
//
//           _______      _______                                _______
//          |CONS   |--->|CONS   |----------------------------->|CONS   |
//          |       |    |       |                              |       |
//          |_______|    |_______|                              |_______|
//              |            |                                      |
//           ___v___      ___v___      _______      _______      ___v___
//          |ADD    |    |CONS   |--->|CONS   |--->|CONS   |    |SYM c  |
//          |       |    |       |    |       |    |       |    |       |
//          |_______|    |_______|    |_______|    |_______|    |_______|
//                           |            |            |
//                        ___v___      ___v___      ___v___
//                       |MUL    |    |SYM a  |    |SYM b  |
//                       |       |    |       |    |       |
//                       |_______|    |_______|    |_______|

typedef struct U {
	union {
		struct {
			struct U *car;		// pointing down
			struct U *cdr;		// pointing right
		} cons;
		char *printname;
		char *str;
		struct tensor *tensor;
		struct {
			unsigned int *a, *b;	// rational number a over b
		} q;
		double d;
	} u;
	unsigned char k, tag;
} U;

// the following enum is for struct U, member k

enum {
	CONS,
	NUM,
	DOUBLE,
	STR,
	TENSOR,
	SYM,
};

// the following enum is for indexing the symbol table

enum {

	// standard functions first, then nil, then everything else

	ABS,
	ADD,
	ADJ,
	AND,
	ARCCOS,
	ARCCOSH,
	ARCSIN,
	ARCSINH,
	ARCTAN,
	ARCTANH,
	ARG,
	ATOMIZE,
	BESSELJ,
	BESSELY,
	BINDING,
	BINOMIAL,
	CEILING,
	CHECK,
	CHOOSE,
	CIRCEXP,
	CLEAR,
	CLOCK,
	COEFF,
	COFACTOR,
	CONDENSE,
	CONJ,
	CONTRACT,
	COS,
	COSH,
	DECOMP,
	DEFINT,
	DEGREE,
	DENOMINATOR,
	DERIVATIVE,
	DET,
	DIM,
	DIRAC,
	DISPLAY,
	DIVISORS,
	DO,
	DOT,
	DRAW,
	DSOLVE,
	EIGEN,
	EIGENVAL,
	EIGENVEC,
	ERF,
	ERFC,
	EVAL,
	EXP,
	EXPAND,
	EXPCOS,
	EXPSIN,
	FACTOR,
	FACTORIAL,
	FACTORPOLY,
	FILTER,
	FLOATF,
	FLOOR,
	FOR,
	GAMMA,
	GCD,
	HERMITE,
	HILBERT,
	IMAG,
	INDEX,
	INNER,
	INTEGRAL,
	INV,
	INVG,
	ISINTEGER,
	ISPRIME,
	LAGUERRE,
//	LAPLACE,
	LCM,
	LEADING,
	LEGENDRE,
	LOG,
	MAG,
	MOD,
	MULTIPLY,
	NOT,
	NROOTS,
	NUMBER,
	NUMERATOR,
	OPERATOR,
	OR,
	OUTER,
	POLAR,
	POWER,
	PRIME,
	PRINT,
	PRODUCT,
	QUOTE,
	QUOTIENT,
        RANDOM, // by gbl08ma
	RANK,
	RATIONALIZE,
	REAL,
	YYRECT,
	ROOTS,
	SETQ,
	SGN,
	SIMPLIFY,
	SIN,
	SINH,
	SQRT,
	STOP,
	SUBST,
	SUM,
	TAN,
	TANH,
	TAYLOR,
	TEST,
	TESTEQ,
	TESTGE,
	TESTGT,
	TESTLE,
	TESTLT,
	TRANSPOSE,
	UNIT,
	ZERO,

	NIL,	// nil goes here, after standard functions

	AUTOEXPAND,
	BAKE,
	LAST,
	TRACE,
	TTY,

	YYE,

	DRAWX,		// special purpose internal symbols
	METAA,
	METAB,
	METAX,
	SECRETX,

	PI,
	SYMBOL_A,
	SYMBOL_B,
	SYMBOL_C,
	SYMBOL_D,
	SYMBOL_I,
	SYMBOL_J,
	SYMBOL_N,
	SYMBOL_R,
	SYMBOL_S,
	SYMBOL_T,
	SYMBOL_X,
	SYMBOL_Y,
	SYMBOL_Z,

	C1,
	C2,
	C3,
	C4,
	C5,
	C6,

	USR_SYMBOLS,	// this must be last
};

#define E YYE

// TOS cannot be arbitrarily large because the OS seg faults on deep recursion.
// For example, a circular evaluation like x=x+1 can cause a seg fault.
// At this setting (100,000) the evaluation stack overruns before seg fault.

#define TOS 500

#define BUF 10000

#define MAX_PROGRAM_SIZE 100001
#define MAXPRIMETAB 10000

#define _USE_MATH_DEFINES // for MS C++

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <fcntl.h>
#include <string.h>
#include "setjmp.h"
#include <math.h>
#include <errno.h>
#include <asm.h>

#define MAXDIM 24

typedef struct tensor {
	int ndim;
	int dim[MAXDIM];
	int nelem;
	U *elem[1];
} T;

struct display {
	int h, w, n;
	struct {
		int c, x, y;
	} a[1]; // a for array
};

struct text_metric {
	int ascent, descent, width;
};

#define symbol(x) (symtab + (x))
#define iscons(p) ((p)->k == CONS)
#define isrational(p) ((p)->k == NUM)
#define isdouble(p) ((p)->k == DOUBLE)
#define isnum(p) (isrational(p) || isdouble(p))
#define isstr(p) ((p)->k == STR)
#define istensor(p) ((p)->k == TENSOR)
#define issymbol(p) ((p)->k == SYM)
#define iskeyword(p) (issymbol(p) && symnum(p) < NIL)

#define car(p) (iscons(p) ? (p)->u.cons.car : symbol(NIL))
#define cdr(p) (iscons(p) ? (p)->u.cons.cdr : symbol(NIL))
#define caar(p) car(car(p))
#define cadr(p) car(cdr(p))
#define cdar(p) cdr(car(p))
#define cddr(p) cdr(cdr(p))
#define caadr(p) car(car(cdr(p)))
#define caddr(p) car(cdr(cdr(p)))
#define cadar(p) car(cdr(car(p)))
#define cdadr(p) cdr(car(cdr(p)))
#define cddar(p) cdr(cdr(car(p)))
#define cdddr(p) cdr(cdr(cdr(p)))
#define caaddr(p) car(car(cdr(cdr(p))))
#define cadadr(p) car(cdr(car(cdr(p))))
#define caddar(p) car(cdr(cdr(car(p))))
#define cdaddr(p) cdr(car(cdr(cdr(p))))
#define cadddr(p) car(cdr(cdr(cdr(p))))
#define cddddr(p) cdr(cdr(cdr(cdr(p))))
#define caddddr(p) car(cdr(cdr(cdr(cdr(p)))))
#define cadaddr(p) car(cdr(car(cdr(cdr(p)))))
#define cddaddr(p) cdr(cdr(car(cdr(cdr(p)))))
#define caddadr(p) car(cdr(cdr(car(cdr(p)))))
#define cdddaddr(p) cdr(cdr(cdr(car(cdr(cdr(p))))))
#define caddaddr(p) car(cdr(cdr(car(cdr(cdr(p))))))

#define isadd(p) (car(p) == symbol(ADD))
#define ispower(p) (car(p) == symbol(POWER))
#define isfactorial(p) (car(p) == symbol(FACTORIAL))

#define MSIGN(p) (((int *) (p))[-2])
#define MLENGTH(p) (((int *) (p))[-1])

#define MZERO(p) (MLENGTH(p) == 1 && (p)[0] == 0)
#define MEQUAL(p, n) (MLENGTH(p) == 1 && (long long) MSIGN(p) * (p)[0] == (n))

extern int tos;
extern int expanding;
extern int fmt_x;
extern int fmt_index;
extern int fmt_level;
extern int verbosing;
extern int primetab[MAXPRIMETAB];
extern int esc_flag;
extern int draw_flag;
extern int mtotal;
extern int trigmode;
extern char logbuf[];
extern char program_buf[];
extern U symtab[];
extern U *binding[];
extern U *arglist[];
extern U *stack[];
extern U **frame;
extern U *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
extern U *zero, *one, *imaginaryunit;
extern U symtab[];
extern char out_buf[];
extern int out_count;
extern int test_flag;
extern jmp_buf draw_stop_return;
extern int endian;
#define little_endian() (*((unsigned char *) &endian))

#include "prototypes.h"
