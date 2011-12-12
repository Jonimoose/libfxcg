PREFIX=~/sh3eb-elf/bin
CC=$(PREFIX)/sh3eb-elf-gcc
AS=$(PREFIX)/sh3eb-elf-as
AR=$(PREFIX)/sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -m4a-nofpu -mb -Os -nostdlib -Wall -Lr -Iinclude
ARFLAGS=rs
VPATH=src/syscalls
SHSOURCES=$(wildcard src/syscalls/*.S) $(wildcard src/misc/*.S)
CSOURCES=$(wildcard src/misc/*.c)
CXXSOURCES=$(wildcard src/misc/*.cpp)
OBJECTS=$(SHSOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
LIBRARY=libfxcg.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

.S.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(LIBRARY)

