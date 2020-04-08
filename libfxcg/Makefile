TOOLCHAIN_PREFIX=sh3eb-elf-
CC=$(TOOLCHAIN_PREFIX)gcc
AS=$(TOOLCHAIN_PREFIX)as
AR=$(TOOLCHAIN_PREFIX)ar
CFLAGS=-c -ffunction-sections -fdata-sections -Os -Lr -I../include \
	   -mhitachi -m4a-nofpu -flto -std=c99 -Wall -Wextra
ARFLAGS=rs
VPATH=syscalls
SHSOURCES=$(wildcard syscalls/*.S) $(wildcard misc/*.S)
CSOURCES=$(wildcard misc/*.c)
#CXXSOURCES=$(wildcard misc/*.cpp)
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

