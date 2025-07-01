CC=g++

DEBUG ?= 1

ifeq ($(DEBUG),1)
   OPTFLAGS=-g
else
   OPTFLAGS=-O3
endif

CFLAGS=-I. -I../HuntTheWumpusLib -I../CppUnitLite -std=c++20 -Wall -Werror $(OPTFLAGS)

PROJDIR := $(realpath $(CURDIR)/..)
BUILDDIR := $(PROJDIR)/Output

DIRS = UnitTestHuntTheWumpus

SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(PROJDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir))) ../bin
VPATH = $(SOURCEDIRS)
SOURCES = $(foreach dir, $(SOURCEDIRS), $(wildcard $(dir)/*.cpp))

OBJ := $(subst $(PROJDIR),$(BUILDDIR), $(SOURCES:.cpp=.o))

define generateRules
$(1)/%.o: %.cpp
	$(CC) -c -o $$@ $$< $$(CFLAGS)
endef

UnitTestHuntTheWumpus: $(OBJ) directories HuntTheWumpusLib CppUnitLite
	g++ -L../HuntTheWumpusLib -L../CppUnitLite $(OBJ) -o ../bin/UnitTestHuntTheWumpus -lHuntTheWumpus -lCppUnitLite $(OPTFLAGS)

HuntTheWumpusLib:
	$(MAKE) -j -C ../HuntTheWumpusLib all

CppUnitLite:
	$(MAKE) -j -C ../CppUnitLite all

$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	mkdir -p $(TARGETDIRS)

all: directories HuntTheWumpusLib CppUnitLite UnitTestHuntTheWumpus

clean:
	$(MAKE) -j -C ../HuntTheWumpusLib clean
	$(MAKE) -j -C ../CppUnitLite clean
	rm -rf $(TARGETDIRS)
	rm -rf $(BUILDDIR)
