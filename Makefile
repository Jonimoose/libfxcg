.PHONY: all clean
all:
	make -C libfxcg
	make -C libc

clean:
	make -C libfxcg clean
	make -C libc clean
