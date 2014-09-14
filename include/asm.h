#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/* template for generic functions with global name */
/*#define _C_LABEL(x)   _ ## x*/    /* prefix fn name with _ */
#define _C_LABEL(x) x
#define _ENTRY(name)    \
    .text; .align 2; .global name; name:
#define ENTRY(name) \
    _ENTRY(_C_LABEL(name))

/* template for syscalls */
#define SYSCALL_BARE(name, x)   \
ENTRY(name)         \
    mov.l   sc_addr, r2 ;   \
    mov.l   1f, r0 ;    \
    jmp @r2 ;       \
    nop ;           \
1:  .long x

/* allow to have several calls in one object, by specifying
 * the SYSCALL_ADDRESS separately at the end */
#define SYSCALL_ADDRESS \
sc_addr:    .long 0x80020070

#define SYSCALL(name, x)    \
    SYSCALL_BARE(name, x) ; \
    SYSCALL_ADDRESS

#endif