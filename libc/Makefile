CC=sh3eb-elf-gcc
AS=sh3eb-elf-as
AR=sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -m4a-nofpu -mb -Os -nostdlib \
	   -Wall -L../lib -I../include -Wsystem-headers
LDFLAGS=-Wl,-static -lfxcg
ARFLAGS=rsv
OBJECTS=printf.o stdio.o stdlib.o ctype.o string.o unistd.o
LIBRARY=../lib/libc.a

all: $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) ../lib/libfxcg.a
	cp ../lib/libfxcg.a ../lib/libc.a
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

clean:
	rm -f $(OBJECTS) $(LIBRARY)

