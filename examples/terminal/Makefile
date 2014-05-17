CC=sh3eb-elf-gcc
OBJCOPY=sh3eb-elf-objcopy
MKG3A=mkg3a
RM=rm
CFLAGS=-m4a-nofpu -mb -Os -mhitachi -Wall -I../../include -lgcc -L../../lib -flto
LDFLAGS=$(CFLAGS) -nostartfiles -T../../toolchain/prizm.x -Wl,-static -Wl,-gc-sections
OBJECTS=main.o
PROJ_NAME=terminal
BIN=$(PROJ_NAME).bin
ELF=$(PROJ_NAME).elf
ADDIN=$(PROJ_NAME).g3a
 
all: $(ADDIN)

$(ADDIN): $(BIN)
	$(MKG3A) -n :Terminal -i uns:unselected.bmp -i sel:selected.bmp $< $@
.s.o:
	$(CC) -c $(CFLAGS) $< -o $@
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
$(ELF): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(BIN)
clean:
	rm -f $(OBJECTS) $(BIN) $(ELF) $(ADDIN)
