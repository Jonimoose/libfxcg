
include ../toolchain/base_rules

TOOLCHAIN_PREFIX=sh3eb-elf-
CC=$(TOOLCHAIN_PREFIX)gcc
AS=$(TOOLCHAIN_PREFIX)as
AR=$(TOOLCHAIN_PREFIX)gcc-ar
CFLAGS=-c -ffunction-sections -fdata-sections -Os \
	  $(MACHDEP) -ffreestanding -Wall -Wsystem-headers -g -I../include
LDFLAGS=-Wl,-static -lfxcg
ARFLAGS=rsv
OBJECTS=printf.o setjmp.o stdio.o stdlib.o ctype.o string.o unistd.o time.o
LIBRARY=../lib/libc.a

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

clean:
	$(call rm,$(OBJECTS))
	$(call rm,$(LIBRARY))
