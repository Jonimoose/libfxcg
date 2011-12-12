	.global _sys_strcpy
	.text;.align 4
_sys_strcpy:
		mov	r5, r1
		mov	r4, r0
		mov.l	r3, @-r15
		mov.l	r4, @-r15
		mov	#3, r3
		mov	r0, r4
		and	r3, r4
		and	r1, r3
		or	r4, r3
		tst	r3, r3
		bt	XLONG
		mov	r0, r4
		mov.b	@r1+, r0
		mov	r4, r3

X8100:
		cmp/eq	#0, r0
		mov.b	r0, @r3
		bt	X8124
		mov.b	@r1+, r0
		cmp/eq	#0, r0
		mov.b	r0, @(1,r3)
		bt	X8124
		mov.b	@r1+, r0
		cmp/eq	#0, r0
		mov.b	r0, @(2,r3)
		bt	X8124
		mov.b	@r1+, r0
		cmp/eq	#0, r0
		mov.b	r0, @(3,r3)
		bt	X8124
		mov.b	@r1+, r0
		bra	X8100
		add	#4, r3
X8124:
		mov	r4, r0
		mov.l	@r15+, r4
		rts
		mov.l	@r15+, r3
XLONG:
		mov.l	r5, @-r15
		mov.l	r6, @-r15
		mov	r0, r3
		mov.l	@r1+, r4
		mov	#0, r6

X8056:
		cmp/str	r4, r6
		bt	X8084
		mov.l	@r1+, r5
		mov.l	r4, @r0
		cmp/str	r5, r6
		bt	X807A
		mov.l	@r1+, r4
		mov.l	r5, @(4,r0)
		cmp/str	r4, r6
		bt	X807E
		mov.l	@r1+, r5
		mov.l	r4, @(8,r0)
		cmp/str	r5, r6
		bt	X8082
		mov.l	@r1+, r4
		mov.l	r5, @(0x0C,r0)
		bra	X8056
		add	#0x10, r0
X807A:
		bra	X8084
		add	#4, r0
X807E:
		bra	X8084
		add	#8, r0
X8082:
		add	#0xC, r0
X8084:
		add	#-4, r1
		mov	r0, r6
		mov.b	@r1+, r0
		cmp/eq	#0, r0
		mov.b	r0, @r6
		bt	X80A4
		mov.b	@r1+, r0
		cmp/eq	#0, r0
		mov.b	r0, @(1,r6)
		bt	X80A4
		mov.b	@r1+, r0
		cmp/eq	#0, r0
		mov.b	r0, @(2,r6)
		bt	X80A4
		mov.b	@r1+, r0
		mov.b	r0, @(3,r6)

X80A4:
		mov.l	@r15+, r6
		mov	r3, r0
		mov.l	@r15+, r5
		mov.l	@r15+, r4
		rts
		mov.l	@r15+, r3
	.END
