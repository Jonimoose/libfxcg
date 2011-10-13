	.global _strncpy
	.text; .align 4
_strncpy:
		mov	r4, r7
		mov	r5, r0
		mov	#0, r1
		bra	X88CA
		mov	r1, r5
X88C2:
		add	#1, r5
		mov.b	@r0+, r3
		mov.b	r3, @r7
		add	#1, r7

X88CA:
		cmp/hs	r6, r5
		bt	X88E2
		mov.b	@r0, r3
		tst	r3, r3
		bf	X88C2
		cmp/hs	r6, r5
		bt	X88E2

X88D8:
		mov.b	r1, @r7
		add	#1, r5
		cmp/hs	r6, r5
		bf/s	X88D8
		add	#1, r7

X88E2:
		rts
		mov	r4, r0
	.END
