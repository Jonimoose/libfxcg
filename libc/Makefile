CC=sh3eb-elf-gcc
AS=sh3eb-elf-as
AR=sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -m4a-nofpu -mb -Os -nostdlib \
	   -mhitachi -ffreestanding -Wall -L../lib -I../include -Wsystem-headers
LDFLAGS=-Wl,-static -lfxcg
ARFLAGS=rsv
OBJECTS=qsort.o timezone.o time_table_spd.o asctime_r.o asctime.o gmtime_r.o localtime.o localtime_r.o ctime.o isleap.o \
	vfscanf.o fscanf.o rand.o vsscanf.o __v_scanf.o sscanf.o signal/kill.o wait.o environ.o pwd.o \
	strtoul.o locale.o dirent.o getopt.o termios.o ioctl.o mman.o assert.o utime.o stat.o wchar.o fcntl.o printf.o \
	setjmp.o stdio.o stdlib.o ctype.o string.o unistd.o time.o signal/signal.o signal/raise.o fnmatch.o strtoll.o strcasecmp.o \
	strtoull.o strncasecmp.o atexit.o __dtostr.o
LIBRARY=../lib/libc.a

all: $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) ../lib/libfxcg.a
	cp ../lib/libfxcg.a ../lib/libc.a
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

clean:
	rm -f $(OBJECTS) $(LIBRARY)
