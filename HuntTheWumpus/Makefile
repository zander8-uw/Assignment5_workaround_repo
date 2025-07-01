CC=g++

DEBUG ?= 1

ifeq ($(DEBUG),1)
   OPTFLAGS=-g
else
   OPTFLAGS=-O3
endif

CFLAGS=-I. -I../HuntTheWumpusLib -std=c++20 -Wall -Werror $(OPTFLAGS)

PROJDIR := $(realpath $(CURDIR)/..)
BUILDDIR := $(PROJDIR)/Output

DIRS = HuntTheWumpus

SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(PROJDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir))) ../bin
VPATH = $(SOURCEDIRS)
SOURCES = $(foreach dir, $(SOURCEDIRS), $(wildcard $(dir)/*.cpp))

OBJ := $(subst $(PROJDIR),$(BUILDDIR), $(SOURCES:.cpp=.o))

define generateRules
$(1)/%.o: %.cpp
	$(CC) -c -o $$@ $$< $$(CFLAGS)
endef

HuntTheWumpus: $(OBJ) directories HuntTheWumpusLib
	g++ -L../HuntTheWumpusLib $(OBJ) -o ../bin/HuntTheWumpus -lHuntTheWumpus

HuntTheWumpusLib:
	$(MAKE) -j -C ../HuntTheWumpusLib all

$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	mkdir -p $(TARGETDIRS)

all: directories HuntTheWumpus HuntTheWumpusLib

clean:
	$(MAKE) -j -C ../HuntTheWumpusLib clean
	rm -rf $(TARGETDIRS)
	rm -rf $(BUILDDIR)
