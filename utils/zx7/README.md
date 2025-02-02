# zx7 utility

zx7 implementation by Einar Saukas allows for low impact decompression routines with very thorough offline compression. See the unmodified zx7.txt for more information.

Use the compress routine in an offline tool to generate your data, and then decompress it on Prizm devices by using the ZX7Decompress function.

Link to libzx7 by using -lzx7 in your project makefile, and include zx7/zx7.h to use.

The CLI folder contains the source and Makefile for a CLI app to compress and decompress files with zx7.
