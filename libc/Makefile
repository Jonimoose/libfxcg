CC=sh4aeb-nofpu-elf-gcc
AS=sh4aeb-nofpu-elf-as
AR=sh4aeb-nofpu-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -Os \
	   -mhitachi -ffreestanding -Wall -Wsystem-headers -g
LDFLAGS=-Wl,-static -lfxcg
ARFLAGS=rsv
OBJECTS=printf.o setjmp.o stdio.o stdlib.o ctype.o string.o unistd.o time.o
LIBRARY=../lib/libc.a

all: $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) ../lib/libfxcg.a
	cp ../lib/libfxcg.a ../lib/libc.a
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

clean:
	rm -f $(OBJECTS) $(LIBRARY)
