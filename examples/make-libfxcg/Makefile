
CC=sh3eb-elf-gcc
MKG3A=mkg3a
RM=rm
CFLAGS= -mb -Os -mhitachi -Wall -Wextra -I../../include -lgcc -L../../lib
LDFLAGS=$(CFLAGS) -nostartfiles -T../../toolchain/prizm.x -Wl,-static -Wl,-gc-sections
CSOURCES=src/example.c
OBJECTS=src/example.o
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
