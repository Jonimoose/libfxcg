	.global _strchr
	.text;.align 4
_strchr:
		mov	r4, r6
		bra	X87FC
		exts.b	r5, r4
X87F2:
		tst	r2, r2
		bf	X87FA
		rts
		mov	#0, r0
X87FA:
		add	#1, r6
X87FC:
		mov.b	@r6, r2
		cmp/eq	r4, r2
		bf	X87F2
		mov	r6, r0
		rts
		nop
	.END

