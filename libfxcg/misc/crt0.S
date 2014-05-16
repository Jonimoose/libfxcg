! C runtime initialization for Prizm addins
! By Tari and calc84maniac, based on Kristaba's reverse-engineered crt0.
 
    .extern _main
    .global initialize
   
    .section ".pretext"
    .align 2
initialize:
! Preserve things on the stack
    mov.l r14, @-r15    ! Frame pointer
    sts.l pr, @-r15     ! Return address
    mov.l r4, @-r15     ! Parameter 1
   
! Copy .data section into RAM
    mov.l v_datald, r0      ! From
    mov.l v_sdata, r2       ! To
    mov.l v_edata, r3       ! Limit
dataLoop:
    cmp/hs r3, r2
    bt dataDone             ! Stop when r2 >= r3
    mov.l @r0+, r1
    mov.l r1, @r2
    bra dataLoop
    add #4, r2              ! Delay slot
dataDone:
 
! Zero out .bss
    mov.l v_ram_ebss, r2    ! To
    mov.l v_ram_bbss, r3    ! Limit
    mov #0, r1              ! Constant
bssLoop:
    cmp/hi r3, r2
    bf bssDone              ! Stop when r2 <= r3
    nop
    bra bssLoop
    mov.b r1, @-r2          ! Delay slot
bssDone:
   
! RAM is now initialized
 
    mov r5, r14             ! Save parameter 2
    mov #1, r6
    mov #0, r4
    bsr _GlibAddinAplExecutionCheck
    mov r6, r5
   
! main(r4, r5) with same state as input (returns to our caller)
   
    mov.l main, r7
    extu.w r14, r5
    mov.l @r15+, r5
    lds.l @r15+, pr
    jmp @r7
    mov.l @r15+, r14        ! Delay slot
   
_GlibAddinAplExecutionCheck:
    mov.l v_syscall, r2
    jmp @r2                 ! _GlibAddinAplExecutionCheck
    mov #0x29, r0           ! Delay slot
   
! Constants
    .align 4
main:
    .long _main
v_syscall:
    .long 0x80020070
 
v_datald:
    .long _datald
v_edata:
    .long _edata
v_sdata:
    .long _sdata
   
v_ram_bbss:
    .long __bss_start__
v_ram_ebss:
    .long __bss_end__
 
