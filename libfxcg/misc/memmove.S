	.global _sys_memmove
	.text;.align 4
_sys_memmove:
		cmp/eq	r5, r4
		bt	X8534	! if (P1==P2) return P1
		cmp/hs	r5, r4		! P1>=P2?
		bt	X8518
		mov	r5, r0		! r0=P2
		mov	#0, r5
		mov	r5, r3		! r3=0
		cmp/hs	r6, r3
		bt/s	X8534	! if (P3<=0) return P1
		mov	r4, r7		! r7=P1

X8508:
		mov.b	@r0+, r3	! r3=[P2].b
		add	#1, r5
		mov.b	r3, @r7		! [P1].b=r3
		cmp/hs	r6, r5		! count	in P3 reached?
		bf/s	X8508
		add	#1, r7
		bra	X8534
		nop
X8518:
		mov	r4, r7		! r7=P1
		add	r6, r7		! increase P1 by count P3
		mov	r5, r0		! r0=P2
		mov	#0, r5
		mov	r5, r2
		cmp/hs	r6, r2
		bt/s	X8534	! count==0?
		add	r6, r0		! increase P2 by count P3

X8528:
		add	#-1, r0
		add	#1, r5
		mov.b	@r0, r3		! r3=[P2].b
		cmp/hs	r6, r5
		bf/s	X8528
		mov.b	r3, @-r7	! [P1].b=r3

X8534:
		rts
		mov	r4, r0
	.END
