OUTPUT_ARCH(sh3)
 
/* Entry point.  Not really important here, since doing binary output */
ENTRY(initialize)
 
MEMORY
{
        /* Loads code at 300000, skips g3a header */
        rom (rx) : o = 0x00300000, l = 1024k
        ram (rwx) : o = 0x08100000, l = 512k  /* According to Simon docs */
        ilram (rwx) : o = 0xE5200000, l = 4k /* According to http://www.cemetech.net/forum/viewtopic.php?t=9334 */
}
 
SECTIONS
{
        /* Code, in ROM */
        .text : {
                *(.pretext)     /* init stuff */
                *(.text)
                *(.text.*)
        } > rom

        .ilram : {
                _ilramld = LOADADDR(.data) ;
                _silram = . ;
                *(.ilram)
                *(.ilram.*)
                _eilram = . ;
        } > ilram AT> rom
       
        /* Read-only data, in ROM */
        .rodata : {
                *(.rodata)
                *(.rodata.*)
        } > rom
       
        /* RW initialized data, VMA in RAM but LMA in ROM */
        .data : {
                _datald = LOADADDR(.data) ;
                _sdata = . ;
                *(.data)
                *(.data.*);
                _edata = . ;
        } >ram AT>rom
       
        /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >ram
}
