    .global _memsetZero
    .text;.align 4
_memsetZero:
        mov r4,r0
        xor r1,r1
Loop:
        mov.l r1,@(r0,r5)
        tst r5,r5
        bt/s Loop
        add #-4,r5
        rts

