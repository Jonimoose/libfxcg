CC=sh3eb-elf-gcc
MKG3A=mkg3a
RM=rm
CFLAGS=-m4a-nofpu -mb -Os -mhitachi -Wall -nostdlib -I../libfxcg/include -lfxcg -lgcc -L../lib
LDFLAGS=$(CFLAGS) -T../toolchain/prizm.x -Wl,-static -Wl,-gc-sections
CSOURCES=src/example.c
OBJECTS=src/example.o
BIN=Example.bin
ADDIN=$(BIN:.bin=.g3a)
 
all: $(ADDIN)
 
$(BIN): $(OBJECTS)
	$(CC) $(addprefix $(OUTDIR)/,$<)  $(LDFLAGS) -o $@
 
$(ADDIN): $(BIN)
	$(MKG3A) -n :Example $< $@
 
.s.o:
	$(CC) -c $(CFLAGS) $< -o $(OUTDIR)/$@
 
.c.o:
	$(CC) -c $(CFLAGS) $< -o $(OUTDIR)/$@
 
clean:
	$(RM) $(addprefix $(OUTDIR)\,$(OBJECTS)) $(BIN) $(ADDIN)
