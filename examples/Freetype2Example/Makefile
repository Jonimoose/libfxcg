
CC=sh3eb-elf-gcc
MKG3A=mkg3a
RM=rm
CFLAGS=-m4a-nofpu -mb -Os -mhitachi -Wall -I../../include -lgcc -L../../lib -L../freetype-2.5.2/objs/.libs -I../freetype-2.5.2/include/ -flto
LDFLAGS=$(CFLAGS) -nostartfiles -T../../toolchain/prizm.x -Wl,-static -Wl,--gc-sections -lfreetype
OBJECTS=main.o
BIN=freetype2.bin
ADDIN=$(BIN:.bin=.g3a)
 
all: $(ADDIN)
 
$(BIN): $(OBJECTS)
	$(CC) $< $(LDFLAGS) -o $@
 
$(ADDIN): $(BIN)
	$(MKG3A) -n :"Freetype2 Example" -i uns:unselected.bmp -i sel:selected.bmp $< $@
 
.s.o:
	$(CC) -c $(CFLAGS) $< -o $@
 
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
 
clean:
	$(RM) $(addprefix $(OUTDIR)\,$(OBJECTS)) $(BIN) $(ADDIN)
