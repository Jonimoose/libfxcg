CC=sh3eb-elf-gcc
AS=sh3eb-elf-as
AR=sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -m4a-nofpu -mb -Os -nostdlib -Wall -Lr -I../include
ARFLAGS=rs
VPATH=syscalls
SHSOURCES=$(wildcard syscalls/*.S) $(wildcard misc/*.S)
CSOURCES=$(wildcard misc/*.c)
CXXSOURCES=$(wildcard misc/*.cpp)
OBJECTS=$(SHSOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
LIBRARY=../lib/libfxcg.a

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

