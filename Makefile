CC=sh3eb-elf-gcc.exe
AR=sh3eb-elf-ar.exe
CFLAGS=-c -ffunction-sections -fdata-sections -fPIC -m3 -mb -O9 -nostdlib -Wall -Lr -Iinclude
ARFLAGS=rs
VPATH=src/syscalls
SOURCES=$(wildcard src/syscalls/*.S)
OBJECTS=$(SOURCES:.S=.o)
LIBRARY=libfxcg.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

.S.o:
	$(CC) $(CFLAGS) $< -o $@