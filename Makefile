CC=sh3eb-elf-gcc.exe
LD=sh3eb-elf-ld.exe
CFLAGS=-c -fPIC -m3 -mb -O9 -nostdlib -Wall -Lr -Iinclude
LDFLAGS=-r
VPATH=src/syscalls
SOURCES=$(wildcard src/syscalls/*.S)
OBJECTS=$(SOURCES:.S=.o)
LIBRARY=libfxcg.o

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

.S.o:
	$(CC) $(CFLAGS) $< -o $@