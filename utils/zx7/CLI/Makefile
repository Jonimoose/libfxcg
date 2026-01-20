#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:

CXX=g++
AR=ar

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
SOURCES		:=	../src src
INCLUDES	:=

#---------------------------------------------------------------------------------
# options for code and library generation
#---------------------------------------------------------------------------------

CFLAGS	= -O2 \
		  -Wall \
		  -funroll-loops \
		  -fno-trapping-math \
		  -fno-trapv \
		  -Wno-switch \
			-UTARGET_PRIZM \
		  $(INCLUDE)

# ASFLAGS	=	$(CFLAGS)

# LDFLAGS	= -O2 -Wl,-static -Wl,-gc-sections

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

OUTPUT	:=	$(CURDIR)/zx7

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CXXFILES		:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
CFILES		:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
LD	:=	$(CXX)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) -I$(CURDIR)/.. -I/usr/include/

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

.PHONY: clean

#---------------------------------------------------------------------------------
$(OUTPUT):
	$(CXX) $(CFLAGS) $(CXXFILES) $(CFILES) $< -o $(OUTPUT)

#---------------------------------------------------------------------------------
export CYGWIN := nodosfilewarning
clean:
	$(RM) -f $(OUTPUT)

#---------------------------------------------------------------------------------
