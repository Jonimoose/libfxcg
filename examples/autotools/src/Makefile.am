# ----------------------------------------------------
#                 Project settings
# Projects are added to the build system as follows:
# * Create your source files (best in its own folder)
# * Append the project name to 'bin_PROGRAMS'
# * For each target added, make a (target)_SOURCES,
#   (target)_UNS, (target)_SEL, and (target)_NAME.
# ----------------------------------------------------
bin_PROGRAMS = example1 example2
example1_SOURCES = example1/example.c
example2_SOURCES = example2/main.cpp
example1_UNS = ../unselected.bmp
example1_SEL = ../selected.bmp
example2_UNS = ../unselected.bmp
example2_SEL = ../selected.bmp
example1_NAME = Example 1
example2_NAME = Example 2

# ----------------------------------------------------
# No big reason to edit anything below here
# ----------------------------------------------------

AM_CFLAGS = -Os -Wall -mb  -mhitachi -nostdlib  -ffunction-sections -fdata-sections -I@srcdir@/../../include
AM_CXXFLAGS = -Os -Wall -mb  -mhitachi -nostdlib  -ffunction-sections -fdata-sections -I@srcdir@/../../include
AM_LDFLAGS = -mb  -mhitachi -nostdlib -T@srcdir@/../../toolchain/prizm.x  -Wl,-static -Wl,-gc-sections -L@srcdir@/../../lib
LDADD = -lc

all-local: $(bin_PROGRAMS:.bin=.g3a)
%.g3a: $(bin_PROGRAMS)
	@echo mkg3a -n basic:\"$($(@:.g3a=)_NAME)\" -i uns:$($(@:.g3a=)_UNS) -i sel:$($(@:.g3a=)_SEL) $(@:.g3a=.bin) $@
	mkg3a -n "basic:$($(@:.g3a=)_NAME)" -i uns:$($(@:.g3a=)_UNS) -i sel:$($(@:.g3a=)_SEL) $(@:.g3a=.bin) $@
	cp $@ ../bin
clean-local:
	-rm -f *.g3a
