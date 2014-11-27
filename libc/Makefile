TOOLCHAIN_PREFIX=prizm-
CC=$(TOOLCHAIN_PREFIX)gcc
AS=$(TOOLCHAIN_PREFIX)as
AR=$(TOOLCHAIN_PREFIX)ar
CFLAGS=-c -m4a-nofpu -mb -ffunction-sections -fdata-sections -Os \
	   -mhitachi -ffreestanding -Wall -Wsystem-headers -g -I../include -I.
LDFLAGS=-Wl,-static -lfxcg
ARFLAGS=rsv
OBJECTS=printf.o setjmp.o stdio.o stdlib.o ctype.o string.o unistd.o time.o rand.o qsort.o memalign.o strerror.o posix.o stdtime/localtime.o stdtime/asctime.o stdtime/strftime.o __error.o scanf.o strtoul.o
LIBRARY=../lib/libc.a

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS) ../lib/libfxcg.a
	cp ../lib/libfxcg.a ../lib/libc.a
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(LIBRARY)
