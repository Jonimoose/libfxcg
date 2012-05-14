CC=sh3eb-elf-gcc
AS=sh3eb-elf-as
AR=sh3eb-elf-ar
CFLAGS=-c -ffunction-sections -fdata-sections -m4a-nofpu -mb -Os -nostdlib -Wall -Lr -Iinclude
LDFLAGS=-lfxcg
ARFLAGS=rs
OBJECTS=stdio.o stdlib.o
LIBRARY=libc.a

all: $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS) 

clean:
	rm $(OBJECTS) $(LIBRARY)

