#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:

ifeq ($(strip $(FXCGSDK)),)
export FXCGSDK := $(realpath ../../)
endif

include $(FXCGSDK)/toolchain/prizm_rules


#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	src
INCLUDES	:=

#---------------------------------------------------------------------------------
# options for code and library generation
#---------------------------------------------------------------------------------

CFLAGS	= -Os \
		  -Wall \
		  -funroll-loops \
		  -fno-trapping-math \
		  -fno-trapv \
		  -Wno-switch \
		  -std=c99 \
		  $(MACHDEP) $(INCLUDE) $(DEFINES)

CXXFLAGS =	$(CFLAGS) \
		  -fpermissive \
		  -fno-rtti \
		  -fno-exceptions \
		  -fno-threadsafe-statics \
		  -fno-use-cxa-get-exception-ptr 

ASFLAGS	=	$(CFLAGS) 

LDFLAGS	= $(MACHDEP) -O2 -T$(FXCGSDK)/toolchain/prizm.x -Wl,-static -Wl,-gc-sections

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/../../lib/lib$(TARGET)
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) -I$(LIBFXCG_INC) -I$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(SFILES:.S=.o)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
all: $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(BUILD):
	@[ -d $@ ] || mkdir $@

#---------------------------------------------------------------------------------
export CYGWIN := nodosfilewarning
clean:
	$(RM) -fr $(BUILD) $(OUTPUT).a $(OFILES)

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).a: $(OFILES)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif

