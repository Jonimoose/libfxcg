CC=sh3eb-elf-gcc
AS=sh3eb-elf-as
AR=sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -m4a-nofpu -mb -Os -nostdlib \
	   -mhitachi -ffreestanding -Wall -L../lib -I../include -Wsystem-headers
LDFLAGS=-Wl,-static -lfxcg
ARFLAGS=rsv
OBJECTS=signal/kill.o wait.o environ.o pwd.o strtoul.o locale.o dirent.o getopt.o termios.o ioctl.o mman.o assert.o utime.o stat.o wchar.o fcntl.o printf.o setjmp.o stdio.o stdlib.o ctype.o string.o unistd.o time.o signal/signal.o signal/raise.o
LIBRARY=../lib/libc.a

all: $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) ../lib/libfxcg.a
	cp ../lib/libfxcg.a ../lib/libc.a
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

clean:
	rm -f $(OBJECTS) $(LIBRARY)
