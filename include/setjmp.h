#ifndef _FXCG_SETJMP_H
#define _FXCG_SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

// r8 - r15, pr, gbr
typedef unsigned jmp_buf[10];

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#ifdef __cplusplus
}
#endif

#endif
