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
       
        /* Uninitialized data (fill with 0), in RAM */
        .bss : {
                _bbss = . ;
                *(.bss) *(COMMON);
                _ebss = . ;
        } >ram
}
