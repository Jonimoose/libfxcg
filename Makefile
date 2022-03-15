
all: lib/libfxcg.a lib/libc.a

lib:
	mkdir -p lib

lib/libfxcg.a: lib
	make -C libfxcg

lib/libc.a: lib
	make -C libc

clean:
	rm lib/*.a
	rm libc/*.o
	rm libfxcg/misc/*.o
	rm libfxcg/syscalls/*.o
