
CC=sh3eb-elf-gcc
MKG3A=mkg3a
RM=rm
CFLAGS=-m4a-nofpu -mb -Os -mhitachi -Wall -I../include -lgcc -L../lib
LDFLAGS=$(CFLAGS) -nostartfiles -T../toolchain/prizm.x -Wl,-static -Wl,-gc-sections
CSOURCES=src/example1/example.c
OBJECTS=src/example1/example.o
BIN=Example.bin
ADDIN=$(BIN:.bin=.g3a)
 
all: $(ADDIN)
 
$(BIN): $(OBJECTS)
$(CC) $< $(LDFLAGS) -o $@
 
$(ADDIN): $(BIN)
$(MKG3A) -n :Example $< $@
 
.s.o:
$(CC) -c $(CFLAGS) $< -o $@
 
.c.o:
$(CC) -c $(CFLAGS) $< -o $@
 
clean:
$(RM) $(addprefix $(OUTDIR)\,$(OBJECTS)) $(BIN) $(ADDIN)