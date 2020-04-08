libfxcg
=======
Syscall and helper library for the Casio FX-CG (Prizm) color graphing calculator

Not much to see here except the syscalls, hopefully that will change in the future.

Building
--------
1. Change `TOOLCHAIN_PREFIX` in `libfxcg/Makefile` and `libc/Makefile` to the prefix of your toolchain.  The default is `sh3eb-elf-`.
2. make

### Examples
In a similar way, to build the cmake example, you must change the cmake variable `TOOLCHAIN_PREFIX` either with ccmake or `cmake -DTOOLCHAIN_PREFIX:STRING=prefix-`.
