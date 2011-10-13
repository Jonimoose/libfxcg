	.global _strcat
	.text; .align 4
_strcat:
		mov	r4, r6
		bra	X87D4
		mov	r5, r7

X87D2:
		add	#1, r6

X87D4:
		mov.b	@r6, r2
		tst	r2, r2
		bf	X87D2

X87DA:
		mov.b	@r7+, r2
		mov.b	r2, @r6
		extu.b	r2, r2
		tst	r2, r2
		bf/s	X87DA
		add	#1, r6
		rts
		mov	r4, r0
	.END	
