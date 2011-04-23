! This is the initialization routine for Prizm Add-in
! Written by Kristaba using the conv.g3a addin data (from OS 1.0 or 1.02)


	.extern _main
	.global initialize

	.section ".pretext"
	.align 2
initialize:  ! 0x300000 with virtual address mapping
	mov.l	r14, @-r15
	sts.l	pr, @-r15
	mov.l	r4, @-r15
	bsr	ram_init
	mov	r5, r14

	mov	#1, r6
	mov	#0, r4
	bsr	_GLibAddinAplExecutionCheck	! (0, 1, 1) so I think it's this syscall
	mov	r6, r5

	mov.l	main, r7
	extu.w	r14, r5
	mov.l	@r15+, r4
	lds.l	@r15+, pr
	jmp	@r7		! jmp _main(r4, r5); using given arguments of initialize routine
	mov.l	@r15+, r14


	.align 4
main:
	.long	_main


	! Copy .data in ram and fill .bss with 0x00
ram_init:
	mov	#0, r6
	bra	LoopConditionBss
	mov.l	Something1, r2

CopyByteBss:
	mov.l	r6, @r2		! 0 -> @(0x08100004 + 4xloop)
	add	#4, r2
LoopConditionBss:
	mov.l	Something2, r5
	cmp/hs	r5, r2
	bf	CopyByteBss

	mov.l	Something3, r6
	bra	LoopConditionData
	mov.l	Something4, r5
CopyByteData:
	mov.l	@r5+, r2	! @(rom_data + 4*loop) -> r2
	mov.l	r2, @r6		! r2 -> @(data_begin + 4*loop)
	add	#4, r6
LoopConditionData:
	mov.l	Something5, r4
	cmp/hs	r4, r6
	bf	CopyByteData

	mov.l	RamStart, r1	! first ram address
	mov.l	Something1, r5		
	sub	r5, r4		! @Something5 - @Something1 ->r4
	add	#4, r4
	rts
	mov.l	r4, @r1



	.align 4
RamStart:
	.long	0x08100000	! ram address




_GLibAddinAplExecutionCheck:
	mov.l	SysCall, r2
	mov	#0x29, r0
	jmp	@r2
	nop


	.align 4
SysCall:
	.long	0x80020070

Something1:	! was 3001e4
	.long	_bbss			! start bss address
Something2:	! was 3001e8
	.long	_ebss			! end bss address
Something3:	! was 3001ec
	.long	_bdata			! start data address
Something5:	! was 3001f0
	.long	_edata			! end data address
Something4:	! was 3001f4
	.long	_romdata		! address of data section in rom