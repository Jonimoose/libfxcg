    .global _sys_strcmp
    .text;.align 4
_sys_strcmp:
        mov r4, r0
        or  r5, r0
        tst #3, r0
        bf  XBYTE

        mov #0, r2
        mov.l   @r5+, r3
        mov.l   @r4+, r0

X8146:
        mov.l   @r4+, r6
        cmp/str r2, r3
        bt  X8198
        cmp/eq  r0, r3
        mov.l   @r5+, r1
        bf  X8192
        mov.l   @r4+, r0
        cmp/str r2, r1
        bt  X8198
        cmp/eq  r6, r1
        mov.l   @r5+, r3
        bf  X818E
        mov.l   @r4+, r6
        cmp/str r2, r3
        bt  X8198
        cmp/eq  r0, r3
        mov.l   @r5+, r1
        bf  X8192
        mov.l   @r4+, r0
        cmp/str r2, r1
        bt  X8198
        cmp/eq  r6, r1
        mov.l   @r5+, r3
        bf  X818E
        mov.l   @r4+, r6
        cmp/str r2, r3
        bt  X8198
        cmp/eq  r0, r3
        mov.l   @r5+, r1
        bf  X8192
        mov.l   @r4+, r0
        cmp/str r2, r1
        bt  X8198
        cmp/eq  r6, r1
        mov.l   @r5+, r3
        bt  X8146

X818E:
        mov r6, r0
        mov r1, r3

X8192:
        rts
        sub r3, r0

X8198:
        add #-4, r5
        add #-8, r4

XBYTE:
        mov #0, r2
        mov.b   @r5+, r3
        mov.b   @r4+, r0

X81A2:
        mov.b   @r4+, r6
        cmp/eq  #0, r0
        bt  X81EE
        cmp/eq  r0, r3
        mov.b   @r5+, r1
        bf  X81EE
        mov.b   @r4+, r0
        cmp/eq  r2, r6
        bt  X81EA
        cmp/eq  r6, r1
        mov.b   @r5+, r3
        bf  X81EA
        mov.b   @r4+, r6
        cmp/eq  #0, r0
        bt  X81EE
        cmp/eq  r0, r3
        mov.b   @r5+, r1
        bf  X81EE
        mov.b   @r4+, r0
        cmp/eq  r2, r6
        bt  X81EA
        cmp/eq  r6, r1
        mov.b   @r5+, r3
        bf  X81EA
        mov.b   @r4+, r6
        cmp/eq  #0, r0
        bt  X81EE
        cmp/eq  r0, r3
        mov.b   @r5+, r1
        bf  X81EE
        mov.b   @r4+, r0
        cmp/eq  r2, r6
        bt  X81EA
        cmp/eq  r6, r1
        mov.b   @r5+, r3
        bt  X81A2

X81EA:
        mov r6, r0
        mov r1, r3

X81EE:
        rts
        sub r3, r0

        .END
