OUTPUT_ARCH(sh3)
ENTRY(initialize)
MEMORY
{
        rom  : o = 0x00300000, l = 512k
        ram  : o = 0x08100004, l = 64k  /* pretty safe guess */
}
SECTIONS
{
        .text : {
                *(.pretext)     /* init stuff */
                *(.text)
		*(.text.*)
        } > rom
        .rodata : {
                *(.rodata)
                *(.rodata.*)
                _romdata = . ;  /* symbol for initialization data */
        } > rom
        .bss : {
                _bbss = . ;
                *(.bss) *(COMMON);
                _ebss = . ;
        } > ram
        .data : AT(_romdata) {
                _bdata = . ;
                *(.data)
		*(.data.*);
                _edata = . ;
        } > ram
}