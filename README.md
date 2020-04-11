# libfxcg

![Latest release](https://img.shields.io/github/v/release/Jonimoose/libfxcg)

Syscall and helper library for the Casio FX-CG (Prizm) color graphing
calculator.

# Usage

You can download precompiled packages from the
[releases](https://github.com/Jonimoose/libfxcg/releases/latest) page. If
you're using Windows you can get a precompiled SDK package that includes
everything you need to get started. See the [Windows
HOWTO](docs/howto-windows.md) document for a walkthrough.

## SDK packages

The SDK packages provide everything you need to build programs, including a
compiler. You should use these unless you know you want to do something
different.

Place your project(s) in the `projects` directory underneath the unpacked
release. You may wish to copy or modify the included `example` project to
get set up quickly. Running `make` from the project directory will suffice to
build a ready-to-run g3a.

## Library archives

For other platforms (non-Windows) where we don't currently provide SDK packages,
the library archive releases provide the compiled libraries (`.a`) and
corresponding include files to use with your compiler and build system.

### Add-in build requirements

It is infeasible to document how to set up your compiler on many platforms, but
the basic requirement is that you have a version of GCC built to target
`sh3eb-elf-none`.

When building code, you'll need to use the `-mb`, `-m4a-nofpu`, `-mhitachi` and
`-nostdlib` flags to GCC to build code in big-endian mode for SH4a using the
Hitachi calling convention, and skipping any C library or startup code that may
come with your compiler.

When linking, you'll need to explicitly link `libgcc` and `libfxcg` at least,
and optionally also `libc` for utility functions. `libgcc` provides support
functions that the compiler requires, and `libfxcg` includes all of the
Prizm-specific items, including startup code necessary to start up a program
correctly.

When linking a final binary, you will also need to specify an appropriate linker
script (`-Tscript.x`). You may wish to use or adapt [the one we
provide](toolchain/prizm.x).

# Examples and more documentation.

A few example programs can be found in the `examples` directory.

More detailed documentation can be found on
[WikiPrizm](http://prizm.cemetech.net/).
