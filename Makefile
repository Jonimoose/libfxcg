PREFIX=~/sh3eb-elf/bin
CC=$(PREFIX)/sh3eb-elf-gcc
AR=$(PREFIX)/sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -fPIC -m4a-nofpu -mb -Os -nostdlib -Wall -Lr -Iinclude
ARFLAGS=rs
VPATH=src/syscalls
SHSOURCES=$(wildcard src/syscalls/*.s) $(wildcard src/misc/*.s)
CSOURCES=$(wildcard src/misc/*.c)
OBJECTS=$(SHSOURCES:.s=.o) $(CSOURCES:.c=.o)
LIBRARY=libfxcg.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

.s.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(LIBRARY)

