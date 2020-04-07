TOOLCHAIN_PREFIX=sh3eb-elf-
CC=$(TOOLCHAIN_PREFIX)gcc
AS=$(TOOLCHAIN_PREFIX)as
AR=$(TOOLCHAIN_PREFIX)ar
CFLAGS=-c -ffunction-sections -fdata-sections -Os \
	   -m4a-nofpu -mhitachi -ffreestanding -Wall -Wsystem-headers -g -I../include
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
