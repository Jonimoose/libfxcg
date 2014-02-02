/*------------------------------------------------------*/
/* SH SERIES C Compiler Ver. 1.0                        */
/* Copyright (c) 1992 Hitachi,Ltd.                      */
/* Licensed material of Hitachi,Ltd.                    */
/*------------------------------------------------------*/
/***********************************************************************/
/* SPEC;                                                               */
/*  NAME = setjmp.h :                                ;                 */
/*                                                                     */
/*  FUNC = this module do the following functions    ;                 */
/*                                                                     */
/*  CLAS = UNIT;                                                       */
/*                                                                     */
/* END;                                                                */
/***********************************************************************/
#ifndef _SETJMP
#define _SETJMP

typedef int jmp_buf[38];
typedef int jmp_buf_a[54];

#ifdef __cplusplus
extern "C" {
#endif
extern int setjmp(jmp_buf);
extern void longjmp(jmp_buf, int);
extern int setjmp_a(jmp_buf);
extern void longjmp_a(jmp_buf, int);

extern volatile int _errno;

#ifdef __cplusplus
}
#endif

#ifndef SEQERR
#define SEQERR     1108
#endif

#endif
